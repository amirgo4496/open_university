#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


/* -------------------------------------------------------------------------- *
 * Description - Export all the extern symbols mentions to a file.
 * Arguments - sym_list a list of all symbols mentions
 * 		src_fname the name of the file without the matching postfix.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportSymbols(slist_t* symbol_list, char *src_fname, 
			char *postfix,action_t ExportFunc)
{
	char *fname = GetFileName(src_fname, postfix);
	FILE *target_file = fopen(fname, "w+");
	int err_code = 0;
	SListSortSymbols(symbol_list);
	err_code = SListForEach(SListStart(symbol_list),SListEnd(symbol_list),ExportFunc, target_file);
	fclose(target_file);
	free(fname);
	return err_code;
}



/* -------------------------------------------------------------------------- *
 * Description - Export the ocject file.
 * Arguments - mem_img the memory image for the current file
 * 		src_fname the current file name.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportObject(memory_image_t *mem_img, char *src_fname)
{
	char *obj_fname = GetFileName(src_fname, ".ob");
	FILE *obj_file = fopen(obj_fname, "w+");
	int err_code = 0;
	fprintf(obj_file, "%ld %ld\n", mem_img->code_len, mem_img->data_len);
	err_code = MemImageForEach(mem_img, ExportObjectAction, obj_file);	

	fclose(obj_file);
	free(obj_fname);
	return err_code;
}

/* -------------------------------------------------------------------------- *
 * Description - Exports all the given files.
 * Arguments - sym_list a list of all symbols mentions
 * 		src_fname the name of the file without the matching postfix.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportDo(assembler_data_t *assembler_data, char *src_fname)
{
	int err_code = 0;
	err_code = ExportObject(assembler_data->mem_img, src_fname);
	if(!err_code && assembler_data->entry_symbols)
	{
		err_code = ExportSymbols(assembler_data->entry_symbols, src_fname, 
			".ent",ExportEntryAction);
	}
	
	if(!err_code && assembler_data->external_symbols)
	{
		ExportSymbols(assembler_data->external_symbols, src_fname, 
			".ext",ExportExternalAction);
	}
	return err_code;
}


/* -------------------------------------------------------------------------- *
 * Description - Export one line to the object file.
 * Arguments - data - memory cell fromt he memory image
 * 		param the FILE * to be written.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportObjectAction(void *data, void *param)
{
	FILE *obj_file = (FILE *)param;
	int ret = 0;
	memory_cell_t *mem_cell = (memory_cell_t *)data;
	short decimal =  mem_cell->machine_code & (~(1 << 15)); 
	int octal = DecimalToOctal((unsigned)decimal);
	ret = fprintf(obj_file, "%04ld %05d\n",(mem_cell->address + IC_OFFSET), octal);
	return !(ret > 0);
}	

/* -------------------------------------------------------------------------- *
 * Description - Export one line to the entry file.
 * Arguments - data - entry symbol from the list
 * 		param the FILE * to be written.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportEntryAction(void *data ,void *param)
{
	user_data_t *user_data = (user_data_t *)data;
	FILE *entry_file = (FILE *)param;

	fprintf(entry_file, "%s %d\n",user_data->symbol.name, user_data->symbol.value);
	return 0;
}

/* -------------------------------------------------------------------------- *
 * Description - Export one line to the extern file.
 * Arguments - data - extern symbol from the list
 * 		param the FILE * to be written.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportExternalAction(void *data ,void *param)
{
	user_data_t *user_data = (user_data_t *)data;
	FILE *entry_file = (FILE *)param;

	fprintf(entry_file, "%s %04d\n",user_data->symbol.name, user_data->symbol.value);
	return 0;
}

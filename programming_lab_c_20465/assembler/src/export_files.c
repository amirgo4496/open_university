#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


int ExportEntries(slist_t *entry_list, char *src_fname)
{
	char *entry_fname = GetFileName(src_fname, ".ent");
	FILE *entry_file = fopen(entry_fname, "w+");

	SListSortSymbols(entry_list);
	SListForEach(SListStart(entry_list),SListEnd(entry_list),ExportEntryAction, entry_file);
	fclose(entry_file);
	free(entry_fname);
	return 0;
}

int ExportExternals(slist_t* extern_list, char *src_fname)
{
	char *extern_fname = GetFileName(src_fname, ".ext");
	FILE *extern_file = fopen(extern_fname, "w+");

	SListSortSymbols(extern_list);
	SListForEach(SListStart(extern_list),SListEnd(extern_list),ExportExternalAction, extern_file);
	fclose(extern_file);
	free(extern_fname);
	return 0;
}

int ExportObject(memory_image_t *mem_img, char *src_fname)
{
	char *obj_fname = GetFileName(src_fname, ".obj");
	FILE *obj_file = fopen(obj_fname, "w+");
	
	fprintf(obj_file, "%ld %ld\n", mem_img->code_len, mem_img->data_len);
	MemImageForEach(mem_img, ExportObjectAction, obj_file);	

	fclose(obj_file);
	free(obj_fname);
	return 0;
}

int ExportDo(assembler_data_t *assembler_data, char *src_fname)
{
	if(assembler_data->entry_symbols)
	{
		ExportEntries(assembler_data->entry_symbols, src_fname);
	}
	
	if(assembler_data->external_symbols)
	{
		ExportExternals(assembler_data->external_symbols, src_fname);
	}
	ExportObject(assembler_data->mem_img, src_fname);
	return 0;
}

int ExportObjectAction(void *data, void *param)
{
	FILE *obj_file = (FILE *)param;
	memory_cell_t *mem_cell = (memory_cell_t *)data;
	short decimal =  mem_cell->machine_code & (~(1 << 15)); 
	int octal = DecimalToOctal((unsigned)decimal);
	fprintf(obj_file, "%04ld %05d\n",(mem_cell->address + IC_OFFSET), octal);
	return 0;
}	

int ExportEntryAction(void *data ,void *param)
{
	user_data_t *user_data = (user_data_t *)data;
	FILE *entry_file = (FILE *)param;

	fprintf(entry_file, "%s %d\n",user_data->symbol.name, user_data->symbol.value);
	return 0;
}

int ExportExternalAction(void *data ,void *param)
{
	user_data_t *user_data = (user_data_t *)data;
	FILE *entry_file = (FILE *)param;

	fprintf(entry_file, "%s %04d\n",user_data->symbol.name, user_data->symbol.value);
	return 0;
}

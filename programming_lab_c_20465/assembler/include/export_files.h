#ifndef __EXP_FILES__
#define __EXP_FILES__

/* -------------------------------------------------------------------------- *
 * Description - Export the ocject file.
 * Arguments - mem_img the memory image for the current file
 * 		src_fname the current file name.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportObject(memory_image_t *mem_img, char *src_fname);

/* -------------------------------------------------------------------------- *
 * Description - Exports all the given files.
 * Arguments - sym_list a list of all symbols mentions
 * 		src_fname the name of the file without the matching postfix.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportDo(assembler_data_t *assembler_data, char *src_fname);

/* -------------------------------------------------------------------------- *
 * Description - Export one line to the entry file.
 * Arguments - data - entry symbol from the list
 * 		param the FILE * to be written.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportEntryAction(void *symbol ,void *param);

/* -------------------------------------------------------------------------- *
 * Description - Export one line to the extern file.
 * Arguments - data - extern symbol from the list
 * 		param the FILE * to be written.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportExternalAction(void *data ,void *param);

/* -------------------------------------------------------------------------- *
 * Description - Export one line to the object file.
 * Arguments - data - memory cell fromt he memory image
 * 		param the FILE * to be written.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportObjectAction(void *data, void *param);
/* -------------------------------------------------------------------------- *
 * Description - Export all the extern symbols mentions to a file.
 * Arguments - sym_list a list of all symbols mentions
 * 		src_fname the name of the file without the matching postfix.
 * Return - err code (0 for success).
 * -------------------------------------------------------------------------- */
int ExportSymbols(slist_t* symbol_list, char *src_fname, 
			char *postfix,action_t ExportFunc);
#endif

#ifndef __STAGE_TWO__
#define __STAGE_TWO__


/* -------------------------------------------------------------------------- *
 * Description - User wrapper for stage two of the assembler.
 * Arguments - assembler_data_t data structure and metadata used by the assembler.
 * Return - latest error code (0 success).
 * -------------------------------------------------------------------------- */
int S2Do(assembler_data_t *assembler_data);
/* -------------------------------------------------------------------------- *
 * Description - Stage two parser for the source file
 * 		resolves unresolved symbols addresses in the memory image,
 * 		and creates a list of entry and external symbols later to be used
 * 		by the assembler, if error occures it notifies the user.
 * Arguments - src_file
 * 		symbol_table
 * 		macro_table
 * 		mem_img the memory image to be updated
 * 		entry_list the list of entries declerations.
 * 		extern_list the list of extern uses.
 * Return - returns latest error code or 0 for success.
 * -------------------------------------------------------------------------- */
int S2ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, 
			hash_table_t **macro_table, memory_image_t *mem_img,
			slist_t *entry_list, slist_t *extern_list);
/* -------------------------------------------------------------------------- *
 * Description - update a symbol address in the given memory image
 * 		and the given extern list, notifies the user if error.
 * Arguments - current operation
 * 		IC current instruction counter
 * 		mem_image to be updated
 * 		symbol_table
 * 		line counter - current line number
 * 		length of the parsed operation to update IC
 * 		extern_list to be updated.
 * Return - error code or 0 for success.
 * -------------------------------------------------------------------------- */
int S2UpdateSymbol(char *operation, int IC, memory_image_t *mem_img,
			hash_table_t *symbol_table, int line_counter, int *length,
			slist_t *extern_list);
#endif

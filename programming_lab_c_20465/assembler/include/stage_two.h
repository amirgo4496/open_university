#ifndef __STAGE_TWO__
#define __STAGE_TWO__



int S2Do(assembler_data_t *assembler_data);
int S2ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, 
			hash_table_t **macro_table, memory_image_t *mem_img,
			slist_t *entry_list, slist_t *extern_list);

int S2UpdateSymbol(char *operation, int IC, memory_image_t *mem_img,
			hash_table_t *symbol_table, int line_counter, int *length,
			slist_t *extern_list);
#endif

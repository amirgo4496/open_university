#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


int PrintSymbolAction(void *data ,void *param);
int PrintMacroAction(void *data ,void *param);


int main(void)
{

	FILE *src_file = fopen("test_file2", "r+");
	FILE *target_file = fopen("target_file", "w+");

	int i = 0;

	hash_table_t *macro_table = HashCreate(128, HashFunc, PreAsmCompMacroFunc);
	hash_table_t *symbol_table = HashCreate(128, HashFunc, S1CompSymbolFunc);
	memory_image_t *mem_image = NULL;

	

	PreAsmParseSourceFile(src_file, target_file, &macro_table);

	fseek(target_file, 0, SEEK_SET);
	mem_image = S1ParseSourceFile(target_file, &symbol_table, &macro_table);

	fseek(target_file, 0, SEEK_SET);	
	S2ParseSourceFile(target_file, &symbol_table, &macro_table, mem_image, NULL, NULL); 
	printf("\nMACROS:\n");
	HashForEach(macro_table, PrintMacroAction, NULL);
	printf("\nSYMBOLS:\n");
	HashForEach(symbol_table, PrintSymbolAction, NULL);

	printf("\nCode Image:\n");
	MemImagePrint(mem_image);

	printf("\n%lu,%lu\n",mem_image->code_len,mem_image->data_len);	
	HashDestroy(macro_table);
	HashDestroy(symbol_table);

	MemImageDestroy(mem_image);

	fclose(src_file);
	fclose(target_file);
	return 0;
}


int PrintSymbolAction(void *data ,void *param)
{
	user_data_t *user_data = (user_data_t *)data;
	printf("Name: %s --- Value: %d --- Type: %d\n",user_data->symbol.name, user_data->symbol.value, user_data->symbol.type);
	return 0;
}


int PrintMacroAction(void *data ,void *param)
{
	user_data_t *user_data = (user_data_t *)data;
	printf("Name: %s --- Start: %d --- End: %d\n",user_data->macro.name, user_data->macro.start_offset, user_data->macro.end_offset);
	return 0;
}

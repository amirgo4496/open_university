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

	

	macro_table = HashCreate(128, HashFunc, PreAsmCompMacroFunc);
	symbol_table = HashCreate(128, HashFunc, S1CompSymbolFunc);


	PreAsmParseSourceFile(src_file, target_file, &macro_table);

	fseek(target_file, 0, SEEK_SET);
	mem_image = S1ParseSourceFile(target_file, &symbol_table, &macro_table);
 
	printf("\nMACROS:\n");
	HashForEach(macro_table, PrintMacroAction, NULL);
	printf("\nSYMBOLS:\n");
	HashForEach(symbol_table, PrintSymbolAction, NULL);

	printf("\nCode Image:\n");
	MemImagePrint(mem_image);
	
	HashDestroy(macro_table);
	HashDestroy(symbol_table);

	MemImageDestroy(mem_image);

	fclose(src_file);
	fclose(target_file);

	/*int numbers[MAX_LINE_LEN] = {0};
	int i = 0;
	i = S1ParseData("12,-21,22,1231\n", numbers);
	if(i < 0)
	{
		printf("error in input\n");
		return 1;
	}
	while(i--)
	{
		printf("%d\n", numbers[i]);
	}*/
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

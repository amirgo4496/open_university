#include "../src/my_libs.h"
#include "../src/glibc_libs.h"

int main(int argc, char **argv)
{
	FILE *src_file = NULL; 
	FILE *target_file = NULL; 

	hash_table_t *macro_table = NULL;
	hash_table_t *symbol_table = NULL;
	memory_image_t *code_image = NULL;
	memory_image_t *data_image = NULL;

	int i = 0;

	if(argc < 2)
	{
		printf("Please enter file names as inputs like so:\n assembler file1 file2 ...\n");
		return -1;
	}

	for(i = 0; i < argc; ++i)
	{

		src_file = fopen("test_file", "r+");
		target_file = fopen("target_file", "w+");
	
		macro_table = HashCreate(128, HashFunc, PreAsmCompMacroFunc);
		symbol_table = HashCreate(128, HashFunc, S1CompSymbolFunc);

		code_image = MemImageCreate(64);
		data_image = MemImageCreate(64);

		PreAsmParseSourceFile(src_file, target_file, &macro_table);


		HashDestroy(macro_table);
		HashDestroy(symbol_table);

		MemImageDestroy(code_image);
		MemImageDestroy(data_image);

		fclose(src_file);
		fclose(target_file);

	}
	return 0;
}


#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


int main(void)
{
	FILE *src_file = fopen("test_file", "r+");
	FILE *target_file = fopen("target_file", "w+");

	hash_table_t *macro_table = HashCreate(128, HashFunc, PreAsmCompMacroFunc);
	PreAsmParseSourceFile(src_file, target_file, &macro_table);


	HashDestroy(macro_table);
	fclose(src_file);
	fclose(target_file);
	return 0;
	
}

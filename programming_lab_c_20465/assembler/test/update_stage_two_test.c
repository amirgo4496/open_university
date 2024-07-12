#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


int PrintSymbolAction(void *data ,void *param);
int PrintMacroAction(void *data ,void *param);


int main(void)
{

	assembler_data_t assembler_data;
	InitAsmData(&assembler_data);

	PreAsmDo("test_file2", &assembler_data);

	S1Do(&assembler_data);
	S2Do(&assembler_data);
	
	printf("\nMACROS:\n");
	HashForEach(assembler_data.macro_table, PrintMacroAction, NULL);
	printf("\nSYMBOLS:\n");
	HashForEach(assembler_data.symbol_table, PrintSymbolAction, NULL);
	printf("\nCode Image:\n");
	MemImagePrint(assembler_data.mem_img);

	printf("\nExtern Symbols:\n");
	SListForEach(SListStart(assembler_data.external_symbols), SListEnd(assembler_data.external_symbols),
				PrintSymbolAction, NULL);

	printf("\nEntry Symbols:\n");
	SListForEach(SListStart(assembler_data.entry_symbols), SListEnd(assembler_data.entry_symbols),
				PrintSymbolAction, NULL);


	DestroyAsmData(&assembler_data);
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

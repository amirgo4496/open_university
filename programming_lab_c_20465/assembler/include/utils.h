#ifndef __UTILS__
#define __UTILS__

#include "../include/glibc_libs.h"
#include "../include/my_libs.h"


typedef struct assembler_data
{
	hash_table_t **symbol_table;
	hash_table_t **macro_table;
	memory_image_t *mem_img;
	slist_t *external_symbols;
	slist_t *entry_symbols;
	FILE *src_file;	
} assembler_data_t;


char *GetPreAsmTargetFName(char *src_fname);
void DestroyAsmData(assembler_data_t *assembler_data);
void InitAsmData(assembler_data_t *assembler_data);
size_t HashFunc(const void *data);
int GetInstructionType(const char *name);
void PrintBin(unsigned n);
int IsNumber(char *str);
void PrintWordBinary(unsigned short n);
char *TrimPrefix(char *str);
char *TrimPostfix(char *str);
#endif

#ifndef __UTILS__
#define __UTILS__

#include "../include/glibc_libs.h"
#include "../include/my_libs.h"


typedef struct assembler_data
{
	hash_table_t *symbol_table;
	hash_table_t *macro_table;
	memory_image_t *mem_img;
	slist_t *external_symbols;
	slist_t *entry_symbols;
	FILE *src_file;	
} assembler_data_t;

/* -------------------------------------------------------------------------- *
 * Description - Takes a fname as the base name and a postfix to be added
 * 		and returns the input concatenated.
 * 		This function allocates memory for the returned string and its the user
 * 		responsibility to free once unused. 
 * Arguments - fname - The file name base name, postifix to be added.
 * Return - The concatenated string of fname + postfix.
 * -------------------------------------------------------------------------- */
char *GetFileName(char *src_fname, char *postfix);
/* -------------------------------------------------------------------------- *
 * Description -Takes decimal value and input and returns the corresponding octal value.
 * Arguments - decimal - decimal integer.
 * Return - octal representation.
 * -------------------------------------------------------------------------- */
int DecimalToOctal(int decimal);

/* -------------------------------------------------------------------------- *
 * Description -Free all data structure used by the assembler process.
 * Arguments - assembler_data_t that was given to InitAsmData.
 * Return - None.
 * -------------------------------------------------------------------------- */
void DestroyAsmData(assembler_data_t *assembler_data);

/* -------------------------------------------------------------------------- *
 * Description -Initialize all of assembler_data_t members, to later be used by the assembler.
 * Arguments - assembler_data_t to be initialized.
 * Return - None.
 * -------------------------------------------------------------------------- */
void InitAsmData(assembler_data_t *assembler_data);

/* -------------------------------------------------------------------------- *
 * Description -Simple Hash function (Used in the HashTable structures of the assembler).
 * Arguments - void *data to be hashed.
 * Return - size_t hash value.
 * -------------------------------------------------------------------------- */
size_t HashFunc(const void *data);
/* -------------------------------------------------------------------------- *
 * Description -Takes an intruction name and returns its type.
 * Arguments - char *name - name of the instruction.
 * Return - int type of the instruction or 0 if its not an instruction.
 * -------------------------------------------------------------------------- */
int GetInstructionType(const char *name);

/* -------------------------------------------------------------------------- *
 * Description -Helper Function to print a binary representation of short integer.
 * Arguments - n - to be printed.
 * Return - None.
 * -------------------------------------------------------------------------- */
void PrintWordBinary(unsigned short n);
/* -------------------------------------------------------------------------- *
 * Description -Checks whether a given string represent a number
 * 		The string can be prefixed with +/-.
 * Arguments - char *str to be tested.
 * Return - int 1 for valid number 0 otherwise.
 * -------------------------------------------------------------------------- */
int IsNumber(char *str);

/* -------------------------------------------------------------------------- *
 * Description -Takes a string and trims leading spaces.
 * Arguments - char *str to be trimmed.
 * Return - char *str trimmed.
 * -------------------------------------------------------------------------- */
char *TrimPrefix(char *str);
/* -------------------------------------------------------------------------- *
 * Description -Takes a string and trims trailing spaces.
 * Arguments - char *str to be trimmed.
 * Return - char *str trimmed.
 * -------------------------------------------------------------------------- */
char *TrimPostfix(char *str);

#endif

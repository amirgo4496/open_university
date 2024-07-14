#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


/* -------------------------------------------------------------------------- *
 * Description -Takes decimal value and input and returns the corresponding octal value.
 * Arguments - decimal - decimal integer.
 * Return - octal representation.
 * -------------------------------------------------------------------------- */
int DecimalToOctal(int decimal)
{
	int octal = 0;
	int mult = 1, tmp = decimal;
	while(tmp)
	{
		octal += (tmp % 8) * mult;
		tmp /= 8;
		mult *= 10;
	}
	return octal;
}

/* -------------------------------------------------------------------------- *
 * Description - Takes a fname as the base name and a postfix to be added
 * 		and returns the input concatenated.
 * 		This function allocates memory for the returned string and its the user
 * 		responsibility to free once unused. 
 * Arguments - fname - The file name base name, postifix to be added.
 * Return - The concatenated string of fname + postfix.
 * -------------------------------------------------------------------------- */
char *GetFileName(char *fname, char *postfix)
{
	char *target_fname = NULL;
	int len = 0;
	len = strlen(fname);
	target_fname = malloc(len + 5);
	if(!target_fname)
	{
		return NULL;
	}
	strncpy(target_fname, fname, len + 5);
	strcpy((target_fname + len), postfix);
	return target_fname;
}

/* -------------------------------------------------------------------------- *
 * Description -Free all data structure used by the assembler process.
 * Arguments - assembler_data_t that was given to InitAsmData.
 * Return - None.
 * -------------------------------------------------------------------------- */
void DestroyAsmData(assembler_data_t *assembler_data)
{
	if(assembler_data->symbol_table)
	{
		HashDestroy(assembler_data->symbol_table);
	}
	if(assembler_data->macro_table)
	{
		HashDestroy(assembler_data->macro_table);
	}
	if(assembler_data->external_symbols)
	{
		SListDestroy(assembler_data->external_symbols);
	}
	if(assembler_data->entry_symbols)
	{
		SListDestroy(assembler_data->entry_symbols);
	}
	if(assembler_data->mem_img)
	{
		MemImageDestroy(assembler_data->mem_img);
	}
	if(assembler_data->src_file)
	{
		fclose(assembler_data->src_file);
	}
}

/* -------------------------------------------------------------------------- *
 * Description -Initialize all of assembler_data_t members, to later be used by the assembler.
 * Arguments - assembler_data_t to be initialized.
 * Return - None.
 * -------------------------------------------------------------------------- */
void InitAsmData(assembler_data_t *assembler_data)
{
	assembler_data->symbol_table = NULL;
	assembler_data->macro_table = NULL;
	assembler_data->external_symbols = NULL;
	assembler_data->entry_symbols = NULL;
	assembler_data->mem_img = NULL;
	assembler_data->src_file = NULL;
}

/* -------------------------------------------------------------------------- *
 * Description -Simple Hash function (Used in the HashTable structures of the assembler).
 * Arguments - void *data to be hashed.
 * Return - size_t hash value.
 * -------------------------------------------------------------------------- */
size_t HashFunc(const void *data)
{
        size_t i = 0;
        char *str = (char *)data;
        size_t size = 0;
        const int p = 53; /* closest prime to */
        const size_t m = (pow(10, 9) + 9); /* very big prime */
        long hash_value = 0;
        long power = 1;
        size = strlen(str);
        for (i = 0; i < (size - 1); ++i)
        {
                hash_value = (hash_value + (str[i] - 'a' + 1) * power) % m;
                power = (power * p) % m;
        }
        return hash_value;
}

/* -------------------------------------------------------------------------- *
 * Description -Takes an intruction name and returns its type.
 * Arguments - char *name - name of the instruction.
 * Return - int type of the instruction or 0 if its not an instruction.
 * -------------------------------------------------------------------------- */
int GetInstructionType(const char *name)
{
	int ret = 0;
	if(!strcmp(name, ".data"))
	{
		ret = DATA;
	}
	else if(!strcmp(name, ".string"))
	{
		ret = STRING;
	}
	else if(!strcmp(name, ".entry"))
	{
		ret = ENTRY;
	}
	else if(!strcmp(name, ".extern"))
	{
		ret = EXTERN;
	}
	return ret;
}


/* -------------------------------------------------------------------------- *
 * Description -Helper Function to print a binary representation of short integer.
 * Arguments - n - to be printed.
 * Return - None.
 * -------------------------------------------------------------------------- */
void PrintWordBinary(unsigned short n)
{
	int i = 15;
	unsigned short mask = 16384;
	while(i--)
	{
		if(n & mask)
		{
			printf("1");
		}
		else
        	{
            		printf("0");
        	}
		mask >>= 1;
	}
}

/* -------------------------------------------------------------------------- *
 * Description -Takes a string and trims leading spaces.
 * Arguments - char *str to be trimmed.
 * Return - char *str trimmed.
 * -------------------------------------------------------------------------- */
char *TrimPrefix(char *str)
{
	while(*str && isspace(*str))
	{
		++str;
	}
	return str;
}

/* -------------------------------------------------------------------------- *
 * Description -Takes a string and trims trailing spaces.
 * Arguments - char *str to be trimmed.
 * Return - char *str trimmed.
 * -------------------------------------------------------------------------- */
char *TrimPostfix(char *str)
{
	while(*str && !isspace(*str))
	{
		++str;
	}
	*str = '\0';
	return str;
}

/* -------------------------------------------------------------------------- *
 * Description -Checks whether a given string represent a number
 * 		The string can be prefixed with +/-.
 * Arguments - char *str to be tested.
 * Return - int 1 for valid number 0 otherwise.
 * -------------------------------------------------------------------------- */
int IsNumber(char *str)
{
	if('-' == *str || '+' == *str)
	{
		++str;
	}
	while(*str && isdigit(*str))
	{
		++str;
	}
	return '\0' == *str;

}



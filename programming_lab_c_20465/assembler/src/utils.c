#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


char *GetPreAsmTargetFName(char *src_fname)
{
	char *target_fname = NULL;
	int src_len = strlen(src_fname);
	target_fname = malloc(src_len + 4);
	if(!target_fname)
	{
		return NULL;
	}
	strncpy(target_fname, src_fname, src_len + 4);
	strcpy((target_fname + src_len), ".am");
	return target_fname;
}

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

void InitAsmData(assembler_data_t *assembler_data)
{
	assembler_data->symbol_table = NULL;
	assembler_data->macro_table = NULL;
	assembler_data->external_symbols = NULL;
	assembler_data->entry_symbols = NULL;
	assembler_data->mem_img = NULL;
	assembler_data->src_file = NULL;
}

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


/*
	returns type of instruction or 0 if not instruction.
*/
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

char *TrimPrefix(char *str)
{
	while(*str && isspace(*str))
	{
		++str;
	}
	return str;
}

char *TrimPostfix(char *str)
{
	while(*str && !isspace(*str))
	{
		++str;
	}
	*str = '\0';
	return str;
}


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



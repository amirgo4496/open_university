#include "../include/my_libs.h"
#include "../include/glibc_libs.h"

int S2Do(assembler_data_t *assembler_data)
{
	return S2ParseSourceFile(assembler_data->src_file, assembler_data->symbol_table,
					assembler_data->macro_table, assembler_data->mem_img);
}

int S2ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, 
			hash_table_t **macro_table, memory_image_t *mem_img)
{
	char curr_line[MAX_LINE_LEN] = {0}, line_cpy[MAX_LINE_LEN] = {0};
	char *token = NULL;
	int IC = 0, L = 0;
	int line_counter = 0;
	int instruction_type = 0;
	symbol_t *found_symbol = NULL;
	int err_code = 0;
	
	while(fgets(curr_line, MAX_LINE_LEN, src_file))
	{
		L = 0;
		++line_counter;
		strcpy(line_cpy ,curr_line);	
		token = strtok(line_cpy, "\t\n\r\f ");
		if(strchr(token, ':'))
		{
			token = strtok(NULL, "\t\n\r\f ");
		}

		if((instruction_type = GetInstructionType(token)))
		{
			if(ENTRY != instruction_type)
			{
				continue;
			}
			token = strtok(NULL, "\t\n\r\f ");
				
			if(!(found_symbol = HashFind(*symbol_table,token)))
			{
				err_code = UNDECLARED_SYMBOL;
				PrintError(UNDECLARED_SYMBOL, line_counter);
				continue;
			}
			found_symbol->type = ENTRY; 
		}
		else
		{
		
			err_code = S2UpdateSymbol(token, IC, mem_img,
					*symbol_table, line_counter, &L);
		}

		IC += L;
	}
	return err_code;	
}

int S2UpdateSymbol(char *operation, int IC, memory_image_t *mem_img,
			hash_table_t *symbol_table, int line_counter, int *length)
{
	int i = 0;
	int operand_num = GetOperandNum(GetOperation(operation));
	int addr_type = 0;
	int is_reg = 0;
	int err_code = 0;
	int L = 1;
	char *token = NULL;
	memory_cell_t *mem_cell = NULL;
	symbol_t *symbol = NULL;
	for(; i < operand_num; ++i)
	{
		token = strtok(NULL, ",\t\n\r\f ");
		addr_type = GetAddressingType(token);
		switch(addr_type)
		{
			case DIRECT_ADDRESSING:
				symbol = HashFind(symbol_table, token);	
				if(!symbol)
				{
					err_code = UNDECLARED_SYMBOL;
					PrintError(UNDECLARED_SYMBOL, line_counter);
					continue;
				}
				mem_cell = MemImageGetCell(mem_img ,IC + L);
				if(EXTERN == symbol->type)
				{
					mem_cell->machine_code = 1;
				}
				else
				{
					mem_cell->machine_code = (symbol->value << 3) | DIRECT_ADDRESSING;	
				}
				++L;
				break;
			case IMMEDIATE_ADDRESSING:
				++L;
				break;
			case DIRECT_REG_ADDRESSING:
			case INDIRECT_REG_ADDRESSING:
				if(!is_reg)
				{
					++L;
					is_reg = 1;
				}
				break;
		}		
	}
	*length = L;
	return err_code;
}


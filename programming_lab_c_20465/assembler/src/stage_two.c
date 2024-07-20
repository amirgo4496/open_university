#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


/* -------------------------------------------------------------------------- *
 * Description - User wrapper for stage two of the assembler.
 * Arguments - assembler_data_t data structure and metadata used by the assembler.
 * Return - latest error code (0 success).
 * -------------------------------------------------------------------------- */
int S2Do(assembler_data_t *assembler_data)
{
	slist_t *extern_list = SListCreate(), *entry_list = SListCreate();

	if(!extern_list || !entry_list)
	{
		return SLIST_ERR;
	}
	
	assembler_data->entry_symbols = entry_list;
	assembler_data->external_symbols = extern_list;

	return S2ParseSourceFile(assembler_data->src_file, &(assembler_data->symbol_table),
					&(assembler_data->macro_table), assembler_data->mem_img,
					entry_list, extern_list);
}


/* -------------------------------------------------------------------------- *
 * Description - Stage two parser for the source file
 * 		resolves unresolved symbols addresses in the memory image,
 * 		and creates a list of entry and external symbols later to be used
 * 		by the assembler, if error occures it notifies the user.
 * Arguments - src_file
 * 		symbol_table
 * 		macro_table
 * 		mem_img the memory image to be updated
 * 		entry_list the list of entries declerations.
 * 		extern_list the list of extern uses.
 * Return - returns latest error code or 0 for success.
 * -------------------------------------------------------------------------- */
int S2ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, 
			hash_table_t **macro_table, memory_image_t *mem_img,
			slist_t *entry_list, slist_t *extern_list)
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
			if(ENTRY == instruction_type)
			{
				symbol_t ent_sym;
				token = strtok(NULL, "\t\n\r\f ");
				memset(&ent_sym, 0, sizeof(ent_sym));
				if(!(found_symbol = HashFind(*symbol_table,token)))
				{
					err_code = UNDECLARED_SYMBOL;
					PrintError(UNDECLARED_SYMBOL, line_counter);
					continue;
				}
				found_symbol->type = ENTRY;
				strcpy(ent_sym.name, token);
				ent_sym.value = found_symbol->value;
				SListInsertBefore(SListStart(entry_list), (void *)&ent_sym);
			}
			else
			{
				continue;
			}
		}
		else
		{
		
			err_code |= S2UpdateSymbol(token, IC, mem_img,
					*symbol_table, line_counter, &L,
					extern_list);
		}

		IC += L;
	}
	return err_code;	
}
/* -------------------------------------------------------------------------- *
 * Description - update a symbol address in the given memory image
 * 		and the given extern list, notifies the user if error.
 * Arguments - current operation
 * 		IC current instruction counter
 * 		mem_image to be updated
 * 		symbol_table
 * 		line counter - current line number
 * 		length of the parsed operation to update IC
 * 		extern_list to be updated.
 * Return - error code or 0 for success.
 * -------------------------------------------------------------------------- */
int S2UpdateSymbol(char *operation, int IC, memory_image_t *mem_img,
			hash_table_t *symbol_table, int line_counter, int *length,
			slist_t *extern_list)
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
					symbol_t ext_sym;
					memset(&ext_sym, 0, sizeof(ext_sym));
					strcpy(ext_sym.name, token);
					ext_sym.value = IC + IC_OFFSET + L;

					mem_cell->machine_code = 1;
					SListInsertBefore(SListStart(extern_list), (void *)&ext_sym);
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


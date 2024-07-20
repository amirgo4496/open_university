#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


enum S1_PARSER_STATES
{
	ESCAPE = 1,
	QUOTES_START = 2,
	QUOTES_END = 3,
	SIGN = 4,
	COMMA = 5,
	PARSING_NUMBER = 6,
	PARSING_OPERAND = 7
	/*PARSING_DST_OPERAND = 8*/
};



int UpdateSymbolAddress(void *data ,void *param);


/* -------------------------------------------------------------------------- *
 * Description - User Wrapper for the assembler first stage.
 * Arguments - assembler_data_t assembler used data structure and metadata.
 * Return - 0 for success or 1 otherwise.
 * -------------------------------------------------------------------------- */
int S1Do(assembler_data_t *assembler_data)
{
	assembler_data->symbol_table = HashCreate(128, HashFunc, S1CompSymbolFunc);
	assembler_data->mem_img = S1ParseSourceFile(assembler_data->src_file,
		 &(assembler_data->symbol_table), &(assembler_data->macro_table));

	fseek(assembler_data->src_file, 0, SEEK_SET);
	return !assembler_data->mem_img;	
}

/* -------------------------------------------------------------------------- *
 * Description - stage one parser for the source file
 * 		creates the symbol table and memory image fromt he src_file.
 * Arguments - src_file the source file parsed
 * 		symbol_table the symbol table to be used.
 * 		macro_table the macro table created by the pre assembler stage.
 * Return - pointer to a memory image created or NULL if failure occured.
 * -------------------------------------------------------------------------- */
memory_image_t *S1ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, hash_table_t **macro_table) 
{
	char curr_line[MAX_LINE_LEN] = {0} ,line_cpy[MAX_LINE_LEN] = {0};
	char *token = NULL;
	int IC = 0, DC = 0, L = 0;
	int line_counter = 0;
	int type = 0;
	int err = 0;
	int is_symbol = 0;

	symbol_t curr_symbol;
	const operation_t *curr_op;

	memory_image_t *code_img = NULL, *data_img = NULL;
	code_img = MemImageCreate(64);
	data_img = MemImageCreate(64);
	
	while(fgets(curr_line, MAX_LINE_LEN, src_file))
	{
		L = 0;
		is_symbol = 0;
		memset(&curr_symbol, 0, sizeof(symbol_t));
		++line_counter;
		strcpy(line_cpy ,curr_line);	
		token = strtok(line_cpy, "\t\n\r\f ");

		

		if(strchr(token, ':'))
		{
			char *to_copy = NULL;
			token[strlen(token) - 1] = '\0';
			if(!S1IsLabelValid(token))
			{
				PrintError(INVALID_LABEL_DECLARATION, line_counter);
				err = INVALID_LABEL_DECLARATION;
				continue;	
			}
			else if(HashFind(*macro_table, token) || HashFind(*symbol_table, token))
			{
				PrintError(SYMBOL_ALREADY_DEFINED, line_counter);
				err = SYMBOL_ALREADY_DEFINED;
				continue;
			}
			is_symbol = 1;
			token = strtok(token, ":");
			strcpy(curr_symbol.name, token);
			to_copy = strchr(curr_line, ':') + 1;
			memmove(curr_line, to_copy, (strlen(to_copy) + 1));
				
		}


		strcpy(line_cpy, curr_line);
		token = strtok(line_cpy, "\t\n\r\f ");

		if((type = GetInstructionType(token)))
		{
			L = S1HandleInstruction(curr_line, type, DC,
						symbol_table, data_img);

			if(L < 0)
			{
				PrintError(L, line_counter);
				err = L; 
				continue;
			}
			DC += L;
		}
		
		else if((curr_op = GetOperation(token)))
		{
			type = CODE;
			token = token + strlen(token) + 1;	
			L = S1ParseOperation(curr_op, token, IC, code_img);
			if(L < 0)
			{
				PrintError(L, line_counter);
				err = L; 
				continue;
			}
			IC += L;
		}
		else
		{
			PrintError(UNKNOWN_TOKEN, line_counter);
			err = UNKNOWN_TOKEN;
		}

		if(is_symbol && !err)
		{
			curr_symbol.type = type;
			switch(type)
			{
				case DATA:
				case STRING:
					curr_symbol.value = DC - L;
					break;
				case ENTRY:
				case EXTERN:
					PrintError(WARNING_ENTRY_EXTERN_SYMBOL, line_counter);
					continue;
					break;
				case CODE:	
					curr_symbol.value = IC - L + IC_OFFSET;
					break;
			}
			HashInsert(symbol_table ,(void *)&curr_symbol);
		}


			
	}
	if(err)
	{
		MemImageDestroy(code_img);
		MemImageDestroy(data_img);
		return NULL;
	}
	HashForEach(*symbol_table, UpdateSymbolAddress, (void *)IC);
	return MemImageUnite(code_img, data_img, IC);	
}

/* -------------------------------------------------------------------------- *
 * Description - Update symbols addresses values for the symbol table
 * 		for the data and string symbols.
 * Arguments - data - pointer to the current symbol
 * 		param - the calculated IC.
 * Return - 0 for success.
 * -------------------------------------------------------------------------- */
int UpdateSymbolAddress(void *data ,void *param)
{
	symbol_t *symbol = (symbol_t *)data;
	int IC = (int)param;
	if(DATA == symbol->type || STRING == symbol->type)
	{
		symbol->value += IC + IC_OFFSET;
	}
	return 0;
}

/* -------------------------------------------------------------------------- *
 * Description - Parse a a given operation.
 * Arguments - operation the operation to be parsed.
 * 		operands the operands for the given operation.
 * 		IC.
 * 		code_img the memory code image
 * Return - error code (0 for success).
 * -------------------------------------------------------------------------- */
int S1ParseOperation(const operation_t *operation, char *operands, int IC,
			memory_image_t *code_img)
{
	char curr_operand[MAX_LABEL_LEN] = {0};
	int state = COMMA;
	int i = 0, operands_num = 0;
	char c = 0;
	int L = 0;
	int valid_operands_num = 0;
	char parsed_operands[MAX_OPERAND_NUM][MAX_LINE_LEN] = {0};

	
	valid_operands_num = GetOperandNum(operation);
	operands = TrimPrefix(operands);
	while((c = *operands++))
	{
		switch(state)
		{
			case COMMA:
				if(operands_num < valid_operands_num)
				{
					if(isspace(c))
					{
						continue;
					}
					else if(',' == c)
					{
						return PARSE_OPERAND_ERR;;
					}
					curr_operand[i++] = c;
					state = PARSING_OPERAND;
				}
				else
				{
					return PARSE_OPERATION_ERR;
				}
				break;
			case PARSING_OPERAND:
				if(',' == c || '\n' == c)
				{
					state = COMMA;
					strcpy(parsed_operands[valid_operands_num - operands_num - 1], curr_operand);
					++operands_num;
					memset(curr_operand, 0, MAX_LABEL_LEN);
					i = 0;
				}
				else
				{
					curr_operand[i++] = c;
				}	
				break;
		}
	}

	for(i = 0; i < operands_num; ++i)
	{
		TrimPostfix(parsed_operands[i]);
	}
	

	if(!S1IsOperandValid(operation, parsed_operands))
	{
		return PARSE_OPERAND_ERR;
	}

	L = S1WriteOperationToImage(code_img, operation, parsed_operands, IC);

	return L;
}

/* -------------------------------------------------------------------------- *
 * Description - Write the given operation to the memory image.
 * Arguments - code_img the memory image to be written
 * 		operation to be wrote
 * 		operands for the given operation
 * 		the current IC.
 * Return - returns 1 if equal,0 otherwise.
 * -------------------------------------------------------------------------- */
int S1WriteOperationToImage(memory_image_t *code_img ,const operation_t *operation,
				char operands[MAX_OPERAND_NUM][MAX_LINE_LEN], int IC)
{
	int operands_num = 0;
	int L = 1;
	int i = 0;
	int addr_type = 0;
	int regs_counter = 0, is_indirect = 0;
	int operation_shift = 3, operand_shift = 3;
	memory_cell_t code_cells[3];


	operands_num = GetOperandNum(operation);
	
	code_cells[0].machine_code = (operation->op_code << 11) | ABSOLUTE_BIT;;
	code_cells[0].address = IC;

	for(i = 0; i < operands_num; ++i)
	{
		memset(&code_cells[i + 1], 0, sizeof(memory_cell_t));
		addr_type = GetAddressingType(operands[i]);

		code_cells[i + 1].address = IC + L;
		switch(addr_type)
		{
			case IMMEDIATE_ADDRESSING:
				code_cells[i + 1].machine_code |= ((unsigned short)atoi(operands[i] + 1) << 3) | ABSOLUTE_BIT;
				break;
			case INDIRECT_REG_ADDRESSING:
			case DIRECT_REG_ADDRESSING:
				if (INDIRECT_REG_ADDRESSING == addr_type)
					is_indirect = 1;
				else
					is_indirect = 0;
				code_cells[i + 1 - regs_counter].machine_code |= ((unsigned short)GetRegisterNum(operands[i] + is_indirect) << operand_shift) | ABSOLUTE_BIT;
				if(regs_counter)
				{
					--L;
				}
				regs_counter += 1;
				break;
			case DIRECT_ADDRESSING:
				code_cells[i + 1].machine_code = -1;
				break;
		}
		++L;
		code_cells[0].machine_code |= ((unsigned short)addr_type << operation_shift);
		operation_shift += 4;	
		operand_shift += 3;
	}
	if(2 == operands_num && 2 > regs_counter)
	{
		code_cells[2].address -= 1;
		code_cells[1].address += 1;
	}
	for(i = 0; i < L; ++i)
	{
		MemImageInsert(code_img, code_cells[i]);
	}
	return L;
}
/* -------------------------------------------------------------------------- *
 * Description - Checks if the given operands are valid for the given operation.
 * Arguments - operation to be tested from
 * 		operands to be validated.
 * Return - 1 if valid 0 otherwise.
 * -------------------------------------------------------------------------- */
int S1IsOperandValid(const operation_t *operation, char operands[MAX_OPERAND_NUM][MAX_LINE_LEN])
{
	int operands_num = 0;
	int is_valid = 0;

	if(*operands[0])
	{
		if(*operands[1])
		{
			operands_num = 2;
		}
		else
		{
			operands_num = 1;
		}
	}

	
	switch(operands_num)
	{
		case 0:
			is_valid =  !(operation->operands_addressing_types[0] | operation->operands_addressing_types[1]);
			break;
		case 1:
			is_valid = GetAddressingType(operands[0]) & operation->operands_addressing_types[1];
			break;
		case 2:
			is_valid = !!(GetAddressingType(operands[0]) & operation->operands_addressing_types[1]);
			is_valid &= !!(GetAddressingType(operands[1]) & operation->operands_addressing_types[0]);
			break;
	}
	
	return is_valid;

}

/* -------------------------------------------------------------------------- *
 * Description - Parses a string operand for the string instruction.
 * Arguments - The string to be parsed.
 * Return - the length of the parsed string or negative error code for invalid input.
 * -------------------------------------------------------------------------- */
int S1ParseString(char *str)
{
	int len = 0;
	int state = QUOTES_START;

	str = TrimPrefix(str);
	if('\"' != *str++)
	{
		return PARSE_STRING_ERR;
	}
	while(*str && state != QUOTES_END)
	{
		switch(state)
		{
			case ESCAPE:
				state = QUOTES_START;
			case QUOTES_START:
				++len;
				if('\\' == *str)
				{
					state = ESCAPE;
				}
				else if('\"' == *str)
				{
					state = QUOTES_END;
				}
		}
		++str;	
	}
	str = TrimPrefix(str);
	if(*str)
	{
		return PARSE_STRING_ERR;
	}
	return len - 1;
}

/* -------------------------------------------------------------------------- *
 * Description - Parses data operands for the data instruction.
 * Arguments - data_str the data operands string representation
 * 		numbers  to be populated by the parsed data operands.
 * Return - error code or 0 for success.
 * -------------------------------------------------------------------------- */
int S1ParseData(char *data_str, int *numbers)
{
	int state = COMMA;
	int L = 0, i = 0;
	char c = 0;
	char curr_number[MAX_LINE_LEN] = {0};
	int prev = 0;
	data_str = TrimPrefix(data_str);
	while((c = *data_str))
	{
		switch(state)
		{
			case PARSING_NUMBER:
				if(isdigit(c))
				{
					curr_number[i++] = c;
				}
				else if(',' == c || '\n' == c)
				{
					state = COMMA;
					numbers[L++] = atoi(curr_number);
					memset(curr_number, 0, MAX_LINE_LEN);
					i = 0;
				}
				else if((' ' == c || '\t' == c) && COMMA == prev)
				{
					/*Continue Prsing the next char*/
				}
				else
				{
					return PARSE_DATA_ERR;
				}
				prev = PARSING_NUMBER;
				break;
			case COMMA:
				if(isdigit(c))
				{
					state = PARSING_NUMBER;
					curr_number[i++] = c;
				}
				else if('-' == c || '+' == c)
				{
					state = SIGN;
					curr_number[i++] = c;
				}
				else if(' ' == c || '\t' == c)
				{
					/*Continue Prsing the next char*/
				}
				else
				{
					return PARSE_DATA_ERR;
				}
				prev = COMMA;
				break;
			case SIGN:
				if(!isdigit(c))
				{
					return PARSE_DATA_ERR;
				}
				curr_number[i++] = c;
				state = PARSING_NUMBER;
				prev = SIGN;
				break;
		}
		++data_str;
	}
	if(*curr_number)
	{
		numbers[L] = atoi(curr_number);
		++L;
	}

	return L;
	
}
/* -------------------------------------------------------------------------- *
 * Description - update the data memory image according to the to be parsed instruction.
 * Arguments - current line to be parsed containing the instruction
 * 		type  of the instruction
 * 		DC current data counter.
 * 		symbol_table.
 * 		data_image the memory image for the data.
 * Return - data length parsed to update the DC or error code (< 0) for failure.
 * -------------------------------------------------------------------------- */
int S1HandleInstruction(char *line, int type, int DC,
			hash_table_t **symbol_table, memory_image_t *data_img)
{
	int data_length = 0;
	char *token = NULL;
	int i = 0;
	int data_nums[MAX_LINE_LEN / 2] = {0};
	char *str_start = NULL;
	memory_cell_t data_cell;
	symbol_t symbol;

	switch(type)
	{
		case STRING:
			str_start = strstr(line, ".string") + strlen(".string");
			data_length = S1ParseString(str_start);
			if(data_length < 0)
			{
				return PARSE_STRING_ERR;	
			}
			str_start = strchr(line, '\"') + 1;
			for(i = 0; i < data_length; ++i)
			{
				data_cell.address = DC + i;
				data_cell.machine_code = *(str_start + i);
				MemImageInsert(data_img, data_cell);
			}
			data_cell.address = DC + i;
			data_cell.machine_code = '\0';
			MemImageInsert(data_img, data_cell);
			++data_length;
			break;
		case DATA:
			data_length = S1ParseData((strstr(line, ".data") + strlen(".data")), data_nums);
			if(data_length < 0)
			{
				return PARSE_DATA_ERR;
			}
			for(i = 0; i < data_length; ++i)
			{
				data_cell.address = DC + i;
				data_cell.machine_code = data_nums[i];
				MemImageInsert(data_img, data_cell);	
			}
			break;
		case EXTERN:
			token = strtok(NULL, "\t\n\r\f ");
			if(!token || strtok(NULL, "\t\n\r\f "))
			{
				return PARSE_EXTERN_ERR;
			}
			else if(!S1IsLabelValid(token))
			{
				return INVALID_SYMBOL_NAME; 
			}
			else if(HashFind(*symbol_table,token))
			{
				return SYMBOL_ALREADY_DEFINED;
			}
			strcpy(symbol.name, token);
			symbol.value = -1;
			symbol.type = type;
			HashInsert(symbol_table ,(void *)&symbol);
			break;
		case ENTRY:
			break;
	}

	return data_length;
}

/* -------------------------------------------------------------------------- *
 * Description - Checks if the given label name is valid.
 * Arguments - label name.
 * Return - returns 1 if valid 0 otherwise.
 * -------------------------------------------------------------------------- */
int S1IsLabelValid(char *label)
{
	int c = 0, is_op = 0, is_instruction = 0;
	int is_letter_start = 0, len = 0;
	is_op = !!GetOperation(label);
	is_instruction = GetInstructionType(label);
	is_letter_start = isalpha(*label++);

	while((c = *label++) && (isalpha(c) || isdigit(c)) && ++len);
	return is_letter_start && !c && !is_op && !is_instruction && len < 32;
}

/* -------------------------------------------------------------------------- *
 * Description - compares two symbols by name(uniqe).
 * Arguments - data - the current symbol
 * 		symbol_name.
 * Return - returns 1 if equal,0 otherwise.
 * -------------------------------------------------------------------------- */
int S1CompSymbolFunc(const void *data , void *symbol_name)
{
        user_data_t *my_data = (user_data_t *)data;
        return !strcmp((const char *)my_data->symbol.name ,(const char *)symbol_name);
}










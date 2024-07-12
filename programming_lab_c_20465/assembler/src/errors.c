#include "../include/my_libs.h"
#include "../include/glibc_libs.h"

void PrintError(int err_code ,int line_counter)
{
	switch(err_code)
	{
		case 0:
			break;
		case INVALID_MACRO_NAME:
			printf("Invalid macro name was found in pre assembler stage in line %d\n", line_counter);
			break;
		case SYMBOL_ALREADY_DEFINED:
			printf("Redefinition of symbol at line %d\n", line_counter);
			break;
		case WRONG_OPERAND_NUM:
			printf("Invalid number of operands at line %d\n", line_counter);
			break;
		case UNNOWN_OPEARATION:
			printf("Unknown operation at line %d\n", line_counter);
			break;
		case WRONG_OPERAND_TYPE:
			printf("Wrong operand type at line %d\n", line_counter);
			break;
		case UNKNOWN_REGISTER:
			printf("Unknown register at line %d\n", line_counter);
			break;
		case INVALID_SYMBOL_NAME:
			printf("Invalid symbol name at line %d\n", line_counter);
			break;
		case INVALID_LABEL_DECLARATION:
			printf("Invalid label declaration at line %d\n", line_counter);
			break;
		case UNKNOWN_TOKEN:
			printf("Unknown token at line %d\n", line_counter);
			break;
		case PARSE_STRING_ERR:
			printf("Error parsing string at line %d\n", line_counter);
			break;
		case PARSE_DATA_ERR:
			printf("Error parsing data at line %d\n", line_counter);
			break;
		case PARSE_EXTERN_ERR:
			printf("Error parsing extern at line %d\n", line_counter);
			break;
		case INVALID_OPERAND:
			printf("Invalid operand at line %d\n", line_counter);
			break;
		case PARSE_OPERATION_ERR:
			printf("Error parsing operation at line %d\n", line_counter);
			break;
		case WARNING_ENTRY_EXTERN_SYMBOL:
			printf("Useless symbol with extern or entry decleration at line %d\n", line_counter);
			break;
		case PARSE_OPERAND_ERR:
			printf("Error parsing operand at line %d\n", line_counter);
			break;
		case MACRO_ALREADY_DEFINED:
			printf("Macro name redefined at line %d\n", line_counter);
			break;		
		case UNDECLARED_SYMBOL:
			printf("Symbol name without decleration at line %d\n", line_counter);
			break;



	}
}

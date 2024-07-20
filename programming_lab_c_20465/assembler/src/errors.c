#include "../include/my_libs.h"
#include "../include/glibc_libs.h"


/* -------------------------------------------------------------------------- *
 * Description - Takes err_code and line_number where error was found and prints
 * 		an informative error.
 * Arguments - int err_code (documented at errors.h), int line_number.
 * Return - None.
 * -------------------------------------------------------------------------- */
void PrintError(int err_code ,int line_number)
{
	switch(err_code)
	{
		case 0:
			break;
		case INVALID_MACRO_NAME:
			printf("Invalid macro name was found in pre assembler stage in line %d\n", line_number);
			break;
		case SYMBOL_ALREADY_DEFINED:
			printf("Redefinition of symbol at line %d\n", line_number);
			break;
		case WRONG_OPERAND_NUM:
			printf("Invalid number of operands at line %d\n", line_number);
			break;
		case UNNOWN_OPEARATION:
			printf("Unknown operation at line %d\n", line_number);
			break;
		case WRONG_OPERAND_TYPE:
			printf("Wrong operand type at line %d\n", line_number);
			break;
		case UNKNOWN_REGISTER:
			printf("Unknown register at line %d\n", line_number);
			break;
		case INVALID_SYMBOL_NAME:
			printf("Invalid symbol name at line %d\n", line_number);
			break;
		case INVALID_LABEL_DECLARATION:
			printf("Invalid label declaration at line %d\n", line_number);
			break;
		case UNKNOWN_TOKEN:
			printf("Unknown token at line %d\n", line_number);
			break;
		case PARSE_STRING_ERR:
			printf("Error parsing string at line %d\n", line_number);
			break;
		case PARSE_DATA_ERR:
			printf("Error parsing data at line %d\n", line_number);
			break;
		case PARSE_EXTERN_ERR:
			printf("Error parsing extern at line %d\n", line_number);
			break;
		case INVALID_OPERAND:
			printf("Invalid operand at line %d\n", line_number);
			break;
		case PARSE_OPERATION_ERR:
			printf("Error parsing operation at line %d\n", line_number);
			break;
		case WARNING_ENTRY_EXTERN_SYMBOL:
			printf("Useless symbol with extern or entry decleration at line %d\n", line_number);
			break;
		case PARSE_OPERAND_ERR:
			printf("Error parsing operand at line %d\n", line_number);
			break;
		case MACRO_ALREADY_DEFINED:
			printf("Macro name redefined at line %d\n", line_number);
			break;		
		case UNDECLARED_SYMBOL:
			printf("Symbol name without decleration at line %d\n", line_number);
			break;
	}
}

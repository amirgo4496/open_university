#ifndef __ERRORS__
#define __ERRORS__

enum ERROR_CODES
{
	INVALID_MACRO_NAME = -1,
	SYMBOL_ALREADY_DEFINED = -2,
	WRONG_OPERAND_NUM = -3,
	UNNOWN_OPEARATION = -4,
	WRONG_OPERAND_TYPE = -5,
	UNKNOWN_REGISTER = -6,
	INVALID_SYMBOL_NAME = -7,
	INVALID_LABEL_DECLARATION = -8,
	UNKNOWN_TOKEN = -9,
	PARSE_STRING_ERR = -10,
	PARSE_DATA_ERR = -11,
	PARSE_EXTERN_ERR = -12,
	INVALID_OPERAND = -13,
	PARSE_OPERATION_ERR = -14,
	WARNING_ENTRY_EXTERN_SYMBOL = -15,
	PARSE_OPERAND_ERR = -16,
	MACRO_ALREADY_DEFINED = -17,
	UNDECLARED_SYMBOL = -18,
	FOPEN_ERR = -19,
	SLIST_ERR = -20,
	HASH_TABLE_ERR = -21
};

/* -------------------------------------------------------------------------- *
 * Description - Takes err_code and line_number where error was found and prints
 * 		an informative error.
 * Arguments - int err_code (documented at errors.h), int line_number.
 * Return - None.
 * -------------------------------------------------------------------------- */
void PrintError(int err_code ,int line_counter);

#endif

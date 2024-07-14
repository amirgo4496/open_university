#ifndef __STAGE_ONE__
#define __STAGE_ONE__
#include "../include/operations.h"



/* -------------------------------------------------------------------------- *
 * Description - User Wrapper for the assembler first stage.
 * Arguments - assembler_data_t assembler used data structure and metadata.
 * Return - 0 for success or 1 otherwise.
 * -------------------------------------------------------------------------- */
int S1Do(assembler_data_t *assembler_data);

/* -------------------------------------------------------------------------- *
 * Description - Checks if the given label name is valid.
 * Arguments - label name.
 * Return - returns 1 if valid 0 otherwise.
 * -------------------------------------------------------------------------- */
int S1IsLabelValid(char *label);

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
			hash_table_t **symbol_table, memory_image_t *data_img);

/* -------------------------------------------------------------------------- *
 * Description - Parses data operands for the data instruction.
 * Arguments - data_str the data operands string representation
 * 		numbers  to be populated by the parsed data operands.
 * Return - error code or 0 for success.
 * -------------------------------------------------------------------------- */
int S1ParseData(char *data_str, int *numbers);
/* -------------------------------------------------------------------------- *
 * Description - Parses a string operand for the string instruction.
 * Arguments - The string to be parsed.
 * Return - the length of the parsed string or negative error code for invalid input.
 * -------------------------------------------------------------------------- */
int S1ParseString(char *str);
int ParseOperand(char *operand);

/* -------------------------------------------------------------------------- *
 * Description - compares two symbols by name(uniqe).
 * Arguments - data - the current symbol
 * 		symbol_name.
 * Return - returns 1 if equal,0 otherwise.
 * -------------------------------------------------------------------------- */
int S1CompSymbolFunc(const void *data , void *param);
/* -------------------------------------------------------------------------- *
 * Description - Parse a a given operation.
 * Arguments - operation the operation to be parsed.
 * 		operands the operands for the given operation.
 * 		IC.
 * 		code_img the memory code image
 * Return - error code (0 for success).
 * -------------------------------------------------------------------------- */
int S1ParseOperation(const operation_t *operation, char *operands, int IC,
			memory_image_t *code_img);
/* -------------------------------------------------------------------------- *
 * Description - Checks if the given operands are valid for the given operation.
 * Arguments - operation to be tested from
 * 		operands to be validated.
 * Return - 1 if valid 0 otherwise.
 * -------------------------------------------------------------------------- */
int S1IsOperandValid(const operation_t *operation, char operands[MAX_OPERAND_NUM][MAX_LINE_LEN]);
/* -------------------------------------------------------------------------- *
 * Description - Write the given operation to the memory image.
 * Arguments - code_img the memory image to be written
 * 		operation to be wrote
 * 		operands for the given operation
 * 		the current IC.
 * Return - returns 1 if equal,0 otherwise.
 * -------------------------------------------------------------------------- */
int S1WriteOperationToImage(memory_image_t *code_img ,const operation_t *operation,
				char operands[MAX_OPERAND_NUM][MAX_LINE_LEN], int IC);
/* -------------------------------------------------------------------------- *
 * Description - stage one parser for the source file
 * 		creates the symbol table and memory image fromt he src_file.
 * Arguments - src_file the source file parsed
 * 		symbol_table the symbol table to be used.
 * 		macro_table the macro table created by the pre assembler stage.
 * Return - pointer to a memory image created or NULL if failure occured.
 * -------------------------------------------------------------------------- */
memory_image_t *S1ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, hash_table_t **macro_table);
#endif

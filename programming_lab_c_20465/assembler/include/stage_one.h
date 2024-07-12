#ifndef __STAGE_ONE__
#define __STAGE_ONE__
#include "../include/operations.h"



int S1Do(assembler_data_t *assembler_data);
int S1IsLabelValid(char *label);
int S1HandleInstruction(char *line, int type, int DC,
			hash_table_t **symbol_table, memory_image_t *data_img);
int S1ParseData(char *data_str, int *numbers);
int S1ParseString(char *str);
int ParseOperand(char *operand);
int S1CompSymbolFunc(const void *data , void *param);
int S1ParseOperation(const operation_t *operation, char *operands, int IC,
			memory_image_t *code_img);

int S1IsOperandValid(const operation_t *operation, char operands[MAX_OPERAND_NUM][MAX_LINE_LEN]);

int S1WriteOperationToImage(memory_image_t *code_img ,const operation_t *operation,
				char operands[MAX_OPERAND_NUM][MAX_LINE_LEN], int IC);
memory_image_t *S1ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, hash_table_t **macro_table);
/*memory_image_t *S1ParseSourceFile(FILE *src_file, hash_table_t **symbol_table, hash_table_t **macro_table, memory_image_t *code_img, memory_image_t *data_img);*/
#endif

#ifndef __INSTRUCTIONS__
#define __INSTRUCTIONS__

#define OPERATIONS_AMOUNT (16)
#define REGISTERS_AMOUNT (8)
#define EXTERNAL_BIT (1)
#define RELOCATABLE_BIT (2)
#define ABSOLUTE_BIT (4)

#define IMMEDIATE_ADDRESSING (1)
#define DIRECT_ADDRESSING (2)
#define INDIRECT_REG_ADDRESSING (4)
#define DIRECT_REG_ADDRESSING (8)

#define MAX_OPERAND_NUM (2)
#define SRC_OPERAND_IDX (0)
#define DST_OPERAND_IDX (1)
typedef struct instruction
{
	uint8_t op_code;
	uint8_t operands_addressing_types[MAX_OPERAND_NUM];
	char name[5];
} operation_t;

typedef struct reg
{
	char name[4];
	uint8_t num;
} reg_t;



/* -------------------------------------------------------------------------- *
 * Description - returns operation matching with the given name.
 * Arguments - operation ame to be searched.
 * Return - found operation pointer or NULL.
 * -------------------------------------------------------------------------- */
const operation_t *GetOperation(const char *name);

/* -------------------------------------------------------------------------- *
 * Description - returns the number for the given register string representation.
 * Arguments - name- register name as string.
 * Return - int- the parsed register number.
 * -------------------------------------------------------------------------- */
int GetRegisterNum(char *name);
/* -------------------------------------------------------------------------- *
 * Description - returns the addressing type for the given operand.
 * Arguments - operand to be parsed.
 * Return - addressing type or 0 for error.
 * -------------------------------------------------------------------------- */
int GetAddressingType(char *operand);

/* -------------------------------------------------------------------------- *
 * Description - returns the number of legal operands for the given operation.
 * Arguments - operation.
 * Return - number of legal operands.
 * -------------------------------------------------------------------------- */
int GetOperandNum(const operation_t *operation);
#endif

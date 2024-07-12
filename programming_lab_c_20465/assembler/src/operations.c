#include "../include/my_libs.h"
#include "../include/glibc_libs.h"

static const operation_t operations[OPERATIONS_AMOUNT] = 
{
	{0,
	{(IMMEDIATE_ADDRESSING | DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING),
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	"mov"},
	{1,
	{(IMMEDIATE_ADDRESSING | DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING),
	(IMMEDIATE_ADDRESSING | DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	 "cmp"},
	{2,
	{(IMMEDIATE_ADDRESSING | DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING),
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	 "add"},
	{3,
	{(IMMEDIATE_ADDRESSING | DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING),
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	"sub"},
	{4,
	{DIRECT_ADDRESSING,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	"lea"},
	{5,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	"clr"},
	{6,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	"not"},
	{7,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
 	"inc"},
	{8,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	 "dec"},
	{9,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING)},
	 "jmp"},
	{10,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING)},
	 "bne"},
	{11,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	 "red"},
	{12,
	{0,
	(IMMEDIATE_ADDRESSING | DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING | DIRECT_REG_ADDRESSING)},
	 "prn"},
	{13,
	{0,
	(DIRECT_ADDRESSING | INDIRECT_REG_ADDRESSING)},
	 "jsr"},
	{14, {0, 0}, "rts"},
	{15, {0, 0}, "stop"}
};

static const reg_t registers[REGISTERS_AMOUNT] = {{"r0", 0}, {"r1" ,1}, {"r2", 2}, {"r3", 3}, {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7}};
const operation_t *GetOperation(const char *name)
{
	int i = 0;
	const operation_t *op = NULL;
	for(; i < OPERATIONS_AMOUNT && strcmp(name ,operations[i].name); ++i);

	if(i < OPERATIONS_AMOUNT)
	{
		op = &operations[i];
	}
	
	return op;
}

int GetRegisterNum(char *name)
{
	int i = 0;
	for(; i < REGISTERS_AMOUNT && strcmp(registers[i].name, name); ++i);
	return i != REGISTERS_AMOUNT ? i : 0;
}

int GetAddressingType(char *operand)
{
	int type = 0;
	if('#' == *operand && IsNumber(operand + 1))
	{
		type = IMMEDIATE_ADDRESSING;
	}
	else if('*' == *operand && GetRegisterNum(operand + 1))
	{
		type = INDIRECT_REG_ADDRESSING;
	}
	else if(GetRegisterNum(operand))
	{
		type = DIRECT_REG_ADDRESSING;
	}
	else if(S1IsLabelValid(operand))
	{
		type = DIRECT_ADDRESSING; 
	}
	return type;
}

int GetOperandNum(const operation_t *operation)
{
	return !!operation->operands_addressing_types[SRC_OPERAND_IDX] + !!operation->operands_addressing_types[DST_OPERAND_IDX];
}

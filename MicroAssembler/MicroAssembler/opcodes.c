#include "opcodes.h"

uint32_t generate_op_type1(uint32_t opcode, uint32_t operand)
{
	operand = ( operand << 4 ) & OPERAND_1;
	return opcode | operand;
}

uint32_t generate_op_type2(uint32_t opcode, uint32_t first_operand, uint32_t second_operand)
{
	first_operand = (first_operand << 3) & L_OPERAND_2;
	second_operand &= R_OPERAND_2;
	return opcode | first_operand | second_operand;
}

uint32_t generate_op_type3(uint32_t opcode, uint32_t first_operand, uint32_t second_operand)
{
	first_operand = (first_operand << 4) & L_OPERAND_3;
	second_operand = ( (second_operand & 0b00001111) | ( (second_operand & 0b11110000) << 4 ) ) & R_OPERAND_3;
	return opcode | first_operand | second_operand;
}

uint32_t generate_op_type4(uint32_t opcode, uint32_t first_operand, uint32_t second_operand)
{
	first_operand = (first_operand << 4) & L_OPERAND_4;
	second_operand = ( (second_operand & 0b00001111) | ( (second_operand & 0b00010000) << 5 ) ) & R_OPERAND_4;
	return opcode | first_operand | second_operand;
}

uint32_t generate_op_type5(uint32_t opcode, uint32_t operand)
{
	uint32_t word_low = operand & L_OPERAND_5;
	uint32_t word_high = ((operand & ~L_OPERAND_5) << 3) & H_OPERAND_5;
	return opcode | word_low | word_high;
}

uint32_t generate_op_type6(uint32_t opcode, uint32_t first_operand, uint32_t second_operand)
{
	first_operand = ((first_operand & 0b00001111) | ((first_operand & 0b00110000) << 5)) & L_OPERAND_6;
	second_operand = (second_operand << 4) & R_OPERAND_6;
	return opcode | first_operand | second_operand;
}

uint32_t generate_op_type7(uint32_t opcode, uint32_t operand)
{
	operand = ((operand - 1)  << 3) & OPERAND_7;
	return opcode | operand;
}
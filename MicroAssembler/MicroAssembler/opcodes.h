#ifndef OPCODES_H_
#define OPCODES_H_

#include <stdint.h>
#include "opcodes_def.h"

/* Instructions: DEC INC */
uint32_t generate_op_type1(uint32_t opcode, uint32_t operand);

/* Instructions: SBI CBI */
uint32_t generate_op_type2(uint32_t opcode, uint32_t first_operand, uint32_t second_operand);

/* Instructions: LDI */
uint32_t generate_op_type3(uint32_t opcode, uint32_t first_operand, uint32_t second_operand);

/* Instructions: ADD SUB AND OR EOR */
uint32_t generate_op_type4(uint32_t opcode, uint32_t first_operand, uint32_t second_operand);

/* Instructions: JMP */
uint32_t generate_op_type5(uint32_t opcode, uint32_t operand);

/* Instructions: OUT IN */
uint32_t generate_op_type6(uint32_t opcode, uint32_t first_operand, uint32_t second_operand);

/* Instructions: BREQ BRNE*/
uint32_t generate_op_type7(uint32_t opcode, uint32_t operand);

#endif
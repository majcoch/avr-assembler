#include "assembler.h"

long numerical_operand_in_range(int32_t operand_value, int32_t min_value, int32_t max_value, int operand_order, long line) {
	if (operand_value > max_value || operand_value < min_value) {
		printf("line %d: operand %d out of range: %d\n", line, operand_order, operand_value);
		return 1;
	}
	else return 0;
}

long address_operand_in_range(int32_t operand_value, int32_t min_value, int32_t max_value, int operand_order, long line) {
	if (operand_value > max_value || operand_value < min_value) {
		printf("line %d: relative branch out of reach\n", line);
		return 1;
	}
	else return 0;
}

long operand_defined(int exists, long line, char* mnemonic) {
	if (!exists) {
		printf("line %d: undefined symbol: '%s'\n", line, mnemonic);
		return 1;
	}
	else return 0;
}

long assemble_to_binary()
{
	long semantic_error_counter = 0;
	for (int i = 0; i < ins_index; i++)
	{
		int32_t first_operand = 0;
		int32_t second_operand = 0;
		int first_operand_exists = find_label_value(instructions[i].first_operand, &first_operand);
		int second_operand_exists = find_label_value(instructions[i].second_operand, &second_operand);
		long error_counter = 0;
		
		switch (instructions[i].operation_code)
		{
		case DEC_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type1(DEC_OPCODE, first_operand);
			break;

		case INC_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type1(INC_OPCODE, first_operand);
			break;

		case SBI_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += operand_defined(second_operand_exists, instructions[i].line, instructions[i].second_operand);
			error_counter += numerical_operand_in_range(first_operand, 0, 31, 1, instructions[i].line);
			error_counter += numerical_operand_in_range(second_operand, 0, 7, 2, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type2(SBI_OPCODE, first_operand, second_operand);
			break;

		case CBI_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += operand_defined(second_operand_exists, instructions[i].line, instructions[i].second_operand);
			error_counter += numerical_operand_in_range(first_operand, 0, 31, 1, instructions[i].line);
			error_counter += numerical_operand_in_range(second_operand, 0, 7, 2, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type2(CBI_OPCODE, first_operand, second_operand);
			break;

		case LDI_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += operand_defined(second_operand_exists, instructions[i].line, instructions[i].second_operand);
			error_counter += numerical_operand_in_range(first_operand, 16, 31, 1, instructions[i].line);
			error_counter += numerical_operand_in_range(second_operand, -256, 255, 2, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type3(LDI_OPCODE, first_operand, second_operand);
			break;

		case ADD_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type4(ADD_OPCODE, first_operand, second_operand);
			break;

		case SUB_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type4(SUB_OPCODE, first_operand, second_operand);
			break;

		case AND_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type4(AND_OPCODE, first_operand, second_operand);
			break;

		case OR_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type4(OR_OPCODE, first_operand, second_operand);
			break;

		case EOR_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = generate_op_type4(EOR_OPCODE, first_operand, second_operand);
			break;

		case JMP_CODE:
			opcodes[i].length = 4;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += numerical_operand_in_range(first_operand, 0, 0x3FFFF, 1, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type5(JMP_OPCODE, first_operand);
			break;
		case NOP_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			opcodes[i].opcode = NOP_OPCODE;
			break;

		case OUT_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += numerical_operand_in_range(first_operand, 0, 63, 1, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type6(OUT_OPCODE, first_operand, second_operand);
			break;

		case IN_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += numerical_operand_in_range(first_operand, 0, 63, 1, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type6(IN_OPCODE, first_operand, second_operand);
			break;

		case BREQ_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += address_operand_in_range(first_operand - (int32_t)opcodes[i].address, -63, 64, 1, instructions[i].line);
			if(!error_counter) opcodes[i].opcode = generate_op_type7(BREQ_OPCODE, first_operand - opcodes[i].address);
			break;

		case BRNE_CODE:
			opcodes[i].length = 2;
			opcodes[i].address = instructions[i].address;
			error_counter += operand_defined(first_operand_exists, instructions[i].line, instructions[i].first_operand);
			error_counter += address_operand_in_range(first_operand - (int32_t)opcodes[i].address, -63, 64, 1, instructions[i].line);
			if (!error_counter) opcodes[i].opcode = generate_op_type7(BRNE_OPCODE, first_operand - opcodes[i].address);
			break;

		}
		op_index++;
		semantic_error_counter += error_counter;
	}
	return semantic_error_counter;
}
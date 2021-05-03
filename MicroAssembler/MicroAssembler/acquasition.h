#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "config.h"

typedef struct instruction {
	long line;					// Line number in .asm file - error tracking
	int operation_code;			// Program internal code
	char* first_operand;		// String containing mnemonic of left operand
	char* second_operand;		// String containing mnemonic of right operand
	uint32_t address;			// Location in MCU memory
} assembly_instruction;

typedef struct label {
	char* name;					// String containing mnemonic of label
	int32_t value;				// Numerical value - address, number etc.
} assembly_label;

extern assembly_instruction instructions[];
extern assembly_label labels[];
extern long ins_index;
extern long lab_index;

void add_instruction(int operation_code, char* first_operand, char* second_operand, uint32_t address, long line);

void add_label(char* name, int32_t value);

int find_label_value(char* label, int32_t* value);

void clear_instruction_list();

void clear_label_list();

#endif
#ifndef BINARY_H_
#define BINARY_H_

#include <stdio.h>
#include "opcodes.h"
#include "acquasition.h"
#include "opcodes_def.h"

typedef struct binary {
	uint32_t opcode;		// Binary value of opcode in BigEndian
	uint32_t address;		// Address in microcontroller memory where instruction should be placed
	uint8_t length;			// AVR has two types of instructions - 16 and 32 bits long
} binary_instruction;

extern binary_instruction opcodes[];
extern long op_index;

long assemble_to_binary();

#endif
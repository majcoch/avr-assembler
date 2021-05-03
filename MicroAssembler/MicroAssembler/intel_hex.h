#ifndef INTEL_HEX_H_
#define INTEL_HEX_H_

#include <stdint.h>
#include <stdio.h>
#include "assembler.h"

typedef struct hex_file_line
{
	uint8_t data_size;
	uint32_t starting_address;
	uint8_t record_type;
	uint8_t data_payload[16];	// Max number of bytes in one line
	uint8_t checksum;
} hex_file_entry;

int generate_hex_file(char* file_name);

void convert_to_intel_hex();

void print_to_file(FILE* output_file);

uint8_t calculate_check_sum(struct hex_file_line *line);

void convert_to_byte_array(uint32_t value, uint8_t* byte_array, int size);

#endif
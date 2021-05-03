#include "intel_hex.h"

#define MAX_REC_SIZE 16
int record_index = 0;
hex_file_entry hex[MAX_CODE_SIZE];

int generate_hex_file(char* file_name)
{
	FILE* hex_file = NULL;
	fopen_s(&hex_file, file_name, "w");
	if (NULL != hex_file) {
		convert_to_intel_hex();
		print_to_file(hex_file);
		fclose(hex_file);
		return 1;
	}
	else return 0;
}

void convert_to_intel_hex()
{
	int ins_size = 0;		// Size pf current instruction
	int rec_count = 0;		// Number of 2 byte sets in single record
	int rec_size = 0;		// Size of single record in bytes
	int divide_flag = 0;	// 4 byte instruction has to be cut in half if falls between records
	uint32_t prev_addr = 0;
	for (int i = 0; i < ins_index; i += rec_count)
	{
		// Starting new record
		if( 1 == divide_flag ) hex[record_index].starting_address = (opcodes[i].address + 1) * 2;
		else hex[record_index].starting_address = (opcodes[i].address * 2);
		hex[record_index].record_type = 0x00;
		rec_count = 0;	// Reset number of 2byte sets in single record
		for (rec_size = 0; rec_size < MAX_REC_SIZE && (i + rec_count) < ins_index; rec_size += ins_size)
		{

			if (1 == divide_flag)
			{
				convert_to_byte_array(opcodes[i].opcode, &(hex[record_index].data_payload[rec_size]), 2);
				prev_addr = opcodes[i + rec_count].address + 1;
				divide_flag = 0;
				ins_size = 2;
				rec_count++;
			}
			else
			{
				if (opcodes[i + rec_count].address - prev_addr > 1 && 0 != rec_size)
				{
					prev_addr = opcodes[i + rec_count].address;
					break;
				}
				/* Adding 4 byte instruction when only 2 bytes are avaliable */
				if ((rec_size + opcodes[i + rec_count].length) > MAX_REC_SIZE)
				{
					convert_to_byte_array(opcodes[i + rec_count].opcode >> 16, &(hex[record_index].data_payload[rec_size]), 2);
					divide_flag = 1;
					ins_size = 2;
				}
				/* Adding any instruction when there is a place */
				else
				{
					convert_to_byte_array(opcodes[i + rec_count].opcode, &(hex[record_index].data_payload[rec_size]), opcodes[i + rec_count].length);
					ins_size = opcodes[i + rec_count].length;
					if( ins_size == 2 ) prev_addr = opcodes[i + rec_count].address;
					else prev_addr = opcodes[i + rec_count].address + 1;
					rec_count++;
				}
			}
		}
		hex[record_index].data_size = rec_size;
		hex[record_index].checksum = calculate_check_sum(&hex[record_index]);
		record_index++;
	}
}

void print_to_file(FILE* output_file)
{
	//First record in file - type 02
	fprintf(output_file, ":020000020000FC\n");	// Extended Segment Address

	// Data records - type 00;
	for (int i = 0; i < record_index; i++) {
		fprintf(output_file, ":%02X%04X%02X", hex[i].data_size, hex[i].starting_address, hex[i].record_type);
		for (int j = 0; j < hex[i].data_size; j++) {
			fprintf(output_file, "%02X", hex[i].data_payload[j]);
		}
		fprintf(output_file, "%02X\n", hex[i].checksum);
	}

	// Last record in file - type 01
	fprintf(output_file, ":00000001FF\n");	// Terminate hex file contents
}

uint8_t calculate_check_sum(struct hex_file_line *line)
{
	uint8_t address_hi = line->starting_address >> 8;
	uint8_t address_lo = line->starting_address;
	uint32_t sum = line->data_size + address_hi + address_lo + line->record_type;
	for (int i = 0; i < line->data_size; i++) {
		sum += line->data_payload[i];
	}
	return ~sum + 1;	// Calculate two's complemet of the last byte
}

void convert_to_byte_array(uint32_t value, uint8_t* byte_array, int size)
{
	if (2 == size) {
		byte_array[0] = value;
		byte_array[1] = value >> 8;
	}
	else if(4 == size) {
		byte_array[2] = value;
		byte_array[3] = value >> 8;
		byte_array[0] = value >> 16;
		byte_array[1] = value >> 24;
	}
}
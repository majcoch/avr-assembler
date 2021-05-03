#include "acquasition.h"

void add_instruction(int operation_code, char* first_operand, char* second_operand, uint32_t address, long line)
{
	if (ins_index < MAX_CODE_SIZE)
	{
		instructions[ins_index].operation_code = operation_code;
		if (NULL != first_operand)
		{
			instructions[ins_index].first_operand = _strdup(first_operand);
			free(first_operand);	// move to parses.y
		}
		if (NULL != second_operand)
		{
			instructions[ins_index].second_operand = _strdup(second_operand);
			free(second_operand);	// move to parses.y
		}
		instructions[ins_index].address = address;
		instructions[ins_index].line = line;
		ins_index++;
	}
}

void add_label(char* name, int32_t value)
{
	if (lab_index < MAX_LABEL_SIZE)
	{
		labels[lab_index].name = _strdup(name);
		free(name);					// move to parse.y
		labels[lab_index].value = value;
		lab_index++;
	}
}

int find_label_value(char* label, int32_t* value)
{
	if (NULL == label) return 0;
	int32_t result = 0;
	/* Numerical value */
	if ((result = atoi(label)) || label[0] == '0')
	{
		(*value) =  result;
		return 1;
	}
	/* GPWR address */
	else if ( ('R' == label[0]) && ( (result = atoi( &(label[1]) )) || label[1] == '0') )
	{
		(*value) = result;
		return 1;
	}
	/* Tag value */
	else {
		/* Search for mnemonic in label table */
		for (int i = 0; i < lab_index; i++) {
			if (0 == strcmp(label, labels[i].name)) {
				(*value) = labels[i].value;
				return 1;
			}
		}
		return 0;
	}
}

void clear_instruction_list()
{
	for (int i = 0; i < ins_index; i++) {
		if (NULL != instructions[i].first_operand) free(instructions[i].first_operand);
		if (NULL != instructions[i].second_operand) free(instructions[i].second_operand);
	}
}

void clear_label_list()
{
	for (int i = 0; i < lab_index; i++) {
		if (NULL != labels[i].name) free(labels[i].name);
	}
}
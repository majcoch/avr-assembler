/* Memory leak detection */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

	/* Standard libraries */
#pragma warning (disable : 4996)	// fopen unsafe - disable
#include <stdlib.h>
#include <string.h>

	/* Flex and Yacc */
#include "lexer.h"
extern long syntax_error_counter;

/* assmebler stuff */
#include "acquasition.h"
#include "assembler.h"
#include "intel_hex.h"
#include "config.h"

assembly_instruction instructions[MAX_CODE_SIZE];
long ins_index = 0;
assembly_label labels[MAX_LABEL_SIZE];
long lab_index = 0;
binary_instruction opcodes[MAX_CODE_SIZE];
long op_index = 0;

int main( int argc, char* argv[] ) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	char* input_file_name;
	char* output_file_name;
	if (5 == argc) {
		if (!strcmp(argv[1], "-i") && !strcmp(argv[3], "-o")) {
			input_file_name = argv[2];
			output_file_name = argv[4];
		}
		else if (!strcmp(argv[1], "-o") && !strcmp(argv[1], "-i")) {
			input_file_name = argv[4];
			output_file_name = argv[2];
		}
		else {
			printf("inavlid parameters\n");
			return 1;
		}

		FILE* input_file;
		input_file = fopen(input_file_name, "r");
		if (NULL != input_file) {
			set_input_strean(input_file);
			yyparse();
			long semantic_error_counter = assemble_to_binary();
			long errors_total = syntax_error_counter + semantic_error_counter;
			if (0 == errors_total) {
				if (generate_hex_file(output_file_name)) 
					printf("========== Build: 1 succeeded, 0 failed ==========\n");
				else 
					printf("Unable to create output file\n");
			}
			else 
				printf("========== Build: 0 succeeded, 1 failed ==========\n");
			fclose(input_file);
			clear_instruction_list();
			clear_label_list();
		}
		else 
			printf("Unable to open input file\n");
		clear_flex_buffer();
		return 0;
	}
}
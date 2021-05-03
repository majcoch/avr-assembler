%locations	// Enable location support for error messages generation
%defines	// Generate include file with symbols and types
%{
	#define YYERROR_VERBOSE 1	// Generate more detialed error massages

	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	#pragma warning (disable : 4996)

	#include "acquasition.h"
	#include "opcodes_def.h"

	void printError(char* s);
	void yyerror(char *s);
	int yylex();

	long location_counter = 0;
%}

%union
{
    int intValue;
    char *stringValue;
}

%token INCLUDE		".include"
%token EQU			".equ"
%token CSEG			".cseg"
%token ORG			".org"
%token FILE_NAME	"file name"
%token LABEL		"label"

%token LOW			"low function"
%token HIGH			"high function"

%token NUMBER		"number"

%token GPWR_REGISTER	"working register"

%token DEC		"dec"
%token INC		"inc"
%token LDI		"ldi"
%token ADD		"add"
%token SUB		"sub"
%token SBI		"sbi"
%token CBI		"cbi"
%token AND		"and"
%token OR		"or"
%token EOR		"eor"
%token JMP		"jmp"
%token NOP		"nop"
%token OUT		"out"
%token IN		"in"
%token BREQ		"breq"
%token BRNE		"brne";

%%
program: include_section equ_section code_section
		|																{ printf("Empty source file\n"); }
		;

include_section: INCLUDE FILE_NAME equ_section	include_section
		|
		;

equ_section:	EQU  LABEL '=' NUMBER  equ_section						{
																			int32_t label_value;
																			if( !find_label_value($<stringValue>2, &label_value) )
																			{
																				add_label($<stringValue>2, $<intValue>4);
																			}
																			else
																			{
																				char buffer [50];
																				sprintf(buffer, "line %d: invalid redefinition of '%s'", @$.first_line, $<stringValue>2);
																				printError(buffer);
																			}
																		}
		|
		;

code_section: CSEG code_block											
		|
		;

code_block: org_expression instruction_list code_block
		|
		;

org_expression:	 ORG NUMBER												{ 
																			location_counter = $<intValue>2;
																			int32_t label_value;
																			if( find_label_value("FLASHEND", &label_value) ) 
																			{
																				if( location_counter > label_value )
																				{
																					char buffer [50];
																					sprintf(buffer, "line %d: .cseg is beyond end of memory at 0x7fff", @$.first_line);
																					printError(buffer);
																				}
																			}
																			else 
																			{
																				char buffer [50];
																				sprintf(buffer, "line %d: FLASHEND label is not defined", @$.first_line);
																				printError(buffer);
																			}
																			
																		}
		|
		;

instruction_list: instruction instruction_list
				| LABEL ':' instruction instruction_list				{
																			int32_t label_value;
																			if( !find_label_value($<stringValue>1, &label_value) )
																			{
																				add_label($<stringValue>1, $<intValue>3);
																			}
																			else
																			{
																				char buffer [50];
																				sprintf(buffer, "line %d: duplicate label '%s'", @$.first_line, $<stringValue>2);
																				printError(buffer);
																			}
																		}

				| error	instruction_list								{
																			yyerrok;
																		}
				|
				;

instruction:	DEC GPWR_REGISTER										{
																			add_instruction(DEC_CODE, $<stringValue>2, NULL, location_counter, @$.first_line);
																			$<intValue>$ = location_counter; 
																			location_counter += 1; 
																		}

			|	INC GPWR_REGISTER										{
																			add_instruction(INC_CODE, $<stringValue>2, NULL, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	SBI numerical_operand ',' numerical_operand				{
																			add_instruction(SBI_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	CBI numerical_operand ',' numerical_operand				{
																			add_instruction(CBI_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}


			|	LDI GPWR_REGISTER ',' number_operand					{
																			add_instruction(LDI_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	ADD GPWR_REGISTER ',' GPWR_REGISTER							{
																			add_instruction(ADD_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	SUB GPWR_REGISTER ',' GPWR_REGISTER							{
																			add_instruction(SUB_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	AND GPWR_REGISTER ',' GPWR_REGISTER							{
																			add_instruction(AND_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	OR  GPWR_REGISTER ',' GPWR_REGISTER							{
																			add_instruction(OR_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	EOR GPWR_REGISTER ',' GPWR_REGISTER							{
																			add_instruction(EOR_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	JMP numerical_operand									{
																			add_instruction(JMP_CODE, $<stringValue>2, NULL, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 2;
																		}

			|	NOP														{
																			add_instruction(NOP_CODE, NULL, NULL, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	OUT numerical_operand ',' GPWR_REGISTER					{
																			add_instruction(OUT_CODE, $<stringValue>2, $<stringValue>4, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	IN GPWR_REGISTER ',' numerical_operand					{
																			add_instruction(IN_CODE, $<stringValue>4, $<stringValue>2, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	BREQ numerical_operand									{
																			add_instruction(BREQ_CODE, $<stringValue>2, NULL, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}

			|	BRNE numerical_operand									{
																			add_instruction(BRNE_CODE, $<stringValue>2, NULL, location_counter, @$.first_line);
																			$<intValue>$ = location_counter;
																			location_counter += 1;
																		}
			;

number_operand:	numerical_operand
	|			assembly_function
	;

numerical_operand:	NUMBER												{ char* value = malloc(16); itoa($<intValue>1, value, 10); $<stringValue>$ = value; }
	|				LABEL					
	;

assembly_function:	LOW '(' numerical_operand ')'						{
																			int32_t label_value;
																			if( find_label_value($<stringValue>3, &label_value) )
																			{
																				char* value = malloc(16);
																				itoa( (uint8_t)label_value, value, 10);
																				$<stringValue>$ = value;
																				free($<stringValue>3);
																			}
																			else $<stringValue>$ = $<stringValue>3;
																		}

	|				HIGH '(' numerical_operand ')'						{
																			int32_t label_value;
																			if( find_label_value($<stringValue>3, &label_value) )
																			{
																				char* value = malloc(16);
																				itoa( (uint8_t)(label_value >> 8), value, 10);
																				$<stringValue>$ = value;
																				free($<stringValue>3);
																			}
																			else $<stringValue>$ = $<stringValue>3;
																		}
	;

%%

%option yylineno
%{
	#pragma warning (disable : 4996)	// fileno and fopen unsafe versions in lexer.c
	#pragma warning (disable: 4102)		// Unreferenced label in lexer.c file

	/* Standard C libraries */
	#include <stdlib.h>
	#include <string.h>

	/* Flex and Yacc stuff */
	#include "config.h"
	#include "parser.h"
	#include "lexer.h"
	
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;		// Tracking lines numeration
	YY_BUFFER_STATE stos[MAX_INC_FILES];
	int nBufor = 0;
	int tmp_yylineno = 1;
	
	/* Globals */
	long syntax_error_counter = 0;
%}

%x MULTI_LINE
%x ONE_LINE
 
%%
	/* Basic data types for literal values  */
"-"?[0-9]+					{	
								yylval.intValue = atoi(yytext);								
								return NUMBER;
							}

"-"?0x[0-9a-fA-F]+			{	
								yylval.intValue = strtol(yytext, NULL, 16);
								return NUMBER;
							}

"-"?0b[01]+					{
								yylval.intValue = strtol(&(yytext[2]), NULL, 2);
								return NUMBER;
							}

	/* CPU Registers */
R([0-9]|[1-2][0-9]|3[0-1])	{
								yylval.stringValue = strdup(yytext);
								return GPWR_REGISTER;
							}

	/* Assembly language directives */
".include"|".INCLUDE"			return INCLUDE; 
".equ"|".EQU"					return EQU;
".cseg"|".CSEG"					return CSEG;
".org"|".ORG"					return ORG;

	/* Assembly language nuilt-in functions */
"LOW"							return LOW;
"HIGH"							return HIGH;

	/* AVR Assembly instruction set */
"dec"|"DEC"						return DEC;
"inc"|"INC"						return INC;
"ldi"|"LDI"						return LDI;
"add"|"ADD"						return ADD;
"sub"|"SUB"						return SUB;
"sbi"|"SBI"						return SBI;
"cbi"|"CBI"						return CBI;
"and"|"AND"						return AND;
"or"|"OR"						return OR;
"eor"|"EOR"						return EOR;
"jmp"|"JMP"						return JMP;
"nop"|"NOP"						return NOP;
"out"|"OUT"						return OUT;
"in"|"IN"						return IN;
"breq"|"BREQ"					return BREQ;
"brne"|"BRNE"					return BRNE;

	/* Include file name */
["][a-zA-Z0-9_]+".inc"["]		{
									char* file_name = strdup( &yytext[1] );
									file_name[yyleng - 2] = 0;
									FILE* input_file = fopen(file_name, "r");
									free(file_name);

									if( NULL != input_file ) {
										stos[nBufor++] = YY_CURRENT_BUFFER;
										yyin = input_file;
										tmp_yylineno = yylineno;
										yy_switch_to_buffer( yy_create_buffer(yyin, YY_BUF_SIZE) );	
									}
									else printf("line %d: cannot find include file: %s\n",yylineno, yytext);
									return FILE_NAME;
									
								}

<<EOF>>							{
									if(nBufor == 0) yyterminate();
									else
									{
										yy_delete_buffer(YY_CURRENT_BUFFER);
										yy_switch_to_buffer(stos[--nBufor]);
									}
								}

	/* User defined constant */
[a-zA-Z0-9_]+					{
									yylval.stringValue = strdup(yytext);
									return LABEL;
								}

	/* Assembly keysigns */
[=]								return '=';
[,]								return ',';
[:]								return ':';
[)]								return ')';
[(]								return '(';

	/* Skip white characters */
[\n]*
[ ]*
[	]*


	/* Comments management */
<ONE_LINE>.			
<ONE_LINE>\n		BEGIN(INITIAL);
<MULTI_LINE>.		
<MULTI_LINE>"*/"	BEGIN(INITIAL);
"/*"				BEGIN(MULTI_LINE);
"//"|";"			BEGIN(ONE_LINE);

	/* No match found */
.								return *yytext;
%%

int yywrap( ) {
	yylineno = tmp_yylineno;
	return 1;
}

void yyerror(char* s) {
	fprintf(stderr, "line %d: %s\n", yylineno, s);
	syntax_error_counter++;
}

void set_input_strean(FILE *stream) {
	yyin = stream;
	yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
}

void clear_flex_buffer( ) {
	yy_delete_buffer(YY_CURRENT_BUFFER);
}

void printError(char* s) {
	fprintf(stderr, "%s\n", s);
	syntax_error_counter++;
}
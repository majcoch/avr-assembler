#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>

void set_input_strean(FILE* stream);

void clear_flex_buffer();

extern int yyparse();

#endif
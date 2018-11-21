#include <stdio.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef SIZE
#define SIZE 100
#endif

#ifndef TYPES
#define TYPES
typedef enum code {  
	OPS = 0,
	INIT = -1,

    INT_VALUE = 20,
    FLOAT_VALUE = 21,  
    CHAR_VALUE = 22, 

    SEMICOLON = 40,
    COMMA = 41,
    OPEN_PARENTHESIS = 42,
    CLOSE_PARENTHESIS = 43,
    OPEN_BRACES = 44,
    CLOSE_BRACES = 45, 

    MAIN = 50,
    IF = 51,
    ELSE = 52,
    WHILE = 53,
    DO = 54,
    FOR = 55,
    INT = 56,
    FLOAT = 57,
    CHAR = 58,
    IDENTIFIER = 59,

    ADDITION = 60,
    SUBTRACTION = 61,
    MULTIPLICATION = 62,
    DIVISION = 63,
    ASSIGNMENT = 64,

    GREATER_THAN = 70,
    GREATER_THAN_OR_EQUAL = 71,
    LESS_THAN = 72,
    LESS_THAN_OR_EQUAL = 73,
    EQUAL = 74,
    NOT_EQUAL = 75,

    LETTER = 80,
    DIGIT = 81,
	END_OF_FILE = 82
} Code;

typedef enum error {  
    FIRST = 100,
	SECOND = 101,
	THIRD = 102,
	FOURTH = 103,
	FIFTH = 104,
	SIXTH = 105,
	SEVENTH = 106,
	EIGHTH = 107,
	NINTH = 108,
	TENTH = 109,
	ELEVENTH = 110,
	TWELFTH = 111,
	THIRTEENTH = 112,
	FOURTEENTH = 113,
	FIFTEENTH = 114,
	SIXTEENTH = 115,
	SEVENTEENTH = 116,
	EIGHTEENTH = 117,
	NINETEENTH = 118,
	TWENTIETH = 119,
	TWENTY_FIRST = 120,
	TWENTY_SECOND = 121,
	TWENTY_THIRD = 122,
	TWENTY_FOURTH = 123,
	TWENTY_FIFTH = 124,
	TWENTY_SIXTH = 125,
	TWENTY_SEVENTH = 126,
	TWENTY_EIGHTH = 127,
	TWENTY_NINTH = 128,
	THIRTIETH = 129,
	THIRTY_FIRST = 130,
	THIRTY_SECOND = 131,
	THIRTY_THIRD = 132,
	THIRTY_FOURTH = 133
} Error;

typedef struct token {
    Code code;
    char lexeme[SIZE];
    Error error;
	int line;
	int column;
} Token;

typedef struct stack {
	char stackLexeme[SIZE];
	Code code;
	int scope;
	struct stack * next;
} Stack;

typedef enum boolean {
    FALSE =  0,
    TRUE = 1,
} Boolean;

typedef enum blank {  
    SPACE = 32,
    TAB = 9,
    NEWLINE = 10,
	RETURN = 13,
} Blank;

Token token;
Boolean isDiv;
Stack * symbolTable;
int scope, line, column, label;
#endif
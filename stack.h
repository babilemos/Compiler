#include "types.h"

void pop(Stack ** stack);
Boolean peek(char lexeme[]);
void push(char lexeme[], Code type);
Token catchVariable(char lexeme[], int scopeL);
void printTable();
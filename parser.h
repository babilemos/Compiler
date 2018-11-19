#include "types.h"

void program(FILE * file);
void block(FILE * file);
void variables(FILE * file);
void commands(FILE * file);
void basicCommands(FILE * file);
void loop(FILE * file);
Token factor(FILE * file);
Token term(FILE * filen) ; 
void assignment(FILE * file);
void relationalExpression(FILE * file);
Token arithmeticExpression(FILE * file);
Token arithmeticExpressionL(FILE * file);
void programErrorPrinter();
#include "types.h"
#include "stack.h"

// TIRA ELEMENTOS DO ESCOPO ENCERRADO DA TABELA

void pop(Stack ** stack) {
	Stack * aux;
	if (* stack == NULL)
		return;
	else {
		aux = * stack;
		while (aux->scope == scope) {
			* stack = aux->next;
			free(aux);
			aux = * stack;
			if (* stack == NULL)
				return;
		}
	}
}

// VALIDA A DECLARACAO DE VARIAVEL 

Boolean peek(char lexeme[]) {
	Stack * aux = symbolTable;
	if (symbolTable == NULL) {
		return TRUE;
	}
	else {
		while (aux != NULL) {
			if (aux->scope < scope)
				return TRUE;
			if (strcmp(aux->stackLexeme, lexeme) == 0)
				return FALSE;
			aux = aux->next;
		}
	}
	return TRUE;
}

// INSERE ELEMENTO NA TABELA

void push(char lexeme[], Code type) {
	Stack * newNode;
	newNode = (Stack *) malloc(sizeof(Stack));
	strcpy(newNode->stackLexeme, lexeme);
	newNode->scope = scope;
	newNode->code = type;

	if (symbolTable == NULL) {
		symbolTable = newNode;
		newNode->next = NULL;
	}
	else {
		newNode->next = symbolTable;
		symbolTable = newNode;
	}
}

// PROCURA A VARIAVEL NA TABELA

Token catchVariable(char lexeme[], int scopeL) {
	Stack * node = symbolTable;
	Token aux;
	aux.code = OPS;
	strcpy(aux.lexeme, " ");

	if (symbolTable == NULL) {
		return aux;
	}
	else {
		while (node != NULL) {
			if (strcmp(lexeme, node->stackLexeme) == 0) {
				if (node->code == INT_VALUE) {
					aux.code = INT_VALUE;
				}
				else if (node->code == FLOAT_VALUE) {
					aux.code = FLOAT_VALUE;
				}
				else if (node->code == CHAR_VALUE) {
					aux.code = CHAR_VALUE;
				}
				strcpy(aux.lexeme, lexeme);
				return aux;
			}
			else
				node = node->next;
		}
		return aux;
	}
}

// IMPRIME A TABELA INTEIRA

void printTable() {
	Stack * node = symbolTable;
	
	if (node == NULL) {
		printf("\nA tabela esta vazia");
	}
	else {
		while (node != NULL) {
			printf("Lexema: %s \tTipo: %i \tEscopo:%i\n", node->stackLexeme, node->code, node->scope);
			node = node->next;
		}
	}
}
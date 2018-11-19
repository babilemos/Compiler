#include "types.h"

void tokenPrinter() {
    if(token.code == INT_VALUE) {
        printf("%s\n", token.lexeme);
        printf("[VALOR INTEIRO]\n");
    } else if(token.code == FLOAT_VALUE) {
        printf("%s\n", token.lexeme);
        printf("[VALOR FLOAT]\n");
    } else if(token.code == CHAR_VALUE) {
        printf("%s\n", token.lexeme);
        printf("[VALOR CHAR]\n");
    } else if(token.code == SEMICOLON) {
        printf(";\n");
        printf("[PONTO E VIRGULA]\n");
    } else if(token.code == COMMA) {
        printf(",\n");
        printf("[VIRGULA]\n");
    } else if(token.code == OPEN_PARENTHESIS) {
        printf("(\n");
        printf("[ABRE PARENTESES]\n");
    } else if(token.code == CLOSE_PARENTHESIS) {
        printf(")\n");
        printf("[FECHA PARENTESES]\n");
    } else if(token.code == OPEN_BRACES) {
        printf("{\n");
        printf("[ABRE CHAVES]\n");
    } else if(token.code == CLOSE_BRACES) {
        printf("}\n");
        printf("[FECHA CHAVES]\n");
    } else if(token.code == MAIN) {
        printf("main\n");
        printf("[MAIN - PALAVRA RESERVADA]\n");
    } else if(token.code == IF) {
        printf("if\n");
        printf("[IF - PALAVRA RESERVADA]\n");
    } else if(token.code == ELSE) {
        printf("else\n");
        printf("[ELSE - PALAVRA RESERVADA]\n");
    } else if(token.code == WHILE) {
        printf("while\n");
        printf("[WHILE - PALAVRA RESERVADA]\n");
    } else if(token.code == DO) {
        printf("do\n");
        printf("[DO - PALAVRA RESERVADA]\n");
    } else if(token.code == FOR) {
        printf("for\n");
        printf("[FOR - PALAVRA RESERVADA]\n");
    } else if(token.code == INT) {
        printf("int\n");
        printf("[INT - PALAVRA RESERVADA]\n");
    } else if(token.code == FLOAT) {
        printf("float\n");
        printf("[FLOAT - PALAVRA RESERVADA]\n");
    } else if(token.code == CHAR) {
        printf("char\n");
        printf("[CHAR - PALAVRA RESERVADA]\n");
    } else if(token.code == IDENTIFIER) {
        printf("%s\n", token.lexeme);
        printf("[IDENTIFICADOR]\n");
    } else if(token.code == ADDITION) {
        printf("+\n");
        printf("[OPERADOR DE SOMA]\n");
    } else if(token.code == SUBTRACTION) {
        printf("-\n");
        printf("[OPERADOR DE SUBTRACAO]\n");
    } else if(token.code == MULTIPLICATION) {
        printf("*\n");
        printf("[OPERADOR DE MULTIPLICACAO]\n");
    } else if(token.code == DIVISION) {
        printf("/\n");
        printf("[OPERADOR DE DIVISAO]\n");
    } else if(token.code == ASSIGNMENT) {
        printf("=\n");
        printf("[OPERADOR DE ATRIBUICAO]\n");
    } else if(token.code == GREATER_THAN) {
        printf(">\n");
        printf("[OPERADOR MAIOR QUE]\n");
    } else if(token.code == GREATER_THAN_OR_EQUAL) {
        printf(">=\n");
        printf("[OPERADOR MAIOR QUE OU IGUAL]\n");
    } else if(token.code == LESS_THAN) {
        printf("<\n");
        printf("[OPERADOR MENOR QUE]\n");
    } else if(token.code == LESS_THAN_OR_EQUAL) {
        printf("<=\n");
        printf("[OPERADOR MENOR QUE OU IGUAL]\n");
    } else if(token.code == EQUAL) {
        printf("==\n");
        printf("[OPERADOR DE IGUALDADE]\n");
    } else if(token.code == NOT_EQUAL) {
        printf("!=\n");
        printf("[OPERADOR DE DIFERENCA]\n");
    }
} 

void scannerErrorPrinter() {
	if(token.error == FIRST) {
		printf("ERRO NO SCANNER - linha %d, coluna %d, ultimo token lido %s: CARACTERE INVALIDO\n", token.line, token.column, token.lexeme);
	} else if(token.error == SECOND) {
		printf("ERRO NO SCANNER - linha %d, coluna %d, ultimo token lido %s: FLOAT MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRD) {
		printf("ERRO NO SCANNER - linha %d, coluna %d, ultimo token lido %s: COMENTARIO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == FOURTH) {
		printf("ERRO NO SCANNER - linha %d, coluna %d, ultimo token lido %s: VALOR CHAR MAL FORMADO\n", token.line, token.column, token.lexeme);
	}
}

Token scanner(FILE * file) {
    char character; 
	int counter = 0;
	strcpy(token.lexeme, "\0");
	while(TRUE) {
		Start: character = fgetc(file); ++column; 

		if(character == SPACE) { 
			while(character == SPACE) { character = fgetc(file); ++column; }
		}

		// OPERADORES 

		if(character == '+') { 
			token.code = ADDITION; 
		} else if(character == '-') { 
			token.code = SUBTRACTION; 
		} else if(character == '*') { 
			token.code = MULTIPLICATION; 
		} else if(character == '/') { 
			if((character = fgetc(file)) == '/') {   // COMENTARIOS
				++column;
				while(character != NEWLINE && !feof(file)) {
					character = fgetc(file);
					++column;
				}
				if(feof(file)) {
					token.code = END_OF_FILE;
				} else {
					continue;
				}
			} else if(character == '*') {
				Restart: while(character != '*' && !feof(file)) {
							character = fgetc(file);
							++column;
							if(character == NEWLINE) {
								++line;
							}
						 }
						 if(feof(file)) {
							token.error = THIRD;
							token.line = ++line; token.column = column;
							scannerErrorPrinter();
							exit(EXIT_SUCCESS);
						} else if(character == '*') {
							character = fgetc(file);
							++column;
							if(feof(file)) {
								token.error = THIRD;
								token.line = ++line; token.column = column;
								scannerErrorPrinter();
								exit(EXIT_SUCCESS);
							}
							else if(character == '/') {
								++line;
								continue;
							} else {
								goto Restart;
							}
						 }
			} else { 
				token.code = DIVISION; 
						ungetc(character, file);
					} 
		} else if(character == '=') { 
			if((character = fgetc(file)) == '=') { 
				token.code = EQUAL; ++column; 
			} else {
				token.code = ASSIGNMENT;
				ungetc(character, file);
			}
		} else if(character == '!') { 
			if((character = fgetc(file)) == '=') { 
				token.code = NOT_EQUAL; ++column; 
			} else {
				token.error = FIRST; 
				token.line = ++line; token.column = column;
				scannerErrorPrinter(); 
				exit(EXIT_SUCCESS);
			}
		} else if(character == '>') { 
			if((character = fgetc(file)) == '=') {
				token.code = GREATER_THAN_OR_EQUAL; ++column; 
			} else {
				token.code = GREATER_THAN;
				ungetc(character, file);
			}
		} else if(character == '<') { 
			if((character = fgetc(file)) == '=') { 
				token.code = LESS_THAN_OR_EQUAL; ++column; 
			} else {
				token.code = LESS_THAN;
				ungetc(character, file);
			}
		} else if(character == '(') { 
			token.code = OPEN_PARENTHESIS; 
		} else if(character == ')') { 
			token.code = CLOSE_PARENTHESIS; 
		} else if(character == '{') { 
			token.code = OPEN_BRACES; 
		} else if(character == '}') { 
			token.code = CLOSE_BRACES; 
		} else if(character == ',') { 
			token.code = COMMA; 
		} else if(character == ';') { 
			token.code = SEMICOLON; 
		}

		// IDENTIFICADORES E PALAVRAS RESERVADAS
		
		else if(character == '_' || isalpha(character)) {
			for(counter = 0; character == '_' || isalpha(character) || isdigit(character); ++counter) {
				token.lexeme[counter] = character; ++column;
				character = fgetc(file);
			} --column;
			ungetc(character, file);
			token.lexeme[counter] = '\0';
			if(strcmp(token.lexeme, "if") == 0) { 
				token.code = IF; 
			} else if(strcmp(token.lexeme, "else") == 0) { 
				token.code = ELSE; 
			} else if(strcmp(token.lexeme, "while") == 0) { 
				token.code = WHILE; 
			} else if(strcmp(token.lexeme, "do") == 0) { 
				token.code = DO; 
			} else if(strcmp(token.lexeme, "main") == 0) { 
				token.code = MAIN; 
			} else if(strcmp(token.lexeme, "for") == 0) { 
				token.code = FOR; 
			} else if(strcmp(token.lexeme, "int") == 0) { 
				token.code = INT; 
			} else if(strcmp(token.lexeme, "float") == 0) { 
				token.code = FLOAT; 
			} else if(strcmp(token.lexeme, "char") == 0) { 
				token.code = CHAR; 
			} else { 
				token.code = IDENTIFIER; 
			}
		}

		// NUMEROS
		
		else if(isdigit(character)) {
			for (counter = 0; isdigit(character); ++counter) {
				token.lexeme[counter] = character;
				character = fgetc(file); ++column;
			}
			if(character == '.') {
				token.lexeme[counter] = character;
				character = fgetc(file); ++column; ++counter;
				if(isdigit(character)) {
					for (; isdigit(character); ++counter) {
						token.lexeme[counter] = character;
						character = fgetc(file); ++column;
					}
					token.lexeme[counter] = '\0';
					ungetc(character, file);
					token.code = FLOAT_VALUE;
				} else {
					ungetc(character, file);
					token.error = SECOND;
					token.lexeme[counter] = '\0';
					token.line = ++line; token.column = column;
					scannerErrorPrinter();
					exit(EXIT_SUCCESS);
				}
			} else {
				ungetc(character, file);
				token.lexeme[counter] = '\0';
				token.code = INT_VALUE;
			}
		} else if(character == '.') {
			token.lexeme[counter] = character; ++counter;
			character = fgetc(file); ++column; 
			if(isdigit(character)) {
				token.lexeme[0] = '0';
				token.lexeme[counter] = '.'; ++counter;
				for (; isdigit(character); ++counter) {
					token.lexeme[counter] = character;
					character = fgetc(file); ++column;
				}
				token.lexeme[counter] = '\0';
				ungetc(character, file);
				token.code = FLOAT_VALUE;
			} else {
				token.error = SECOND;
				token.lexeme[counter] = '\0';
				token.line = ++line; token.column = column;
				scannerErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		}

		// CHAR

		else if(character == '\'') {
			token.lexeme[counter] = character;
			character = fgetc(file); ++column; ++counter;
			token.lexeme[counter] = character;
			character = fgetc(file); ++column; ++counter;
			token.lexeme[counter] = character; ++counter;
			token.lexeme[counter] = '\0';
			if(character == '\'') {
				token.lexeme[0] = token.lexeme[1];
				token.lexeme[1] = '\0';
				token.code = CHAR_VALUE;
			} else {
				token.error = FOURTH;
				token.line = ++line; token.column = column;
				scannerErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		}

		// CARACTERES BRANCOS

		else if(character == TAB) {
			column += 3; goto Start;
		} else if(character == NEWLINE) {
			column = 0, ++line; goto Start;
		} else if(character == RETURN) {
			column = 0; goto Start;
		} else if(feof(file)) {
			token.code = END_OF_FILE;
		} else { 
			token.error = FIRST; 
			token.line = ++line; token.column = column;
			scannerErrorPrinter(); 
			exit(EXIT_SUCCESS); 
		}
		End: token.line = ++line; token.column = column; tokenPrinter(); return token;  
	}        
}
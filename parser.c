#include "types.h"
#include "scanner.h"
#include "parser.h"
#include "stack.h"

// CHECA ESTRUTURA INICIAL DO PROGRAMA

void program(FILE * file) {
	//<programa> ::= int main"("")" <bloco>
	token = scanner(file);
	if(token.code == INT) {
		token = scanner(file);
		if(token.code == MAIN) {
			token = scanner(file);
			if(token.code == OPEN_PARENTHESIS) {
				token = scanner(file);
				if(token.code == CLOSE_PARENTHESIS) {
					token = scanner(file); scope = 0; symbolTable = NULL;
					block(file); 
				} else {
					token.error = EIGHTH;
					programErrorPrinter();
					exit(EXIT_SUCCESS);
				}
			} else {
				token.error = SEVENTH;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		} else {
			token.error = SIXTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	} else {
		token.error = FIFTH;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
}

// CHECA ESTRUTURA DOS BLOCOS

void block(FILE * file) {
	//<bloco> ::= �{� {<decl_var>}* {<comando>}* �}�
	if(token.code == OPEN_BRACES) {
		scope++; token = scanner(file);
		while(token.code == INT || token.code == FLOAT || token.code == CHAR) {
			variables(file);
		}
		while(token.code == IDENTIFIER || token.code == IF || token.code == DO || token.code == WHILE || token.code == FOR || token.code == OPEN_BRACES) {
			commands(file);
		}

		if(token.code == CLOSE_BRACES) {
			pop(&symbolTable);
			scope--;
			token = scanner(file);

			//printTable();

		} else {
			token.error = ELEVENTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	} else {
		token.error = NINTH;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
}

// CHECA DECLARACOES DE VARIAVEIS 

void variables(FILE * file) {
	//<decl_var> ::= <tipo> <id> {,<id>}* ";"
	Boolean returnType;
	Code type;
	char lexeme[SIZE];
	if(token.code == INT || token.code == FLOAT || token.code == CHAR) {
		if(token.code == INT) {	type = INT_VALUE; }
		else if(token.code == FLOAT) { type = FLOAT_VALUE; }
		else { type = CHAR_VALUE; }
		token = scanner(file);
		Id: if(token.code == IDENTIFIER) {
			strcpy(lexeme, token.lexeme); 
			returnType = peek(lexeme);
			if(returnType == TRUE) {
				push(lexeme, type);
				token = scanner(file);
				if(token.code == COMMA) { 
					token = scanner(file);
					goto Id; 
				} else if(token.code == SEMICOLON) {
					token = scanner(file);
					return;
				} else {
					token.error = TWENTY_EIGHTH;
					programErrorPrinter();
					exit(EXIT_SUCCESS);
				}
			} else {
				token.error = THIRTIETH;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		}
	} else {
		token.error = TENTH;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
}

// CHECA ORDEM DOS COMANDOS

void commands(FILE * file) {
	//<comando> ::= <comando_b�sico> | <itera��o> | if "("<expr_relacional>")" <comando> {else <comando>}?
	int lbl;
	if(token.code == IDENTIFIER || token.code == OPEN_BRACES) {
		basicCommands(file);
	}
	else if(token.code == WHILE || token.code == DO) {
		loop(file);
	}
	else if(token.code == IF) {
		token = scanner(file);
		if(token.code == OPEN_PARENTHESIS) {
			token = scanner(file);
			relationalExpression(file);
			if(token.code == CLOSE_PARENTHESIS) {
				++label; lbl = label; printf("if T%d == 0 goto L%d\n", temp, lbl);
				token = scanner(file);
				commands(file);
				printf("L%d: \n", lbl);
				if(token.code == ELSE) {
					token = scanner(file);
					commands(file);
				}
			} else {
				token.error = THIRTEENTH;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		} else {
			token.error = TWELFTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	} 
}

// CHECA AS EXPRESSOES RELACIONAIS

void relationalExpression(FILE * file) {
	//<expr_relacional> ::= <expr_arit> <op_relacional> <expr_arit>
	int cd;
	Boolean isConvertion = FALSE;
	Token temp1, temp2; temp1.code = temp2.code = INIT;
	temp1 = arithmeticExpression(file);
	if(token.code == EQUAL || token.code == NOT_EQUAL || token.code == LESS_THAN || token.code == LESS_THAN_OR_EQUAL || token.code == GREATER_THAN || token.code == GREATER_THAN_OR_EQUAL) {
		cd = token.code;
		token = scanner(file);
		temp2 = arithmeticExpression(file);
		if(temp1.code == INT_VALUE && temp2.code == FLOAT_VALUE) {
			temp1.code = FLOAT_VALUE;
			++temp; printf("\nT%d = (float) %s\n", temp, temp1.lexeme); isConvertion = TRUE;
		}
		else if(temp1.code == FLOAT_VALUE && temp2.code == INT_VALUE) {
			temp2.code = FLOAT_VALUE;
			++temp; printf("\nT%d = (float) %s\n", temp, temp2.lexeme); isConvertion = TRUE;
		}
		else if(temp1.code == CHAR_VALUE && temp2.code != CHAR_VALUE) {
			token.error = THIRTY_SECOND;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else if(temp1.code == INT_VALUE && temp2.code != INT_VALUE) {
			token.error = THIRTY_THIRD;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else if(temp1.code == FLOAT_VALUE && temp2.code == CHAR_VALUE) {
			token.error = THIRTY_FOURTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
		++temp; printf("T%d = %s", temp, temp1.lexeme);
		if(cd == EQUAL) { printf(" == "); }
		else if(cd == NOT_EQUAL) { printf(" != "); }
		else if(cd == LESS_THAN) { printf(" < "); }
		else if(cd == LESS_THAN_OR_EQUAL) { printf(" <= "); }
		else if(cd == GREATER_THAN) { printf(" > "); }
		else { printf(" >= "); }
		if(isConvertion == TRUE) { printf("T%d", temp); }
		else { printf("%s ", temp2.lexeme); }
		printf("\n");
	} else {
		token.error = TWENTY_FOURTH;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
}

// CHECA OS FATORES

Token factor(FILE * file) {
	//<fator> ::= �(� <expr_arit> �)� | <id> | <real> | <inteiro> | <char>
	Token temp1; temp1.code = INIT;
	if(token.code == OPEN_PARENTHESIS) {
		token = scanner(file);
		temp1 = arithmeticExpression(file);
		if(token.code == CLOSE_PARENTHESIS) {
			token = scanner(file);
			return temp1;
		} else {
			token.error = TWENTY_SIXTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	}
	else if(token.code == IDENTIFIER) {
		temp1 = catchVariable(token.lexeme, scope);
		if(temp1.code == OPS) {
			token.error = THIRTY_FIRST;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else {
			token = scanner(file);
			return temp1;
		}
	} else if(token.code == FLOAT_VALUE || token.code == CHAR_VALUE || token.code == INT_VALUE) {
		strcpy(temp1.lexeme, token.lexeme);
		temp1.code = token.code;
		token = scanner(file);
		return temp1;
	} else {
		token.error = TWENTY_NINTH;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
}

// CHECA OS TERMOS

Token term(FILE * file) { 
	//<termo> ::= <termo> "*" <fator> | <termo> �/� <fator> | <fator>
	Boolean isConvertion = FALSE;
	Token temp1, temp2; temp1.code = temp2.code = INIT;
	temp1 = factor(file);
	while(token.code == DIVISION || token.code == MULTIPLICATION) {
		++temp; printf("T%d =", temp); temp1.isTemp = TRUE;
		printf("%s", temp1.lexeme);
		if(token.code == DIVISION) { isDiv = TRUE; printf(" / "); }
		else if(token.code == MULTIPLICATION) { isDiv = FALSE; printf(" * "); }
		token = scanner(file);
		temp2 = factor(file);
		if(temp2.code == CHAR_VALUE || temp2.code == INT_VALUE || temp2.code == FLOAT_VALUE) {
			if(temp1.code == INT_VALUE && temp2.code == INT_VALUE && isDiv == TRUE) {
				temp1.code = FLOAT_VALUE;
				++temp; printf("\nT%d = (float) %s\n", temp, temp1.lexeme); isConvertion = TRUE;
			}
			else if(temp1.code == INT_VALUE && temp2.code == FLOAT_VALUE) {
				temp1.code = FLOAT_VALUE;
				++temp; printf("\nT%d = (float) %s\n", temp, temp1.lexeme); isConvertion = TRUE;
			}
			else if(temp1.code == CHAR_VALUE && temp2.code != CHAR_VALUE) {
				token.error = THIRTY_SECOND;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			} else if(temp1.code == INT_VALUE && temp2.code != INT_VALUE) {
				token.error = THIRTY_THIRD;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			} else if(temp1.code == FLOAT_VALUE && temp2.code == CHAR_VALUE) {
				token.error = THIRTY_FOURTH;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		}
		if(isConvertion == TRUE) { printf("T%d", temp); }
		else { printf("%s\n", temp2.lexeme); }
	} 
	return temp1;
}

// CHECA AS EXPRESSOES ARITMETICAS

Token arithmeticExpression(FILE * file) {
	//<expr_arit> ::= <expr_arit> "+" <termo>   | <expr_arit> "-" <termo> | <termo>
	Token temp1, temp2; temp1.code = temp2.code = INIT;
	temp1 = term(file);
	temp2 = arithmeticExpressionL(file);
	if(temp2.code == CHAR_VALUE || temp2.code == INT_VALUE || temp2.code == FLOAT_VALUE) {
		if(temp1.code == INT_VALUE && temp2.code == FLOAT_VALUE) {
			temp1.code = FLOAT_VALUE; temp1.isTemp == TRUE;
			++temp; printf("\nT%d = (float) %s\n", temp, temp1.lexeme);
		}
		else if(temp1.code == CHAR_VALUE && temp2.code != CHAR_VALUE) {
			token.error = THIRTY_SECOND;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else if(temp1.code == INT_VALUE && temp2.code != INT_VALUE) {
			token.error = THIRTY_THIRD;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else if(temp1.code == FLOAT_VALUE && temp2.code == CHAR_VALUE) {
			token.error = THIRTY_FOURTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	}	
	return temp1;
}

Token arithmeticExpressionL(FILE * file)  {
	Boolean isConvertion = FALSE;
	int cd = INIT;
	Token temp1, temp2; temp1.code = temp2.code = INIT; temp2.isTemp = FALSE;
	if(token.code == ADDITION || token.code == SUBTRACTION) {
		cd = token.code;
		token = scanner(file);
		temp1 = term(file);
		temp2 = arithmeticExpressionL(file);
	}
	if(temp2.code == CHAR_VALUE || temp2.code == INT_VALUE || temp2.code == FLOAT_VALUE) {
		if(temp1.code == INT_VALUE && temp2.code == FLOAT_VALUE) {
			temp1.code = FLOAT_VALUE;
			++temp; printf("\nT%d = (float) %s\n", temp, temp1.lexeme); isConvertion = TRUE;
		}
		else if(temp1.code == CHAR_VALUE && temp2.code != CHAR_VALUE) {
			token.error = THIRTY_SECOND;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else if(temp1.code == INT_VALUE && temp2.code != INT_VALUE) {
			token.error = THIRTY_THIRD;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		} else if(temp1.code == FLOAT_VALUE && temp2.code == CHAR_VALUE) {
			token.error = THIRTY_FOURTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	}	
	if(cd == ADDITION) { printf(" + "); }
	else if(cd == SUBTRACTION) { printf(" - "); }
	if(isConvertion == TRUE || temp2.isTemp == TRUE) { printf("T%d\n", temp); }
	else if(temp2.code == IDENTIFIER || temp2.code ==  CHAR_VALUE || temp2.code == INT_VALUE || temp2.code == FLOAT_VALUE) { printf("%s\n", temp2.lexeme); }
	return temp1;
}

// CHECA AS ATRIBUICOES

void assignment(FILE * file) {
	//<atribui��o> ::= <id> "=" <expr_arit> ";"
	Boolean isConvertion = FALSE;
	Token temp1, temp2; temp1.code = temp2.code = INIT;
	temp1 = catchVariable(token.lexeme, scope);
	if(temp1.code == OPS) {
		token.error = THIRTY_FIRST;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
	else {
		token = scanner(file);
		if(token.code == ASSIGNMENT) {
			token = scanner(file);
			temp2 = arithmeticExpression(file);
			if(token.code == SEMICOLON) {
				if(temp1.code == FLOAT_VALUE && temp2.code == INT_VALUE) {
					temp2.code = FLOAT_VALUE;
					++temp; printf("T%d = (float) %s\n", temp, temp2.lexeme); isConvertion = TRUE;
				}
				else if(temp1.code == CHAR_VALUE && temp2.code != CHAR_VALUE) {
					token.error = THIRTY_SECOND;
					programErrorPrinter();
					exit(EXIT_SUCCESS);
				} else if(temp1.code == INT_VALUE && temp2.code != INT_VALUE) {
					token.error = THIRTY_THIRD;
					programErrorPrinter();
					exit(EXIT_SUCCESS);
				} else if(temp1.code == FLOAT_VALUE && temp2.code == CHAR_VALUE) {
					token.error = THIRTY_FOURTH;
					programErrorPrinter();
					exit(EXIT_SUCCESS);
				}
				printf("%s = ", temp1.lexeme);
				if(isConvertion == TRUE || temp2.isTemp == TRUE) { printf("T%d\n", temp); }
				else  { printf("%s\n", temp2.lexeme); }
				token = scanner(file);
			} else {
				token.error = FIFTEENTH;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		} else {
			token.error = FOURTEENTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	}
}

// CHECA ORDEM DOS COMANDOS BASICOS

void basicCommands(FILE * file) {
	//<comando_b�sico> ::= <atribui��o> | <bloco>
	if(token.code == IDENTIFIER) {
		assignment(file);
	}
	else {
		block(file);
	}
}

// CHECA ORDEM DAS ITERACOES  

void loop(FILE * file) {
	//<itera��o> ::= while "("<expr_relacional>")" <comando> | do <comando> while "("<expr_relacional>")"";"
	int lbl;
	if(token.code == WHILE) {
		++label; lbl = label;
		++label; printf("L%d: \n", lbl);
		token = scanner(file);
		if(token.code == OPEN_PARENTHESIS) {
			token = scanner(file);
			relationalExpression(file);
			if(token.code == CLOSE_PARENTHESIS) {
				printf("if T%d == 0 goto L%d\n", temp, lbl + 1);
				token = scanner(file);
				commands(file);
				printf("goto L%d\n", lbl);
				printf("L%d: \n", lbl + 1);
			} else {
				token.error = NINETEENTH;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		} else {
			token.error = EIGHTEENTH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	}
	else if(token.code == DO) {
		++label; lbl = label;
		++label; printf("L%d: \n", lbl);
		token = scanner(file);
		commands(file);
		if(token.code == WHILE) {
			token = scanner(file);
			if(token.code == OPEN_PARENTHESIS) {
				token = scanner(file);
				relationalExpression(file);
				if(token.code == CLOSE_PARENTHESIS) {
					token = scanner(file);
					if(token.code == SEMICOLON) {
						printf("if T%d == 0 goto L%d\n", temp, lbl + 1);
						printf("goto L%d\n", lbl);
						printf("L%d: \n", lbl + 1);
						token = scanner(file);
					} else {
						token.error = TWENTY_THIRD;
						programErrorPrinter();
						exit(EXIT_SUCCESS);
					}
				} else {
					token.error = TWENTY_SECOND;
					programErrorPrinter();
					exit(EXIT_SUCCESS);
				}
			} else {
				token.error = TWENTY_FIRST;
				programErrorPrinter();
				exit(EXIT_SUCCESS);
			}
		} else {
			token.error = TWENTIETH;
			programErrorPrinter();
			exit(EXIT_SUCCESS);
		}
	} else {
		token.error = SEVENTEENTH;
		programErrorPrinter();
		exit(EXIT_SUCCESS);
	}
}

// PRINTA ERROS DO PARSER E DO SEMANTICO

void programErrorPrinter() {
	if(token.error == FIFTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou a palavra int: CABECALHO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == SIXTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou a palavra main: CABECALHO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == SEVENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o abre par�nteses: CABECALHO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == EIGHTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o fecha par�nteses: CABECALHO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == NINTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o abre chaves: CABECALHO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o tipo da variavel: DECLARACAO MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_EIGHTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o ponto e virgula: DECLARACAO MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == ELEVENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou fechar o bloco: BLOCO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWELFTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou abrir o par�nteses da condicao do comando: IF MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRTEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou fechar o par�nteses da condicao do comando: IF MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == FOURTEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o sinal do comando basico: ATRIBUICAO MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == FIFTEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o ponto e virgula no final do comando basico: ATRIBUICAO MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == SIXTEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o sinal do comando basico: COMANDO BASICO MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == SEVENTEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o comando da iteracao: ITERACAO MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == EIGHTEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou abrir o par�nteses da condicao da iteracao: WHILE MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == NINETEENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou fechar o par�nteses da condicao da iteracao: WHILE MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTIETH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o while na iteracao: DO-WHILE MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_FIRST) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou abrir o par�nteses da condicao da iteracao: DO-WHILE MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_SECOND) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou fechar o par�nteses da condicao da iteracao: DO-WHILE MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_THIRD) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o ponto e virgula no final da condicao da iteracao: DO-WHILE MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_FOURTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o operador relacional: EXPRESSAO RELACIONAL MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_FIFTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou o valor: EXPRESSAO ARITMETICA MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_SIXTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou fechar o parenteses: EXPRESSAO ARITMETICA MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_SEVENTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - faltou fechar o parenteses: EXPRESSAO ARITMETICA MAL FORMADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == TWENTY_NINTH) {
		printf("ERRO NO PARSER - linha %d, coluna %d, ultimo token lido %s - FATOR MAL FORMADO\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRTIETH) {
		printf("ERRO NO SEMANTICO - linha %d, coluna %d, ultimo token lido %s - VARIAVEL JA DECLARADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRTY_FIRST) {
		printf("ERRO NO SEMANTICO - linha %d, coluna %d, ultimo token lido %s - VARIAVEL NAO DECLARADA\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRTY_SECOND) {
		printf("ERRO NO SEMANTICO - linha %d, coluna %d, ultimo token lido %s - CHAR SO SE RELACIONA COM CHAR: TIPOS INCOMPATIVEIS\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRTY_THIRD) {
		printf("ERRO NO SEMANTICO - linha %d, coluna %d, ultimo token lido %s - INT SO SE RELACIONA COM INT: TIPOS INCOMPATIVEIS\n", token.line, token.column, token.lexeme);
	} else if(token.error == THIRTY_FOURTH) {
		printf("ERRO NO SEMANTICO - linha %d, coluna %d, ultimo token lido %s - FLOAT NAO SE RELACIONA COM CHAR: TIPOS INCOMPATIVEIS\n", token.line, token.column, token.lexeme);
	}
}
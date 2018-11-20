#include "parser.h"
#include "types.h"

int main(int argc, char ** argv) {
    FILE * file;
    if((file = (fopen(argv[1], "r"))) == NULL) {
        printf("\nERRO na abertura do arquivo!\n");
		exit(EXIT_FAILURE);
    } else {
		line = 1; column = 1;
		program(file);
		if(feof(file)) {
			printf("\n\n[FIM DE ARQUIVO]\n");
		}
		if(fclose(file) != 0) {
			printf("\nERRO no fechamento do arquivo!\n");
			exit(EXIT_FAILURE);
		}
    }
}   
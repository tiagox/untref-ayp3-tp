#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1

void print_help() {
	printf("Generador de datos en formato JSON.\n\n");
	printf("uso: untref-ayp3-tp [-f <nombre_archivo>]\n\n");
}

int main(int argc, char **argv) {

	FILE* output_file = stdout;

	// Primera entrega 2018-05-05
	{
		if (argc > 1) {
			if (argc == 3 && strcmp(argv[1], "-f") == 0
					&& strlen(argv[2]) > 0) {
				output_file = fopen(argv[2], "w");

				if (!output_file) {
					printf(
							"Ocurrió un error al tratar de crear o acceder al archivo especificado.\n");
					return ERROR;
				}
			} else {
				print_help();
				return ERROR;
			}
		}

		fprintf(output_file, "{}\n");

		fclose(output_file);
	}

	return SUCCESS;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nJson.h"

#define SUCCESS 0
#define ERROR 1

void print_help() {
	printf("Generador de datos en formato JSON.\n\n");
	printf("uso: untref-ayp3-tp [-f <nombre_archivo>]\n\n");
}

int main(int argc, char **argv) {

	FILE* output_file = stdout;

	nJson root;
	njson_init(&root);

	// Primera entrega 2018-05-05
	{
		// Se leen los parametros provistos al programa.
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
	}

	// Segunda entrega 2018-05-19
	{
		// Se definen los distintos nodos que formarán el JSON.
		nJson size_attribute;
		char* size_value = "0 bytes";
		njson_init(&size_attribute);
		njson_set_value(&size_attribute, "size", size_value,
				strlen(size_value) + 1, write_string);

		nJson hash_attribute;
		char* hash_value = "37eb1ba1849d4b0fb0b28caf7ef3af52";
		njson_init(&hash_attribute);
		njson_set_value(&hash_attribute, "hash", hash_value,
				strlen(hash_value) + 1, write_string);

		nJson bytes_attribute;
		int bytes_value = 0;
		njson_init(&bytes_attribute);
		njson_set_value(&bytes_attribute, "bytes", &bytes_value, sizeof(int),
				&write_int);

		nJson thumb_exists_attribute;
		boolean thumb_exists_value = FALSE;
		njson_init(&thumb_exists_attribute);
		njson_set_value(&thumb_exists_attribute, "thumb_exists",
				&thumb_exists_value, sizeof(boolean), &write_boolean);

		nJson rev_attribute;
		char* rev_value = "714f029684fe";
		njson_init(&rev_attribute);
		njson_set_value(&rev_attribute, "rev", rev_value, strlen(rev_value) + 1,
				&write_string);

		nJson modified_attribute;
		char* modified_value = "Wed, 27 Apr 2011 22:18:51 +0000";
		njson_init(&modified_attribute);
		njson_set_value(&modified_attribute, "rev", modified_value,
				strlen(modified_value) + 1, &write_string);

		nJson path_attribute;
		char* path_value = "/Photos";
		njson_init(&path_attribute);
		njson_set_value(&path_attribute, "path", path_value,
				strlen(path_value) + 1, &write_string);

		nJson is_dir_attribute;
		boolean is_dir_value = TRUE;
		njson_init(&is_dir_attribute);
		njson_set_value(&is_dir_attribute, "is_dir", &is_dir_value,
				sizeof(boolean), &write_boolean);

		nJson icon_attribute;
		char* icon_value = "folder";
		njson_init(&icon_attribute);
		njson_set_value(&icon_attribute, "icon", icon_value,
				strlen(icon_value) + 1, &write_string);

		nJson root_attribute;
		char* root_value = "dropbox";
		njson_init(&root_attribute);
		njson_set_value(&root_attribute, "root", root_value,
				strlen(root_value) + 1, &write_string);

		nJson revision_attribute;
		int revision_value = 29007;
		njson_init(&revision_attribute);
		njson_set_value(&revision_attribute, "revision", &revision_value,
				sizeof(int), &write_int);

		nJson coordinates_lat_attribute;
		double coordinates_lat_value = 37.77256666666666;
		njson_init(&coordinates_lat_attribute);
		njson_set_value(&coordinates_lat_attribute, "lat",
				&coordinates_lat_value, sizeof(double), &write_double);

		nJson coordinates_long_attribute;
		double coordinates_long_value = -122.45934166666667;
		njson_init(&coordinates_long_attribute);
		njson_set_value(&coordinates_long_attribute, "long",
				&coordinates_long_value, sizeof(double), &write_double);

		nJson coordinates_object;
		njson_init(&coordinates_object);
		njson_add_attribute(&coordinates_object, &coordinates_lat_attribute,
				sizeof(coordinates_lat_attribute));
		njson_add_attribute(&coordinates_object, &coordinates_long_attribute,
				sizeof(coordinates_long_attribute));

		nJson coordinate_attribute;
		njson_init(&coordinate_attribute);
		njson_set_value(&coordinate_attribute, "coordinate",
				&coordinates_object,
				sizeof(coordinates_object), &write_njson);

		// Se combinan los diferentes nodos sobre el nodo raiz.
		njson_add_attribute(&root, &size_attribute, sizeof(size_attribute));
		njson_add_attribute(&root, &hash_attribute, sizeof(hash_attribute));
		njson_add_attribute(&root, &bytes_attribute, sizeof(bytes_attribute));
		njson_add_attribute(&root, &thumb_exists_attribute,
				sizeof(bytes_attribute));
		njson_add_attribute(&root, &rev_attribute, sizeof(rev_attribute));
		njson_add_attribute(&root, &modified_attribute,
				sizeof(modified_attribute));
		njson_add_attribute(&root, &path_attribute, sizeof(path_attribute));
		njson_add_attribute(&root, &is_dir_attribute, sizeof(is_dir_attribute));
		njson_add_attribute(&root, &coordinate_attribute,
				sizeof(coordinate_attribute));
		njson_add_attribute(&root, &icon_attribute, sizeof(icon_attribute));
		njson_add_attribute(&root, &root_attribute, sizeof(root_attribute));
		njson_add_attribute(&root, &revision_attribute,
				sizeof(revision_attribute));
	}

	njson_write(&root, output_file);
	njson_release(&root);

	// Se asegura un último salto de línea en la escritura del archivo.
	fprintf(output_file, "\n");
	fclose(output_file);

	return SUCCESS;
}

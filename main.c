#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nJson.h"

/**
 * Códigos de error que retornará la aplicación.
 */
enum errors {
	E_SUCCESS = 0, E_WRONG_ARGS, E_FILE_ACCESS
};

/**
 * Imprime un texto con ejemplos de uso y la descripción de cada parámetro.
 */
void print_help() {
	printf("Generador de datos en formato JSON.\n\n");
	printf("\n");
	printf("uso: untref-ayp3-tp [-c|-f <nombre_archivo>]\n");
	printf("\n");
	printf("  -c                    muestra el JSON generado por la consola.\n");
	printf("  -f <nombre_archivo>   escribe el JSON generado en el archivo <nombre_archivo>\n");
	printf("\n");
}

int main(int argc, char **argv) {

	FILE* output_file;

	// Primera entrega 2018-05-05
	{
		if (argc == 2 && strcmp(argv[1], "-c") == 0) {
			output_file = stdout;
		} else if (argc == 3 && strcmp(argv[1], "-f") == 0
				&& strlen(argv[2]) > 0) {
			output_file = fopen(argv[2], "w");
			if (!output_file) {
				fprintf(stderr, "Ocurrió un error al tratar de crear o acceder al archivo especificado.\n");
				return E_FILE_ACCESS;
			}
		} else {
			print_help();
			return E_WRONG_ARGS;
		}
	}

	nJson root;
	njson_init(&root);

	// Segunda entrega 2018-05-19
	{
		nJson size_attr;
		char* size_value = "0 bytes";
		njson_init(&size_attr);
		njson_set_value(&size_attr, "size", size_value, strlen(size_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &size_attr);
		njson_release(&size_attr);

		nJson hash_attr;
		char* hash_value = "37eb1ba1849d4b0fb0b28caf7ef3af52";
		njson_init(&hash_attr);
		njson_set_value(&hash_attr, "hash", hash_value, strlen(hash_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &hash_attr);
		njson_release(&hash_attr);

		nJson bytes_attr;
		int bytes_value = 0;
		njson_init(&bytes_attr);
		njson_set_value(&bytes_attr, "bytes", &bytes_value, sizeof(int), FALSE, &write_int);
		njson_add_element(&root, &bytes_attr);
		njson_release(&bytes_attr);

		nJson thumb_exists_attr;
		boolean thumb_exists_value = FALSE;
		njson_init(&thumb_exists_attr);
		njson_set_value(&thumb_exists_attr, "thumb_exists", &thumb_exists_value, sizeof(boolean), FALSE,
				&write_boolean);
		njson_add_element(&root, &thumb_exists_attr);
		njson_release(&thumb_exists_attr);

		nJson rev_attr;
		char* rev_value = "714f029684fe";
		njson_init(&rev_attr);
		njson_set_value(&rev_attr, "rev", rev_value, strlen(rev_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &rev_attr);
		njson_release(&rev_attr);

		nJson to_remove_attr;
		char* to_remove_value = "this attribute will be removed";
		njson_init(&to_remove_attr);
		njson_set_value(&to_remove_attr, "to_remove", to_remove_value, strlen(to_remove_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &to_remove_attr);
		njson_release(&to_remove_attr);

		nJson modified_attr;
		char* modified_value = "Wed, 27 Apr 2011 22:18:51 +0000";
		njson_init(&modified_attr);
		njson_set_value(&modified_attr, "modified", modified_value, strlen(modified_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &modified_attr);
		njson_release(&modified_attr);

		nJson path_attr;
		char* path_value = "/Photos";
		njson_init(&path_attr);
		njson_set_value(&path_attr, "path", path_value, strlen(path_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &path_attr);
		njson_release(&path_attr);

		nJson is_dir_attr;
		boolean is_dir_value = TRUE;
		njson_init(&is_dir_attr);
		njson_set_value(&is_dir_attr, "is_dir", &is_dir_value, sizeof(boolean), FALSE, &write_boolean);
		njson_add_element(&root, &is_dir_attr);
		njson_release(&is_dir_attr);

		nJson icon_attr;
		char* icon_value = "folder";
		njson_init(&icon_attr);
		njson_set_value(&icon_attr, "icon", icon_value, strlen(icon_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &icon_attr);
		njson_release(&icon_attr);

		nJson root_attr;
		char* root_value = "dropbox";
		njson_init(&root_attr);
		njson_set_value(&root_attr, "root", root_value, strlen(root_value) + 1, FALSE, &write_string);
		njson_add_element(&root, &root_attr);
		njson_release(&root_attr);

		nJson revision_attr;
		int revision_value = 29007;
		njson_init(&revision_attr);
		njson_set_value(&revision_attr, "revision", &revision_value, sizeof(int), FALSE, &write_int);
		njson_add_element(&root, &revision_attr);
		njson_release(&revision_attr);
	}

	// Tercera entrega 2018-06-09
	{
		nJson contents_array;
		njson_init(&contents_array);
		njson_set_value(&contents_array, "contents", 0x0, 0, TRUE, &write_njson);

		nJson contents_array_0;
		njson_init(&contents_array_0);
		njson_set_value(&contents_array_0, 0x0, 0x0, 0, FALSE, &write_njson);

		nJson size_attr;
		char* size_value = "2.3 MB";
		njson_init(&size_attr);
		njson_set_value(&size_attr, "size", size_value, strlen(size_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &size_attr);
		njson_release(&size_attr);

		nJson rev_attr;
		char* rev_value = "38af1b183490";
		njson_init(&rev_attr);
		njson_set_value(&rev_attr, "revision", rev_value, strlen(rev_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &rev_attr);
		njson_release(&rev_attr);

		nJson thumb_exists_attr;
		boolean thumb_exists_value = TRUE;
		njson_init(&thumb_exists_attr);
		njson_set_value(&thumb_exists_attr, "thumb_exists", &thumb_exists_value, sizeof(boolean), FALSE,
				&write_boolean);
		njson_add_element(&contents_array_0, &thumb_exists_attr);
		njson_release(&thumb_exists_attr);

		nJson bytes_attr;
		int bytes_value = 2453963;
		njson_init(&bytes_attr);
		njson_set_value(&bytes_attr, "bytes", &bytes_value, sizeof(int), FALSE, &write_int);
		njson_add_element(&contents_array_0, &bytes_attr);
		njson_release(&bytes_attr);

		nJson modified_attr;
		char* modified_value = "Mon, 07 Apr 2014 23:13:16 +0000";
		njson_init(&modified_attr);
		njson_set_value(&modified_attr, "modified", modified_value, strlen(modified_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &modified_attr);
		njson_release(&modified_attr);

		nJson client_mtime_attr;
		char* client_mtime_value = "Thu, 29 Aug 2013 01:12:02 +0000";
		njson_init(&client_mtime_attr);
		njson_set_value(&client_mtime_attr, "client_mtime", client_mtime_value, strlen(client_mtime_value) + 1, FALSE,
				&write_string);
		njson_add_element(&contents_array_0, &client_mtime_attr);
		njson_release(&client_mtime_attr);

		nJson path_attr;
		char* path_value = "/Photos/flower.jpg";
		njson_init(&path_attr);
		njson_set_value(&path_attr, "path", path_value, strlen(path_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &path_attr);
		njson_release(&path_attr);

		nJson to_remove_attr;
		char* to_remove_value = "this attribute will be removed";
		njson_init(&to_remove_attr);
		njson_set_value(&to_remove_attr, "to_remove", to_remove_value, strlen(to_remove_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &to_remove_attr);
		njson_release(&to_remove_attr);

		nJson photo_info_attr;
		njson_init(&photo_info_attr);
		njson_set_value(&photo_info_attr, "photo_info", 0x0, 0, FALSE, &write_njson);

		nJson lat_long_array;
		njson_init(&lat_long_array);
		njson_set_value(&lat_long_array, "lat_long", 0x0, 0, TRUE, &write_njson);

		nJson lat_long_array_0;
		double lat_long_array_0_value = 37.77256666666666;
		njson_init(&lat_long_array_0);
		njson_set_value(&lat_long_array_0, 0x0, &lat_long_array_0_value, sizeof(double), FALSE, &write_double);
		njson_add_element(&lat_long_array, &lat_long_array_0);
		njson_release(&lat_long_array_0);

		nJson lat_long_array_1;
		double lat_long_array_1_value = -122.45934166666667;
		njson_init(&lat_long_array_1);
		njson_set_value(&lat_long_array_1, 0x0, &lat_long_array_1_value, sizeof(double), FALSE, &write_double);
		njson_add_element(&lat_long_array, &lat_long_array_1);
		njson_release(&lat_long_array_1);

		njson_add_element(&photo_info_attr, &lat_long_array);
		njson_release(&lat_long_array);

		nJson time_taken_attr;
		char* time_taken_value = "Wed, 28 Aug 2013 18:12:02 +0000";
		njson_init(&time_taken_attr);
		njson_set_value(&time_taken_attr, "time_taken", time_taken_value, strlen(time_taken_value) + 1, FALSE,
				&write_string);
		njson_add_element(&photo_info_attr, &time_taken_attr);
		njson_release(&time_taken_attr);

		njson_add_element(&contents_array_0, &photo_info_attr);
		njson_release(&photo_info_attr);

		nJson is_dir_attr;
		boolean is_dir_value = FALSE;
		njson_init(&is_dir_attr);
		njson_set_value(&is_dir_attr, "is_dir", &is_dir_value, sizeof(boolean), FALSE, &write_boolean);
		njson_add_element(&contents_array_0, &is_dir_attr);
		njson_release(&is_dir_attr);

		nJson icon_attr;
		char* icon_value = "page_white_picture";
		njson_init(&icon_attr);
		njson_set_value(&icon_attr, "icon", icon_value, strlen(icon_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &icon_attr);
		njson_release(&icon_attr);

		nJson root_attr;
		char* root_value = "dropbox";
		njson_init(&root_attr);
		njson_set_value(&root_attr, "root", root_value, strlen(root_value) + 1, FALSE, &write_string);
		njson_add_element(&contents_array_0, &root_attr);
		njson_release(&root_attr);

		nJson mime_type_attr;
		char* mime_type_value = "image/jpeg";
		njson_init(&mime_type_attr);
		njson_set_value(&mime_type_attr, "mime_type", mime_type_value, strlen(mime_type_value) + 1, FALSE,
				&write_string);
		njson_add_element(&contents_array_0, &mime_type_attr);
		njson_release(&mime_type_attr);

		nJson contents_revision_attr;
		int contents_revision_value = 14511;
		njson_init(&contents_revision_attr);
		njson_set_value(&contents_revision_attr, "revision", &contents_revision_value, sizeof(int), FALSE, &write_int);
		njson_add_element(&contents_array_0, &contents_revision_attr);
		njson_release(&contents_revision_attr);

		njson_add_element(&contents_array, &contents_array_0);
		njson_release(&contents_array_0);

		njson_add_element(&root, &contents_array);
		njson_release(&contents_array);

		nJson revision_attr;
		int revision_value = 29007;
		njson_init(&revision_attr);
		njson_set_value(&revision_attr, "revision", &revision_value, sizeof(int), FALSE, &write_int);
		njson_add_element(&root, &revision_attr);
		njson_release(&revision_attr);
	}

#if 0

	// Prueba de eliminación y edición de atributos.
	{
		// Se elimina un atributo de primer nivel.
		njson_remove_element(&root, "to_remove");

		// Obtengo el nodo llamado `contents`.
		nJson* contents = 0x0;
		njson_get_element(&root, "contents", &contents);

		// Se remueve un atriburo del objeto `contents`.
		nJson* contents_0 = 0x0;
		njson_get_array_element(contents, 0, &contents_0);
		njson_remove_element(contents_0, "to_remove");

		// Se actualiza un atributo del objeto `contents`.
		nJson new_root_attr;
		char* new_root_value = "~/home/$USER/dropbox";
		njson_init(&new_root_attr);
		njson_set_value(&new_root_attr, "root", new_root_value, strlen(new_root_value) + 1, FALSE, &write_string);
		njson_update_element(contents_0, "root", &new_root_attr);
		njson_release(&new_root_attr);

		// Se actualiza el segundo elemento del array `lat_long`.
		nJson* photo_info = 0x0;
		njson_get_element(contents_0, "photo_info", &photo_info);

		nJson* lat_long = 0x0;
		njson_get_element(photo_info, "lat_long", &lat_long);

		nJson new_lat_long_1;
		double new_lat_long_1_value = 89.12038712307282;
		njson_init(&new_lat_long_1);
		njson_set_value(&new_lat_long_1, 0x0, &new_lat_long_1_value, sizeof(double), FALSE, &write_double);
		njson_update_array_element(lat_long, 1, &new_lat_long_1);
		njson_release(&new_lat_long_1);
	}

#endif

	njson_write(&root, output_file);

	// Se asegura un último salto de línea en la escritura del archivo.
	fprintf(output_file, "\n");

	fclose(output_file);

	njson_release(&root);

	return E_SUCCESS;
}

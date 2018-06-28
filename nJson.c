#include "nJson.h"

nJson* njson_init(nJson* this) {
	this->name = 0x0;
	this->value_size = 0;
	this->value = 0x0;
	this->is_array = FALSE;
	this->element_count = 0;
	this->elements = 0x0;
	this->children = 0x0; // deprecated
	this->next = 0x0; // deprecated
	this->write = 0x0;

	return this;
}

nJson* njson_set_value(nJson* this, const char* name, void* value,
		unsigned value_size, unsigned elementos, writer write) {
	this->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
	strcpy(this->name, name);

	this->value = malloc(value_size);
	memcpy(this->value, value, value_size);

	this->value_size = value_size;

	this->cant_elementos = elementos;

	this->write = write;

	if (this->children) {
		njson_release(this->children);
	}
	return this;
}
/*-----------------------------------------------
 * Realizo modificaciones:
 * comento la ejecucion de limpieza de memoria del final.
 * me borra los nombres de los Json internos(contents y photo_info).
 */
nJson* njson_add_attr(nJson* this, nJson* attribute, unsigned attribute_size) {
	if (this->children) {
		nJson* current = this->children;
		while (current->next != 0x0) {
			current = current->next;
		}
		current->next = (nJson*) malloc(attribute_size);
		memcpy(current->next, attribute, attribute_size);
	} else {
		this->children = (nJson*) malloc(attribute_size);
		memcpy(this->children, attribute, attribute_size);
	}
	//this->cant_elementos++;
	/*
	 free(this->name);
	 this->name = 0x0;
	 free(this->value);
	 this->value = 0x0;
	 this->value_size = 0x0;
	 */
	return this;
}

nJson* njson_write(nJson* this, FILE* output) {
	int i;

	if (this->name) {
		fprintf(output, "\"%s\":", this->name);
	}

	if (this->value_size) {
		//Si cant_elementos es mayor a 1 es un array
		if (this->cant_elementos > 1) {
			fprintf(output, "[");
		}
		//Trata a todos valores como un arrays, de un elemento hasta la cant_elemmentos
		for (i = 0; i < this->cant_elementos; i = i + 1) {
			this->write(output, this->value, i);
			// Imprime "," que se imprimia en el ultimo if
			// fprintf(output, ",");
		}
		if (this->cant_elementos > 1) {
			fprintf(output, "]");
		}
	}

	if (this->children) {
		//agrega corchetes a los array de nJson
		if (this->cant_elementos > 1) {
			fprintf(output, "[");
		}

		fprintf(output, "{");
		njson_write(this->children, output);
		fprintf(output, "}");

		if (this->cant_elementos > 1) {
			fprintf(output, "]");
		}
	}

	if (this->next) {
		fprintf(output, ",");
		njson_write(this->next, output);
	}

	return this;
}

void njson_release(nJson* this) {
	free(this->name);
	this->name = 0x0;

	free(this->value);
	this->value = 0x0;
	this->value_size = 0x0;
	this->cant_elementos = 0x0;
	this->write = 0x0;

	if (this->next) {
		njson_release(this->next);
		this->next = 0x0;
	}

	if (this->children) {
		njson_release(this->children);
		this->children = 0x0;
	}
}

/* -------------------------------------------------------
 * Funciones necesarias para las consignas de la entrega 3
 * -------------------------------------------------------
 */

err_code buscar_njson(nJson* this, nJson* buscado, nJson* encontrado) {
	if (this->children) {
		nJson* current = this->children;
		while (current->next != 0x0
				&& (strcmp(current->next->name, buscado->name) != 0)) {
			if (this->children->children) {
				buscar_njson(this->children, buscado, encontrado);
			}
			current = current->next;
		}
		encontrado = current->next;
	}
	return OK;
}

err_code eliminar_njson(nJson* this, nJson* a_modificar, char* name_atributo) {
	nJson* encontrado = 0X0;
	nJson* padre;
	if (strcmp(this->name, a_modificar->name) != 0) {
		encontrado = buscar_njson(this, a_modificar, encontrado);

	} else {
		encontrado = this;
	}

	if (encontrado->children) {

		nJson* current = encontrado->children;

		while (current->next != 0x0
				&& (strcmp(current->name, name_atributo) != 0)) {
			padre = current;
			current = current->next;
		}
		padre->next = padre->next->next;

		//njson_release(current->next);

		//current->next=NULL;
		current->next = padre->next->next;

	}
	return OK;
}

nJson* modificar_njson(nJson* this, nJson* a_modificar, char* name_atribute,
		void* new_value, unsigned long_new_value) {

	nJson* encontrado = this;

	if (strcmp(this->name, a_modificar->name) != 0) {
		encontrado = buscar_njson(this, a_modificar, encontrado);
	}

	if (encontrado->children) {
		nJson* current = encontrado->children;
		while ((strcmp(current->name, name_atribute) != 0)
				&& current->next != 0x0) {
			current = current->next;
		}

		memcpy(current->value, new_value, long_new_value);
	} else {
		printf("�� No existe atributo \" %s \" !! \n", name_atribute);
	}

	return this;
}

/*******************************************************************************
 * Funciones auxiliares para escribir los tipos de datos especificos.
 * Se agrega a todas el parametro unsigned cantidad.
 ******************************************************************************/

void write_njson(FILE* output, void* value, unsigned cantidad) {
	//En este caso no se trata al valor como un array
	//ya que la funcion itera sobre si misma, por lo cual
	//no se suma el desplazamiento cantidad al puntero.
	//El parametro se recibe por una cuestion de compatibilidad
	//con la funcion generalizada.
	njson_write((nJson*) value, output);
}

void write_string(FILE* output, void* value, unsigned cantidad) {
	//Igual que  la anterior, no se utiliza el desplamiento
	//debido a que, la funcion, trata al valor como un string
	//y no como una cadena de caracteres.
	//El parametro se recibe por una cuestion de compatibilidad
	//con la funcion generalizada.
	fprintf(output, "\"%s\"", (char*) value);
}

void write_int(FILE* output, void* value, unsigned cantidad) {
	fprintf(output, "%d", *((int*) value + cantidad));
}

void write_boolean(FILE* output, void* value, unsigned cantidad) {
	fprintf(output, "%s", (*((boolean*) value + cantidad)) ? "true" : "false");
}

void write_double(FILE* output, void* value, unsigned cantidad) {
	fprintf(output, "%f", *((double*) value + cantidad));
}

void write_float(FILE* output, void* value, unsigned cantidad) {
	fprintf(output, "%f", *((float*) value + cantidad));
}


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

err_code njson_set_value(nJson* this, const char* name, void* value, unsigned value_size, boolean is_array,
		writer write) {
	if (value_size && is_array) {
		return E_INVALID_ARGS_COMBINATION;
	}

	if (name) {
		this->name = (char*) realloc(this->name, (strlen(name) + 1) * sizeof(char));
		strcpy(this->name, name);
	}

	if (value_size) {
		this->value_size = value_size;
		this->value = realloc(this->value, this->value_size);
		memcpy(this->value, value, this->value_size);
	}

	this->write = write;

	this->is_array = is_array;

	// Si el nodo tiene valor, significa que representa un atributo (clave:valor) y que no puede contener elementos.
	if (this->element_count) {
		for (int i = 0; i < this->element_count; ++i) {
			njson_release(this->elements[i]);

			free(this->elements[i]);
			this->elements[i] = 0x0;
		}

		free(this->elements);
		this->elements = 0x0;

		this->element_count = 0;
	}

	return E_OK;
}

nJson* njson_clone(nJson* this, nJson* target) {
	njson_init(target);

	if (this->name) {
		target->name = (char*) malloc((strlen(this->name) + 1) * sizeof(char));
		strcpy(target->name, this->name);
	}

	if (this->value_size) {
		target->value_size = this->value_size;
		target->value = malloc(target->value_size);
		memcpy(target->value, this->value, target->value_size);
	}

	target->write = this->write;

	target->is_array = this->is_array;

	if (this->element_count) {
		target->element_count = this->element_count;
		target->elements = (nJson**) malloc(this->element_count * sizeof(nJson*));
		for (int i = 0; i < target->element_count; ++i) {
			target->elements[i] = (nJson*) malloc(sizeof(nJson));
			njson_clone(this->elements[i], target->elements[i]);
		}
	}

	return this;
}

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

nJson* buscar_njson(nJson* this, nJson* buscado, nJson* encontrado) {
	if (this->children) {
		nJson* current = this->children;
		while (current->next != 0x0 && (strcmp(current->next->name, buscado->name) != 0)) {
			if (this->children->children) {
				buscar_njson(this->children, buscado, encontrado);
			}
			current = current->next;
		}
		encontrado = current->next;
	}
	return this;
}

nJson* eliminar_njson(nJson* this, nJson* a_modificar, char* name_atributo) {
	nJson* encontrado = this;
	nJson* padre = 0x0;

	if (strcmp(this->name, a_modificar->name) != 0) {
		buscar_njson(this, a_modificar, encontrado);
	}

	if (encontrado->children) {
		nJson* current = encontrado->children;

		while (current->next != 0x0 && (strcmp(current->name, name_atributo) != 0)) {
			padre = current;
			current = current->next;
		}
		padre->next = padre->next->next;

		//njson_release(current->next);

		//current->next=NULL;
		current->next = padre->next->next;

	}
	return this;
}

nJson* modificar_njson(nJson* this, nJson* a_modificar, char* name_atribute, void* new_value, unsigned long_new_value) {
	nJson* encontrado = this;

	if (strcmp(this->name, a_modificar->name) != 0) {
		buscar_njson(this, a_modificar, encontrado);
	}

	if (encontrado->children) {
		nJson* current = encontrado->children;
		while ((strcmp(current->name, name_atribute) != 0) && current->next != 0x0) {
			current = current->next;
		}

		memcpy(current->value, new_value, long_new_value);
	} else {
		printf("No existe atributo \" %s \" !! \n", name_atribute);
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


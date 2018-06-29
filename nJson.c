#include "nJson.h"

nJson* njson_init(nJson* this) {
	this->name = 0x0;
	this->value_size = 0;
	this->value = 0x0;
	this->is_array = FALSE;
	this->element_count = 0;
	this->elements = 0x0;
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

nJson* njson_add_element(nJson* this, nJson* attr) {
	nJson new_attr;
	njson_clone(attr, &new_attr);

	this->element_count += 1;
	this->elements = (nJson**) realloc(this->elements, this->element_count * sizeof(nJson*));
	this->elements[this->element_count - 1] = (nJson*) malloc(sizeof(new_attr));
	memcpy(this->elements[this->element_count - 1], &new_attr, sizeof(new_attr));

	// Si se agregan elementos, este nodo no podrá tener un valor asignado.
	if (this->value) {
		free(this->value);
		this->value = 0x0;
		this->value_size = 0x0;
	}

	return this;
}

nJson* njson_write(nJson* this, FILE* output) {
	if (this->name) {
		fprintf(output, "\"%s\":", this->name);
	}

	if (this->value) {
		this->write(output, this->value);
	}

	if (this->element_count) {
		fprintf(output, this->is_array ? "[" : "{");
		for (int i = 0; i < this->element_count; ++i) {
			fprintf(output, i > 0 ? "," : "");
			njson_write(this->elements[i], output);
		}
		fprintf(output, this->is_array ? "]" : "}");
	}

	return this;
}

void njson_release(nJson* this) {
	if (this->name) {
		free(this->name);
		this->name = 0x0;
	}

	if (this->value_size) {
		free(this->value);
		this->value = 0x0;
		this->value_size = 0;
	}

	this->write = 0x0;

	this->is_array = FALSE;

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
}

/******************************************************************************
 * Funciones auxiliares para imprimir tipos de datos soportados por nJson
 ******************************************************************************/

void write_njson(FILE* output, void* value) {
	njson_write((nJson*) value, output);
}

void write_string(FILE* output, void* value) {
	fprintf(output, "\"%s\"", (char*) value);
}

void write_int(FILE* output, void* value) {
	fprintf(output, "%d", *((int*) value));
}

void write_boolean(FILE* output, void* value) {
	fprintf(output, "%s", (*((boolean*) value)) ? "true" : "false");
}

void write_double(FILE* output, void* value) {
	fprintf(output, "%f", *((double*) value));
}

void write_float(FILE* output, void* value) {
	fprintf(output, "%f", *((float*) value));
}

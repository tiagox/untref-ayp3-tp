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

err_code njson_remove_element(nJson* this, const char* name) {
	if (this->is_array) {
		return E_OPERATION_NOT_ALLOWED;
	}

	boolean found = FALSE;

	for (int i = 0; i < this->element_count; ++i) {
		if (strcmp(this->elements[i]->name, name) == 0) {
			found = TRUE;
			njson_release(this->elements[i]);
			free(this->elements[i]);
		}

		if (found && i < this->element_count - 1) {
			this->elements[i] = this->elements[i + 1];
		}
	}

	if (found) {
		this->element_count -= 1;
		this->elements = (nJson**) realloc(this->elements, this->element_count * sizeof(nJson*));
	}

	return found ? E_OK : E_ELEMENT_NOT_FOUND;
}

err_code njson_get_element(nJson* this, const char* name, nJson** element) {
	if (this->is_array) {
		return E_OPERATION_NOT_ALLOWED;
	}

	for (int i = 0; i < this->element_count; ++i) {
		if (strcmp(this->elements[i]->name, name) == 0) {
			*element = this->elements[i];
			return E_OK;
		}
	}

	return E_ELEMENT_NOT_FOUND;
}

err_code njson_get_array_element(nJson* this, unsigned index, nJson** element) {
	if (!this->is_array) {
		return E_OPERATION_NOT_ALLOWED;
	}

	if (index < this->element_count) {
		*element = this->elements[index];
		return E_OK;
	}

	return E_INDEX_OUT_OF_BOUND;
}

err_code njson_update_element(nJson* this, const char* name, nJson* attr) {
	if (this->is_array) {
		return E_OPERATION_NOT_ALLOWED;
	}

	for (int i = 0; i < this->element_count; ++i) {
		if (strcmp(this->elements[i]->name, name) == 0) {
			nJson new_attr;
			njson_clone(attr, &new_attr);
			njson_release(this->elements[i]);
			memcpy(this->elements[i], &new_attr, sizeof(new_attr));
			return E_OK;
		}
	}

	return E_ELEMENT_NOT_FOUND;
}

err_code njson_update_array_element(nJson* this, unsigned index, nJson* attr) {
	if (!this->is_array) {
		return E_OPERATION_NOT_ALLOWED;
	}

	if (index < this->element_count) {
		nJson new_attr;
		njson_clone(attr, &new_attr);
		njson_release(this->elements[index]);
		free(this->elements[index]);
		this->elements[index] = (nJson*) malloc(sizeof(new_attr));
		memcpy(this->elements[index], &new_attr, sizeof(new_attr));
		return E_OK;
	}

	return E_INDEX_OUT_OF_BOUND;
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

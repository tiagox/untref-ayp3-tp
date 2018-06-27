#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _NJSON_H_
#define _NJSON_H_

/**
 * Defino un nuevo tipo de dato enumerado que represente los valores booleanos
 * de `true` y `false`
 * @type boolean
 */
typedef enum _boolean {
	FALSE, TRUE
} boolean;

typedef enum _err_code {
	OK = 0
} err_code;

/**
 * Defino un nuevo tipo de dato puntero a función para representar el prototipo
 * de las funciones de escritura especificos para cada tipo de dato contenido en
 * un nJson.
 * @type writer
 */
typedef void (*writer)(FILE*, void*, unsigned);

/**
 * Estructura base para los nodos de JSON.
 * @type nJson
 */
typedef struct _nJson {
	/**
	 * Nombre del nodo.
	 */
	char* name;
	/**
	 * Puntero al valor almacenado en el nodo.
	 */
	void* value;
	/**
	 * Tamaño en bytes del valor almacenado en el nodo.
	 */
	unsigned value_size;
	/*
	 * Cantidad de elementos del valor, utilizado para el manejo de arrays.
	 */
	unsigned cant_elementos;
	/**
	 * Puntero a la lista de atributos dentro del nodo. Solo hijos directos.
	 */
	struct _nJson* children;
	/**
	 * Puntero a al siguiente nodo hermano, cuando los nodos son atributos de un
	 * nodo padre.
	 */
	struct _nJson* next;
	/**
	 * Función para escribir el tipo de dato especifico guardado como valor del
	 * nodo.
	 */
	writer write;
} nJson;

/**
 * Inicializa un nJson.
 * @param this nJson sobre el cual operará.
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_init(nJson* this);

/**
 * Define un valor que representa el contenido de un nJson.
 * @param this nJson sobre el cual operará.
 * @param name Nombre que recibirá el nJson cuando sea tratado como atributo.
 * @param value Puntero al data que debe ser almacenado como valor del nJson.
 * @param value_size Tamaño en memoria del valor que se almacenará en el nJson.
 * @param size Tamaño en memoria del nJson `child`
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_set_value(nJson* this, const char* name, void* value,
		unsigned value_size, unsigned elementos, writer write);

/**
 * Agrega un atributo al nJson especificado.
 * @param this nJson sobre el cual operará.
 * @param attribute nJson que será agregado como atributo en el nJson `this`.
 * @param attribute_size Tamaño en memoria del nJson `child`
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_add_attr(nJson* this, nJson* attribute, unsigned attribute_size);

/**
 * Escribe el texto que representa al nJson en el descriptor de archivo provisto
 * a la función.
 * @param this nJson sobre el cual operará.
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_write(nJson* this, FILE* output);

/**
 * Primitiva para liberar la memoria allocada para cada uno de los nodos de la
 * estructura de nJson recursivamente.
 * @param this nJson sobre el cual operará.
 */
void njson_release(nJson* this);

/**
 * Escribe un nJson como texto en el archivo espeficado.
 */
void write_njson(FILE* output, void* value, unsigned cantidad);

/**
 * Escribe una cadena de caracteres como texto en el archivo espeficado.
 */
void write_string(FILE* output, void* value, unsigned cantidad);

/**
 * Escribe un entero como texto en el archivo espeficado.
 */
void write_int(FILE* output, void* value, unsigned cantidad);

/**
 * Escribe un booleano como texto en el archivo espeficado.
 */
void write_boolean(FILE* output, void* value, unsigned cantidad);

/**
 * Escribe un flotante como texto en el archivo espeficado.
 */
void write_float(FILE* output, void* value, unsigned cantidad);

/**
 * Escribe un flotante de doble presición como texto en el archivo espeficado.
 */
void write_double(FILE* output, void* value, unsigned cantidad);

/*
 * Funciones necesarias para las consignas de la entrega 3
 */

/*
 * Funcion que busca un Json padre de atributos.
 * @param. nJson raiz.
 * @param. nJson buscado.
 * @param. nJson para devolver el resultado.
 * @return.  retorna un puntero al nJson encontrado.
 */
err_code buscar_njson(nJson*, nJson*, nJson*);

/*
 * Funcion para modificar alguun valor de un nJson
 * @param. nJson raiz.
 * @param. nJson a modificar.
 * @param. char* name del valor a nodificar.
 * @param. puntero void, valor nuevo.
 * @param. longitud del valor nuevo.
 * @return: retorna nJson raiz modificado.
 */
err_code modificar_njson(nJson*, nJson*, char*, void*, unsigned);

/*
 * Funcion utilizadapara elimiinar un nJson.
 * @param. nJson raiz.
 * @param. nJson padre de un atributo a eliminar.
 * @param. char* name del atributo a eliminar.
 * @return: retorna el nJson raiz modificado.
 */
nJson* eliminar_njson(nJson*, nJson*, char*);

#endif /* _NJSON_H_ */

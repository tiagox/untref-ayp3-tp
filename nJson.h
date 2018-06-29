#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _NJSON_H_
#define _NJSON_H_

/**
 * Defino un nuevo tipo de dato enumerado que represente los valores booleanos `true` y `false`.
 * @type boolean
 */
typedef enum _boolean {
	FALSE = 0, TRUE
} boolean;

/**
 * Dato enumerado para devolver los códigos de errores posibles en el uso del TDA nJson.
 * @type err_code
 */
typedef enum _err_code {
	E_OK = 0, E_INVALID_ARGS_COMBINATION, E_OPERATION_NOT_ALLOWED, E_ELEMENT_NOT_FOUND, E_INDEX_OUT_OF_BOUND
} err_code;

/**
 * Defino un nuevo tipo de dato puntero a función para representar el prototipo de las funciones de escritura
 * especificos para cada tipo de dato contenido en un nJson.
 * @type writer
 */
typedef void (*writer)(FILE*, void*);

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
	/**
	 * Flag para indicar si los elementos son elementos de una array o atributos.
	 */
	boolean is_array;
	/*
	 * Cantidad de elementos del valor, utilizado para el manejo de arrays.
	 */
	unsigned element_count;
	/**
	 * Array de elementos contenidos en el nodo. Pueden ser elementos de una array o atributos del nodo JSON.
	 */
	struct _nJson** elements;
	/**
	 * Función para escribir el tipo de dato especifico guardado como valor del nodo.
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
 * @return err_code Puntero al nJson con el cual se operá.
 */
err_code njson_set_value(nJson* this, const char* name, void* value, unsigned value_size, boolean is_array,
		writer write);

/**
 * Crea una copia de toda superficial del contenido del nodo nJson indicado en el nodo de destino.
 * @param this nJson sobre el cual operará.
 * @param target nJson en donde se realizará la copia del contenido de la origina.
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_clone(nJson* this, nJson* target);

/**
 * Agrega un atributo al nJson especificado.
 * @param this nJson sobre el cual operará.
 * @param element nJson que será agregado como atributo en el nJson `this`.
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_add_element(nJson* this, nJson* element);

/**
 * Elimina un atributo contenido en el nodo.
 * @param this nJson al cual se le eliminará el elemento.
 * @param name Nombre del elemento a eliminar.
 * @return err_code Código de error resultado de la operación.
 */
err_code njson_remove_element(nJson* this, const char* name);

/**
 * Retorna un elemento contenido en un nodo por medio del nombre del atributo.
 * @param this nJson sobre el cual operará.
 * @param name Nombre del elemento a retornar.
 * @param element Puntero a un nodo nJson en el cual se devuelve una copia del elemento encontrado.
 * @return err_code Código de error resultado de la operación.
 */
err_code njson_get_element(nJson* this, const char* name, nJson** element);

/**
 * Retorna un elemento contenido en un array por medio del índice del mismo.
 * @param this nJson sobre el cual operará.
 * @param index Índice del elemento a retornar.
 * @param element Puntero a un nodo nJson en el cual se devuelve una copia del elemento encontrado.
 * @return err_code Código de error resultado de la operación.
 */
err_code njson_get_array_element(nJson* this, unsigned index, nJson** element);

/**
 * Reemplaza un atributo contenido en un nodo por otro por medio del nombre del atributo.
 * @param this nJson sobre el cual operará.
 * @param name Nombre del elemento a reemplazar.
 * @param element nJson que reemplazará al atributo existente.
 * @return err_code Código de error resultado de la operación.
 */
err_code njson_update_element(nJson* this, const char* name, nJson* element);

/**
 * Reemplaza un elemento contenido en un array por otro por medio del íncide del mismo.
 * @param this nJson sobre el cual operará.
 * @param index Índice del elemento a reemplazar.
 * @param element nJson que reemplazará al atributo existente.
 * @return err_code Código de error resultado de la operación.
 */
err_code njson_update_array_element(nJson* this, unsigned index, nJson* element);

/**
 * Escribe el texto que representa al nJson en el descriptor de archivo provisto a la función.
 * @param this nJson sobre el cual operará.
 * @return nJson* Puntero al nJson con el cual se operá.
 */
nJson* njson_write(nJson* this, FILE* output);

/**
 * Primitiva para liberar la memoria allocada para cada uno de los nodos de la estructura de nJson recursivamente.
 * @param this nJson sobre el cual operará.
 */
void njson_release(nJson* this);

/******************************************************************************
 * Funciones auxiliares para imprimir tipos de datos soportados por nJson
 ******************************************************************************/

/**
 * Escribe un nJson como texto en el archivo espeficado.
 */
void write_njson(FILE* output, void* value);

/**
 * Escribe una cadena de caracteres como texto en el archivo espeficado.
 */
void write_string(FILE* output, void* value);

/**
 * Escribe un entero como texto en el archivo espeficado.
 */
void write_int(FILE* output, void* value);

/**
 * Escribe un booleano como texto en el archivo espeficado.
 */
void write_boolean(FILE* output, void* value);

/**
 * Escribe un flotante como texto en el archivo espeficado.
 */
void write_float(FILE* output, void* value);

/**
 * Escribe un flotante de doble presición como texto en el archivo espeficado.
 */
void write_double(FILE* output, void* value);

#endif /* _NJSON_H_ */

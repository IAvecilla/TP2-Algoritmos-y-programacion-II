#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include <stdlib.h>

typedef int (*heap_comparador)(void*, void*);

typedef void (*heap_liberar_elemento)(void*);

typedef struct heap heap_t;

/* 
 * Crea un Heap en memoria, recibe un comparador para los elementos y opcionalmente un destructor
 * para dichos elementos.
 * 
 * En caso de exito devuelve un puntero al heap creado, sino devuelve NULL.
 */
heap_t* heap_crear (heap_comparador comparador, heap_liberar_elemento destructor);

/* 
 * Recibe un elemento y el heap donde se desa insertar.
 * En caso de exito devuelve 0 sino, -1.
 */
int heap_insertar (heap_t* heap, void* elemento);

/* 
 * Recibe un heap y quita su raiz.
 * La funcion devuelve dicha raiz, que ya no permanece dentro del heap.
 * El elemento sigue viviendo en memoria, no es liberado por esta funcion.
 */
void* heap_borrar_raiz (heap_t* heap);

/* 
 * Recibe un Heap y lo elimina de la memoria, liberando el espacio ocupado.
 */
void destruir_heap (heap_t* heap);

/* 
 * Recibe un Heap y devuelve un valor booleano.
 * False en caso de que el heap no este vacio y True, si lo esta.
 */
bool heap_vacio (heap_t* heap);

#endif

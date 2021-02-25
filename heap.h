#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include <stdlib.h>

typedef int (*heap_comparador)(void*, void*);

typedef void (*heap_liberar_elemento)(void*);

typedef struct heap heap_t;

heap_t* heap_crear (heap_comparador comparador, heap_liberar_elemento destructor);

int heap_insertar (heap_t* heap, void* elemento);

void* heap_borrar_raiz (heap_t* heap);

void destruir_heap (heap_t* heap);

bool heap_vacio (heap_t* heap);

#endif

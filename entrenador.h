#ifndef __ENTRENADOR_H__
#define __ENTRENADOR_H__

#include "lista.h"
#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct entrenador {
    char nombre[100];
    lista_t* lista_pokemones;
} entrenador_t;

/* 
 * Recibe un entrenador y libera la memoria ocupada por este.
 */
void destruir_entrenador (void* entrenador);

/* 
 * Aloja un entrenador en memoria y devuelve un puntero a este.
 * En caso de error devuelve NULL.
 */
entrenador_t* crear_entrenador();

#endif
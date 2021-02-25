#ifndef __JUGADOR_PRINCIPAL_H__
#define __JUGADOR_PRINCIPAL_H__

#include "lista.h"
#include "pokemon.h"

typedef struct personaje {
    char nombre[100];
    lista_t* lista_pokemones_obtenidos;
} personaje_t;

/* 
 * Recibe un personaje y lo libera de la memoria
 */
void destructor_personaje (void* personaje);

/* 
 * Crea un personaje en memoria
 */
personaje_t* crear_personaje ();

/* 
 * Despliega todos los pokemon del jugador principal y permite que intercambie
 * pokemon de su party con los de su mochila
 */
void cambiar_party (personaje_t* jugador);

#endif
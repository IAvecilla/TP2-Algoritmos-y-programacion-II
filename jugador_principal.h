#ifndef __JUGADOR_PRINCIPAL_H__
#define __JUGADOR_PRINCIPAL_H__

#include "lista.h"
#include "pokemon.h"

typedef struct personaje {
    char nombre[100];
    lista_t* lista_pokemones_obtenidos;
} personaje_t;

void destructor_personaje (void* personaje);

personaje_t* crear_personaje ();

void cambiar_party (personaje_t* jugador);

#endif
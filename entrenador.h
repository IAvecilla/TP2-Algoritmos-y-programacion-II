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

void destruir_entrenador (void* entrenador);

entrenador_t* crear_entrenador();

#endif
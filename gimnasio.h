#ifndef __GIMNASIO_H__
#define __GIMNASIO_H__

#include "lista.h"
#include "entrenador.h"
#include "pokemon.h"

int comparador_de_gimnasios (void* gimnasio1, void* gimnasio2);

void destruir_gimnasio (void* gimnasio);

typedef struct gimnasio {
    char nombre[100];
    int dificultad;
    int id_funcion;
    char nombre_lider[20];
    lista_t* pila_entrenadores;
} gimnasio_t;

gimnasio_t* gimnasio_crear();

#endif
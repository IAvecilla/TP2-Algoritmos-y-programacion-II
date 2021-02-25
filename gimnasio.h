#ifndef __GIMNASIO_H__
#define __GIMNASIO_H__

#include "lista.h"
#include "entrenador.h"
#include "pokemon.h"

/* 
 * Compara dos gimnasios, y devuelve 1 si el primero es mayor que el segundo
 * -1 si el segundo es mayor que el primero o 0 en caso de que sean iguales.
 */
int comparador_de_gimnasios (void* gimnasio1, void* gimnasio2);

/* 
 * Libera la memoria ocupada en la creacion del gimnasio.
 */
void destruir_gimnasio (void* gimnasio);

typedef struct gimnasio {
    char nombre[100];
    int dificultad;
    int id_funcion;
    char nombre_lider[20];
    lista_t* pila_entrenadores;
} gimnasio_t;

/* 
 * Crea un gimnasio en la memoria y devuelve un puntero a este en caso de exito.
 * Si no devuelve NULL.
 */
gimnasio_t* gimnasio_crear();

#endif
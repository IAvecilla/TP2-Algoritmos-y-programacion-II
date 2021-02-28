#ifndef __POKEMON_H__
#define __POKEMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXIMO_MEJORAS 63


typedef struct pokemon{
    char nombre[100];
    int velocidad;
    int defensa;
    int ataque;
    bool vivo;
    size_t cantidad_mejoras;
} pokemon_t;

/* 
 * Recibe un pokemon y lo libera
 */
void destruir_pokemon (pokemon_t* pokemon);

/* 
 * Crea un pokemon en memoria.
 * Devuelve NULL en caso de error.
 */
pokemon_t* crear_pokemon();

/* 
 * Mejora en 1 todas las caracteristicas del pokemon pasado por parametro
 * Esto sucede hasta un limite de 63 puntos mejorados
 */
void mejorar_atributos_pokemon (pokemon_t* pokemon_personaje);

/* 
 * Apila un elemento.
 * Devuelve 0 si pudo o -1 en caso contrario.
 */
void copiar_pokemon (pokemon_t* pokemon_a_copiar, pokemon_t* nuevo_pokemon);

#endif

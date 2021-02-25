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
    int id_pokemon;
    size_t cantidad_mejoras;
} pokemon_t;

void destruir_pokemon (pokemon_t* pokemon);

pokemon_t* crear_pokemon();

void mejorar_atributos_pokemon (pokemon_t* pokemon_personaje);

void copiar_pokemon (pokemon_t* pokemon_a_copiar, pokemon_t* nuevo_pokemon);

#endif
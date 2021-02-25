#ifndef __INTERFAZ_H__
#define __INTERFAZ_H__

#include <stdlib.h>
#include <stdio.h>
#include "gimnasio.h"
#include "jugador_principal.h"
#include "heap.h"
#include "pokemon.h"
#include "entrenador.h"

void dibujar_menu_inicio (bool cargo_jugador, bool cargar_gimnasio);

void dibujar_menu_gimnasio();

void dibujar_menu_batalla (entrenador_t* enemigo, personaje_t* jugador, pokemon_t* pokemon_personaje, pokemon_t* pokemon_rival);

void dibujar_pokemon (pokemon_t* pokemon_jugador, pokemon_t* pokemon_rival);

void dibujar_menu_victoria (bool fue_prestado, heap_t* heap);

void dibujar_menu_derrota ();

void dibujar_pokemones_rival (entrenador_t* entrenador);

void dibujar_pokemones_obtenidos (personaje_t* personaje);

void dibujar_party (personaje_t* personaje);

void mostrar_entrenador_principal (personaje_t* personaje);

void dibujar_gimnasio (gimnasio_t* gimnasio);

void dibujar_proximo_combate ();

char obtener_caracter ();

void mostrar_party_batalla_entrenador (entrenador_t* entrenador);

void mostrar_party_batalla_personaje (personaje_t* personaje);

void limpiar_consola();

#endif
#ifndef __INTERFAZ_H__
#define __INTERFAZ_H__

#include <stdlib.h>
#include <stdio.h>
#include "gimnasio.h"
#include "jugador_principal.h"
#include "heap.h"
#include "pokemon.h"
#include "entrenador.h"

/* 
 * Dibuja en consola el menu de inicio del juego.
 */
void dibujar_menu_inicio (bool cargo_jugador, bool cargar_gimnasio);

/* 
 * Dibuja el menu del gimnasio.
 */
void dibujar_menu_gimnasio();

/* 
 * Dibuja el menu de batalla.
 */
void dibujar_menu_batalla (entrenador_t* enemigo, personaje_t* jugador, pokemon_t* pokemon_personaje, pokemon_t* pokemon_rival);

/* 
 * Dibuja el nombre del pokemon y todos sus atributos.
 */
void dibujar_pokemon (pokemon_t* pokemon_jugador, pokemon_t* pokemon_rival);

/* 
 * Dibuja el menu de victoria.
 */
void dibujar_menu_victoria (bool fue_prestado, heap_t* heap);

/* 
 * Dibuja el menu de derrota
 */
void dibujar_menu_derrota ();

/* 
 * Dibuja los pokemon de la party del rival
 */
void dibujar_pokemones_rival (entrenador_t* entrenador);

/* 
 * Dibuja todos los pokemon del personaje, tanto de la party como de la mochila.
 */
void dibujar_pokemones_obtenidos (personaje_t* personaje);

/* 
 * Dibuja solo los pokemon de la party del personaje
 */
void dibujar_party (personaje_t* personaje);

/* 
 * Muestra la informacion del jugador, su nombre y su party.
 */
void mostrar_entrenador_principal (personaje_t* personaje);

/* 
 * Dibuja las caracteristicas del gimnasio como nivel, nombre del lider y nombre del gimnasio.
 */
void dibujar_gimnasio (gimnasio_t* gimnasio);

/* 
 * Dibuja la tecla a presionar para avanzar de combate
 */
void dibujar_proximo_combate ();

/* 
 * Obtiene un ingreso por parte del usuario y devuelve el el caracter ingresado.
 */
char obtener_caracter ();

/* 
 * Dibuja una interfaz de la actualidad de los pokemon del entrenador enemigo
 * Si el pokemon esta vivo se indica con un punto (.) si esta muerto, con una X.
 */
void mostrar_party_batalla_entrenador (entrenador_t* entrenador);

/* 
 * Dibuja una interfaz de la actualidad de los pokemon del personaje principal.
 * Si el pokemon esta vivo se indica con un punto (.) si esta muerto, con una X.
 */
void mostrar_party_batalla_personaje (personaje_t* personaje);

/* 
 * Limpia cualquier texto previo que estaba en la consola.
 * Esta funcion no es segura ya que utiliza funciones por fuera del estandar del lenguaje.
 */
void limpiar_consola();

#endif
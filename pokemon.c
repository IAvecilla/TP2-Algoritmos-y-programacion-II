#include "pokemon.h"


void destruir_pokemon (pokemon_t* pokemon) {
    free(pokemon);
}

pokemon_t* crear_pokemon () {
    pokemon_t* pokemon = malloc(sizeof(pokemon_t));
    if (!pokemon) {
        return NULL;
    }

    pokemon->cantidad_mejoras = 0;
    pokemon->vivo = true;

    return pokemon;
}

void mejorar_atributos_pokemon (pokemon_t* pokemon) {
    if (pokemon->cantidad_mejoras >= MAXIMO_MEJORAS) {
        return;
    }

    pokemon->ataque++;
    pokemon->defensa++;
    pokemon->velocidad++;
    pokemon->cantidad_mejoras += 3;
}

void copiar_pokemon (pokemon_t* pokemon_a_copiar, pokemon_t* nuevo_pokemon) {
    strcpy(nuevo_pokemon->nombre, pokemon_a_copiar->nombre);
    nuevo_pokemon->ataque = pokemon_a_copiar->ataque;
    nuevo_pokemon->defensa = pokemon_a_copiar->defensa;
    nuevo_pokemon->velocidad = pokemon_a_copiar->velocidad;
}
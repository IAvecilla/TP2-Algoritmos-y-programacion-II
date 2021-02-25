#include "entrenador.h"

void destruir_entrenador (void* entrenador) {
    while (!lista_vacia(((entrenador_t*)entrenador)->lista_pokemones)) {
        pokemon_t* pokemon = lista_elemento_en_posicion(((entrenador_t*)entrenador)->lista_pokemones, 0);
        lista_borrar_de_posicion(((entrenador_t*)entrenador)->lista_pokemones, 0);
        destruir_pokemon(pokemon);
    }
    lista_destruir(((entrenador_t*)entrenador)->lista_pokemones);
    free (entrenador);
}

entrenador_t* crear_entrenador () {
    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if (!entrenador) {
        return NULL;
    }

    entrenador->lista_pokemones = lista_crear();
    return entrenador;
}
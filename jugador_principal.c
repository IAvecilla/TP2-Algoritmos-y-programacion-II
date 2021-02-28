#include "jugador_principal.h"
#include "interfaz.h"


void destructor_personaje (void* personaje) {
    // Junto al personaje, elimino todos sus pokemon
    while (!lista_vacia(((personaje_t*)personaje)->lista_pokemones_obtenidos)) {
        pokemon_t* pokemon = lista_elemento_en_posicion(((personaje_t*)personaje)->lista_pokemones_obtenidos, 0);
        lista_borrar_de_posicion(((personaje_t*)personaje)->lista_pokemones_obtenidos, 0);
        destruir_pokemon(pokemon);
    }

    lista_destruir(((personaje_t*)personaje)->lista_pokemones_obtenidos);
    free(personaje);
}

personaje_t* crear_personaje () {
    personaje_t* personaje = malloc(sizeof(personaje_t));
    if(!personaje) {
        return NULL;
    }

    personaje->lista_pokemones_obtenidos = lista_crear();

    return personaje;
}

void cambiar_party (personaje_t* jugador) {
    char input[100];

    dibujar_pokemones_obtenidos(jugador);
    printf("\nSeleccione el indice del pokemon que desee cambiar: ");
    fgets(input, 100, stdin);
    int seleccion_pokemon_equipo = atoi(input);

    printf("\nSeleccione el indice del pokemon que desea intercambiar: ");
    fgets(input, 100, stdin);
    int seleccion_pokemon_mochila = atoi(input);

    // El indice no corresponde
    if (seleccion_pokemon_equipo - 1 > lista_elementos(jugador->lista_pokemones_obtenidos) || seleccion_pokemon_mochila - 1 >= lista_elementos(jugador->lista_pokemones_obtenidos)) {
        return;
    }

    lista_swap(jugador->lista_pokemones_obtenidos, seleccion_pokemon_equipo - 1, seleccion_pokemon_mochila - 1);
}
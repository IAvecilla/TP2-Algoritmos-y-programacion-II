#include "interfaz.h"

void limpiar_consola () {
    system("clear");
}

char obtener_caracter () {
    char input[10];
    do {
        fgets(input, 10, stdin);
    } while (strlen(input) != 2);
    return input[0];
}

void dibujar_menu_inicio (bool cargo_jugador, bool cargo_gimnasio) {
    if (!cargo_jugador) {
        printf("E: Cargar jugador principal\n");
    } else {
        printf("\n");
    }
    printf("A: Agregar gimnasio a la aventura pokemon\n");
    printf("I: Comenzar partida\n");
    printf("S: Simular partida\n\n");
}

void dibujar_menu_gimnasio () {
    printf("E: Ver jugador\n");
    printf("G: Ver gimnasio\n");
    printf("C: Cambiar party para combatir\n");
    printf("B: Iniciar batalla contra entrenador\n\n");
}

void dibujar_menu_batalla (entrenador_t* enemigo, personaje_t* jugador, pokemon_t* pokemon_personaje, pokemon_t* pokemon_rival) {
    mostrar_party_batalla_entrenador(enemigo);
    printf("\n\n%s\n", pokemon_rival->nombre);
    printf("Ataque: %d\n",pokemon_rival->ataque);
    printf("Defensa: %d\n",pokemon_rival->defensa);
    printf("Velocidad: %d\n\n\n",pokemon_rival->velocidad);
    printf("%s\n", pokemon_personaje->nombre);
    printf("Ataque: %d\n",pokemon_personaje->ataque);
    printf("Defensa: %d\n",pokemon_personaje->defensa);
    printf("Velocidad: %d\n\n",pokemon_personaje->velocidad);
    mostrar_party_batalla_personaje(jugador);
}

void dibujar_proximo_combate () {
    printf("N: Proximo combate\n\n");
}

void dibujar_pokemon (pokemon_t* pokemon_jugador, pokemon_t* pokemon_rival){
    limpiar_consola();
    printf("%s\n %d\n %d\n %d\n", pokemon_rival->nombre, pokemon_rival->ataque, pokemon_rival->defensa, pokemon_rival->velocidad);
    printf("\n\n");
    printf("%s\n %d\n %d\n %d\n", pokemon_jugador->nombre, pokemon_jugador->ataque, pokemon_jugador->defensa, pokemon_jugador->velocidad);
}

void dibujar_menu_victoria (bool fue_prestado, heap_t* heap) {
    if (!fue_prestado) {
        printf("T: Tomar pokemon prestado del rival\n");
    } else {
        printf("\n\n");
    }
    printf("C: Cambiar party\n");
    if (!heap_vacio(heap)) {
        printf("N: Proximo gimnasio\n\n");
    } else {
        printf("No hay mas gimnasios, presiona N para continuar\n\n");
    }
}

void dibujar_menu_derrota () {
    printf("C: Cambiar party\n");
    printf("R: Reintentar Batalla\n");
    printf("F: Finalizar Partida\n\n");
}

bool imprimir_pokemon (void* pokemon, void* extra) {
    if (*((int*)extra) != 0) {
        printf("| %s", (((pokemon_t*)pokemon)->nombre));
        *((int*)extra) -= 1;
        return true;
    }

    printf("\n");
    return false;
}

void imprimir_indices_pokemon (personaje_t* personaje, size_t recorridos, entrenador_t* entrenador) {
    pokemon_t* pokemon_actual;
    for (int i = 0; i < recorridos; i++) {
        if (!entrenador){
            pokemon_actual = lista_elemento_en_posicion(personaje->lista_pokemones_obtenidos, (size_t)i);
        } else {
           pokemon_actual = lista_elemento_en_posicion(entrenador->lista_pokemones, (size_t)i);
        }
        for (int j = 0; j <= strlen(pokemon_actual->nombre); j++){
            if (j == strlen(pokemon_actual->nombre) / 2){
                printf("%d ", i + 1);
            } else {
                printf(" ");
            }
        }
    }
}

void dibujar_pokemones_rival (entrenador_t* entrenador) {
    size_t elementos_a_recorrer = lista_elementos(entrenador->lista_pokemones);
    size_t recorridos = lista_con_cada_elemento(entrenador->lista_pokemones,imprimir_pokemon , &elementos_a_recorrer);
    printf("\n");
    imprimir_indices_pokemon(NULL, recorridos, entrenador);
}

void dibujar_pokemones_obtenidos (personaje_t* personaje) {
    size_t elementos_a_recorrer = lista_elementos(personaje->lista_pokemones_obtenidos);
    size_t recorridos = lista_con_cada_elemento(personaje->lista_pokemones_obtenidos, imprimir_pokemon, &elementos_a_recorrer);
    printf("\n");
    imprimir_indices_pokemon (personaje, recorridos, NULL);
}

void dibujar_party (personaje_t* personaje) {
    size_t elementos_a_recorrer = lista_elementos(personaje->lista_pokemones_obtenidos);
    if (elementos_a_recorrer > 6) {
        elementos_a_recorrer = 6;
    }
    lista_con_cada_elemento(personaje->lista_pokemones_obtenidos,imprimir_pokemon, &elementos_a_recorrer);
}

void mostrar_entrenador_principal (personaje_t* personaje) {
    printf("%s\n", personaje->nombre);
    dibujar_party(personaje);
    printf("\n");
}

void dibujar_gimnasio(gimnasio_t* gimnasio) {
    printf("%s\n", gimnasio->nombre);
    printf("%s\n", gimnasio->nombre_lider);
    printf("%d\n\n", gimnasio->dificultad);
}

bool imprimir_pokemon_UI (void* pokemon, void* extra) {
    if (*((int*)extra) != 0) {
        if (((pokemon_t*)pokemon)->vivo) {
            printf(". ");
        } else {
            printf("X ");
        }
        *((int*)extra) -= 1;
        return true;
    }

    printf("\n");
    return false;
}

void mostrar_party_batalla_entrenador (entrenador_t* entrenador) {
    size_t elementos_a_recorrer = lista_elementos(entrenador->lista_pokemones);
    lista_con_cada_elemento(entrenador->lista_pokemones, imprimir_pokemon_UI, &elementos_a_recorrer);
}

void mostrar_party_batalla_personaje (personaje_t* personaje) {
    size_t elementos_a_recorrer = lista_elementos(personaje->lista_pokemones_obtenidos);
    if (elementos_a_recorrer > 6) {
        elementos_a_recorrer = 6;
    }
    lista_con_cada_elemento(personaje->lista_pokemones_obtenidos, imprimir_pokemon_UI, &elementos_a_recorrer);
}

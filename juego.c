#include "heap.h"
#include "gimnasio.h"
#include "entrenador.h"
#include "pokemon.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "jugador_principal.h"
#include "batallas.h"
#include "interfaz.h"

#define FORMATO_LECTURA_GIMNASIO "%99[^;];%d;%d\n"
#define FORMATO_LECTURA_ENTRENADOR "%99[^\n]\n"
#define FORMATO_LECTURA_POKEMON "%99[^;];%d;%d;%d\n"

#define GANO_PRIMERO 1
#define GANO_SEGUNDO -1
#define VICTORIA 0
#define DERROTA -1
#define CANTIDAD_PARTY 6
#define MENSAJE_BIENVENIDA "Bienvenido a la aventura pokemon! :)\n\n"
#define MENSAJE_DERROTA "La aventura pokemon siempre te espera de vuelta! \n"
#define MENSAJE_VICTORIA "Felicidades ahora sos un verdadero maestro Pokemon!! Gracias por jugar :D"

#define ESPERANDO_INPUT 'P'

bool revivir_pokemon (void* pokemon, void* extra) {
    if (*((int*)extra) != 0) {
        if (!((pokemon_t*)pokemon)->vivo) {
            ((pokemon_t*)pokemon)->vivo = true;
        }
        *((int*)extra) -= 1;
        return true;
    }
    return false;
}

void curar_pokemon (personaje_t* personaje) {
    size_t elementos_a_recorrer = lista_elementos(personaje->lista_pokemones_obtenidos);
    if (elementos_a_recorrer > 6) {
        elementos_a_recorrer = 6;
    }
    lista_con_cada_elemento(personaje->lista_pokemones_obtenidos, revivir_pokemon, &elementos_a_recorrer);
}

void curar_pokemon_enemigo (entrenador_t* entrenador) {
    size_t elementos_a_recorrer = lista_elementos(entrenador->lista_pokemones);
    lista_con_cada_elemento(entrenador->lista_pokemones, revivir_pokemon, &elementos_a_recorrer);
}

personaje_t* cargar_jugador_principal (const char* ruta_archivo) {
    char tipo_lectura;
    bool primer_pokemon = true;

    FILE* archivo_personaje_principal = fopen(ruta_archivo, "r");
    if (!archivo_personaje_principal) {
        return NULL;
    }

    personaje_t* nuevo_personaje = crear_personaje();
    pokemon_t* nuevo_pokemon = crear_pokemon();

    int leidos = fscanf(archivo_personaje_principal, "%c;", &tipo_lectura);

    if (leidos != 1) {
        destruir_pokemon(nuevo_pokemon);
        destructor_personaje(nuevo_personaje);
        fclose(archivo_personaje_principal);
        return NULL;
    }

    while (leidos != EOF) {
        if (tipo_lectura == 'E') {
            leidos = fscanf(archivo_personaje_principal, FORMATO_LECTURA_ENTRENADOR, nuevo_personaje->nombre);
            if (leidos != 1) {
                destruir_pokemon(nuevo_pokemon);
                destructor_personaje(nuevo_personaje);
                fclose(archivo_personaje_principal);
                return NULL;
            }
        } else if (tipo_lectura == 'P') {
            if (!primer_pokemon) {
                nuevo_pokemon = crear_pokemon();
            }
            leidos = fscanf(archivo_personaje_principal, FORMATO_LECTURA_POKEMON, nuevo_pokemon->nombre, &nuevo_pokemon->velocidad, &nuevo_pokemon->ataque, &nuevo_pokemon->defensa);
            if (leidos != 4) {
                destruir_pokemon(nuevo_pokemon);
                destructor_personaje(nuevo_personaje);
                fclose(archivo_personaje_principal);
                return NULL;
            }
            lista_insertar(nuevo_personaje->lista_pokemones_obtenidos, nuevo_pokemon);
            primer_pokemon = false;
        }
        
        leidos = fscanf(archivo_personaje_principal, "%c;", &tipo_lectura);
    }
    
    fclose(archivo_personaje_principal);
    return nuevo_personaje;
}

int iniciar_batalla (entrenador_t* enemigo, personaje_t* jugador, int (*funcion_batalla) (void*, void*), int indice_pokemon_personaje, int indice_pokemon_enemigo, bool es_simulada) {
    if (indice_pokemon_personaje >= CANTIDAD_PARTY){
        return DERROTA;
    } else if (indice_pokemon_enemigo >= lista_elementos(enemigo->lista_pokemones)) {
        return VICTORIA;
    }

    limpiar_consola();
    char mensaje_ganador[100];
    pokemon_t* pokemon_ganador;
    char input;

    pokemon_t* pokemon_personaje = lista_elemento_en_posicion(jugador->lista_pokemones_obtenidos, (size_t)indice_pokemon_personaje);
    pokemon_t* pokemon_enemigo = lista_elemento_en_posicion(enemigo->lista_pokemones, (size_t)indice_pokemon_enemigo);

    if (!es_simulada) {
        dibujar_menu_batalla(enemigo, jugador, pokemon_personaje, pokemon_enemigo);
        if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_PRIMERO) {
            pokemon_enemigo->vivo = false;
            indice_pokemon_enemigo++;
            mejorar_atributos_pokemon(pokemon_personaje);
            strcpy(mensaje_ganador, "El ganador es %s y obtiene +1 en todas sus caracterisiticas\n");
            pokemon_ganador = pokemon_personaje;
            printf(mensaje_ganador, pokemon_personaje->nombre);
        } else if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_SEGUNDO) {
            pokemon_personaje->vivo = false;
            indice_pokemon_personaje++;
            strcpy(mensaje_ganador, "El ganador es %s\n");
            pokemon_ganador = pokemon_enemigo;
            printf(mensaje_ganador, pokemon_enemigo->nombre);
        }
        dibujar_proximo_combate();
        input = obtener_caracter();
    } else {
        if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_PRIMERO) {
            indice_pokemon_enemigo++;
        } else if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_SEGUNDO) {
            indice_pokemon_personaje++;
        }
    }

    while (input != 'N' && input != 'n' && !es_simulada) {
        limpiar_consola();
        dibujar_menu_batalla(enemigo, jugador, pokemon_personaje, pokemon_enemigo);
        printf(mensaje_ganador, pokemon_ganador->nombre);
        dibujar_proximo_combate();
        input = obtener_caracter();
    }
    return iniciar_batalla(enemigo, jugador, funcion_batalla, indice_pokemon_personaje, indice_pokemon_enemigo, es_simulada);    
}

void tomar_prestado (entrenador_t* entrenador, personaje_t* jugador) {
    limpiar_consola();
    char input[100];

    dibujar_pokemones_rival(entrenador);
    printf("\nSeleccione el indice del pokemon que desea tomar prestado: ");
    fgets(input, 100, stdin);
    int seleccion = atoi(input);

    if (seleccion > lista_elementos(entrenador->lista_pokemones)) {
        return;
    }

    pokemon_t* pokemon_seleccionado = lista_elemento_en_posicion(entrenador->lista_pokemones, (size_t)seleccion - 1);
    pokemon_t* nuevo_pokemon = crear_pokemon();
    copiar_pokemon(pokemon_seleccionado, nuevo_pokemon);
    lista_insertar(jugador->lista_pokemones_obtenidos, nuevo_pokemon);
}

int cargar_gimnasio (heap_t* heap,const char* ruta_archivo) {
    char tipo_lectura;
    bool primer_entrenador = true;
    bool primer_pokemon = true;
    bool primer_gimnasio = true;

    gimnasio_t* nuevo_gimnasio = gimnasio_crear();
    entrenador_t* nuevo_entrenador = crear_entrenador();
    pokemon_t* nuevo_pokemon = crear_pokemon();

    FILE* archivo_gimnasios = fopen(ruta_archivo, "r");
    if (!archivo_gimnasios) {
        free(nuevo_entrenador);
        free(nuevo_pokemon);
        free(nuevo_gimnasio);
        printf("Lo sentimos, ese archivo no pudo ser procesado\n\n");
        return -1;
    }

    int leidos = fscanf(archivo_gimnasios, "%c;", &tipo_lectura);

    if (leidos != 1) {
        free(nuevo_gimnasio);
        fclose(archivo_gimnasios);
        return -1;
    }

    while (leidos != EOF) {
        if (tipo_lectura == 'G') {
            if (!primer_gimnasio) {
                nuevo_gimnasio = gimnasio_crear();
            }
            leidos = fscanf(archivo_gimnasios, FORMATO_LECTURA_GIMNASIO, nuevo_gimnasio->nombre, &nuevo_gimnasio->dificultad, &nuevo_gimnasio->id_funcion);
            primer_gimnasio = false;
            heap_insertar(heap, nuevo_gimnasio);

        } else if (tipo_lectura == 'L' || tipo_lectura == 'E') {
            if (!primer_entrenador) {
                nuevo_entrenador = crear_entrenador();
            }
            leidos = fscanf(archivo_gimnasios, FORMATO_LECTURA_ENTRENADOR, nuevo_entrenador->nombre);
            lista_apilar(nuevo_gimnasio->pila_entrenadores, nuevo_entrenador);
            primer_entrenador = false;
            if (tipo_lectura == 'L') {
                strcpy(nuevo_gimnasio->nombre_lider, nuevo_entrenador->nombre);
            }

        } else if (tipo_lectura == 'P') {
            if (!primer_pokemon) {
                nuevo_pokemon = crear_pokemon();
            }
            leidos = fscanf(archivo_gimnasios, FORMATO_LECTURA_POKEMON, nuevo_pokemon->nombre, &nuevo_pokemon->velocidad, &nuevo_pokemon->ataque, &nuevo_pokemon->defensa);
            lista_insertar(nuevo_entrenador->lista_pokemones, nuevo_pokemon);
            primer_pokemon = false;
        }
        leidos = fscanf(archivo_gimnasios, "%c;", &tipo_lectura);
    }
    
    fclose(archivo_gimnasios);
    printf("Gimnasio/s cargado/s con exito!\n\n");
    return 0;
}

bool iniciar_juego (char ingreso_usuario) {
    return (ingreso_usuario == 'I' || ingreso_usuario == 'i' || ingreso_usuario == 'S' || ingreso_usuario == 's');
}

bool reiniciar_batalla(char ingreso_usuario) {
    return (ingreso_usuario == 'R' || ingreso_usuario == 'r');
}

entrenador_t* pasar_a_siguiente_entrenador (gimnasio_t* gimnasio_actual, personaje_t* ash) {
    lista_desapilar(gimnasio_actual->pila_entrenadores);
    entrenador_t* entrenador_siguiente = lista_tope(gimnasio_actual->pila_entrenadores);

    return entrenador_siguiente;
}

int main () {
    bool fue_prestado = false;
    bool cargo_jugador = false;
    bool gimnasios_cargados = false;
    bool es_simulada = false;
    heap_t* heap_gimnasios = heap_crear(comparador_de_gimnasios, destruir_gimnasio);
    char input = ESPERANDO_INPUT;

    int resultado_batalla;
    personaje_t* ash;

    printf("%s", MENSAJE_BIENVENIDA);
    dibujar_menu_inicio(cargo_jugador, gimnasios_cargados);

    while (!iniciar_juego(input)) {
        input = obtener_caracter();
        if ((input == 'E' || input == 'e') && !cargo_jugador) {
            limpiar_consola();
            ash = cargar_jugador_principal("jugador_principal.txt");
            cargo_jugador = true;
        }

        if (input == 'A' || input == 'a') {
            char ingreso[100];
            printf("\nPonga el nombre del archivo para cargar el/los gimnasio: ");
            fgets(ingreso, 100, stdin);
            ingreso[strcspn(ingreso, "\n")] = 0;
            limpiar_consola();
            cargar_gimnasio(heap_gimnasios, ingreso);
            if (!heap_vacio(heap_gimnasios)) {
                gimnasios_cargados = true;
            }
        }

        if (iniciar_juego(input) && !gimnasios_cargados && !cargo_jugador) {
            limpiar_consola();
            printf("Se deben cargar al menos un gimnasio\n");
            printf("Se debe cargar el jugador primero\n\n");
            input = ESPERANDO_INPUT;
        } else if (iniciar_juego(input) && !gimnasios_cargados) {
            limpiar_consola();
            printf("Se debe cargar al menos un gimnasio\n\n");
            input = ESPERANDO_INPUT;
        } else if (iniciar_juego(input) && !cargo_jugador) {
            limpiar_consola();
            printf("Se debe cargar el jugador primero\n\n");
            input = ESPERANDO_INPUT;
        } 

        dibujar_menu_inicio(cargo_jugador, gimnasios_cargados);
    }
    
    gimnasio_t* gimnasio_actual = heap_borrar_raiz(heap_gimnasios);

    if (input == 'S' || input == 's') {
        es_simulada = true;
    }

    limpiar_consola();

    while (gimnasio_actual) {
        fue_prestado = false;
        
        while (input != 'B' && input != 'b' && !es_simulada) {
            dibujar_menu_gimnasio();
            input = obtener_caracter();
            if (input == 'E' || input == 'e') {
                limpiar_consola();
                mostrar_entrenador_principal(ash);
            } else if (input == 'G' || input == 'g') {
                limpiar_consola();
                dibujar_gimnasio(gimnasio_actual);
            } else if (input == 'c' || input == 'C') {
                limpiar_consola();
                cambiar_party(ash);
            }
        }

        entrenador_t* entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores);
        while (!lista_vacia(gimnasio_actual->pila_entrenadores)) {
            switch (gimnasio_actual->id_funcion) {
                case 1:
                    resultado_batalla = iniciar_batalla(entrenador_actual, ash, funcion_batalla_1, 0, 0, es_simulada);
                    break;
                case 2:
                    resultado_batalla = iniciar_batalla(entrenador_actual, ash, funcion_batalla_2, 0, 0, es_simulada);
                    break;
                case 3:
                    resultado_batalla = iniciar_batalla(entrenador_actual, ash, funcion_batalla_3, 0, 0, es_simulada);
                    break;
                case 4:
                    resultado_batalla = iniciar_batalla(entrenador_actual, ash, funcion_batalla_4, 0, 0, es_simulada);
                    break;
                case 5:
                    resultado_batalla = iniciar_batalla(entrenador_actual, ash, funcion_batalla_5, 0, 0, es_simulada);
                    break;
            }

            if (resultado_batalla == VICTORIA) {
                limpiar_consola();
                if (!es_simulada) {
                    if (strcmp(entrenador_actual->nombre, gimnasio_actual->nombre_lider) == 0) {
                        printf("Has derrotado al lider de gimnasio %s y te entrega la medalla pulsa cualquier tecla para continuar\n\n", entrenador_actual->nombre);
                        input = obtener_caracter();
                        break;
                    } else {
                        printf("Has derrotado al entrenador %s puedes apretar N para continuar con la siguiente batalla\n\n", entrenador_actual->nombre);
                    }
                    input = obtener_caracter();
                }
                curar_pokemon(ash);
                destruir_entrenador(entrenador_actual);
                entrenador_actual = pasar_a_siguiente_entrenador (gimnasio_actual, ash);
            } else if (resultado_batalla == DERROTA && !es_simulada) {
                while (!reiniciar_batalla(input)) {
                    limpiar_consola();
                    printf("Has perdido con el entrenador %s\n\n", entrenador_actual->nombre);
                    dibujar_menu_derrota();
                    input = obtener_caracter();
                    if (input == 'C' || input == 'c') {
                        limpiar_consola();
                        cambiar_party(ash);
                    } else if (input == 'F' || input == 'f') {
                        printf(MENSAJE_DERROTA);
                        destruir_gimnasio(gimnasio_actual);
                        destructor_personaje(ash);
                        destruir_heap(heap_gimnasios);
                        return 0;
                    }
                }
                curar_pokemon_enemigo(entrenador_actual);
                curar_pokemon(ash);
                input = ESPERANDO_INPUT;
            } else {
                printf("Has perdido con el entrenador %s en %s\n\n", entrenador_actual->nombre, gimnasio_actual->nombre);
                destruir_gimnasio(gimnasio_actual);
                destructor_personaje(ash);
                destruir_heap(heap_gimnasios);
                return 0;
            }

        }

        while (!es_simulada) {
            limpiar_consola();
            dibujar_menu_victoria(fue_prestado, heap_gimnasios);
            input = obtener_caracter();
            if ((input == 'T' || input == 't') && !fue_prestado) {
                tomar_prestado(entrenador_actual, ash);
                fue_prestado = true;
            } else if (input == 'C' || input == 'c') {
                limpiar_consola();
                cambiar_party(ash);
            } else if (input == 'N' || input == 'n') {
                destruir_gimnasio(gimnasio_actual);
                gimnasio_actual = heap_borrar_raiz(heap_gimnasios);
                curar_pokemon(ash);
                break;
            }
        }

        if (es_simulada) {
            destruir_gimnasio(gimnasio_actual);
            gimnasio_actual = heap_borrar_raiz(heap_gimnasios);
        }
    }

    destructor_personaje(ash);
    destruir_heap(heap_gimnasios);
    limpiar_consola();
    printf("%s\n", MENSAJE_VICTORIA);

}
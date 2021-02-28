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
#define ERROR -1
#define OK 1

#define GANO_PRIMERO 1
#define GANO_SEGUNDO -1
#define VICTORIA 0
#define DERROTA -1
#define CANTIDAD_PARTY 6
#define MENSAJE_BIENVENIDA "Bienvenido a la aventura pokemon! :)\n\n"
#define MENSAJE_DERROTA "La aventura pokemon siempre te espera de vuelta! \n\n"
#define MENSAJE_VICTORIA "Felicidades ahora sos un verdadero maestro Pokemon!! Gracias por jugar :D"

#define ESPERANDO_INPUT 'P'

//Revive el pokemon pasado por parametro
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

//Recibe a un personaje y revive a los pokemon de su party
void curar_pokemon (personaje_t* personaje) {
    size_t elementos_a_recorrer = lista_elementos(personaje->lista_pokemones_obtenidos);
    if (elementos_a_recorrer > 6) {
        elementos_a_recorrer = 6;
    }
    lista_con_cada_elemento(personaje->lista_pokemones_obtenidos, revivir_pokemon, &elementos_a_recorrer);
}

//Recibe un entrenador y cura a todos los pokemon de su party
void curar_pokemon_enemigo (entrenador_t* entrenador) {
    size_t elementos_a_recorrer = lista_elementos(entrenador->lista_pokemones);
    lista_con_cada_elemento(entrenador->lista_pokemones, revivir_pokemon, &elementos_a_recorrer);
}

/* Recibe una ruta de archivo con el apropiado formato y devuelve un puntero al personaje principal
*  En caso de error devuelve NULL
*/
personaje_t* cargar_jugador_principal (const char* ruta_archivo) {
    char tipo_lectura;

    //Flag para ver si es el primer pokemon que se lee del archivo
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
    
    if (primer_pokemon) {
        destruir_pokemon(nuevo_pokemon);
        return NULL;
    }
    
    fclose(archivo_personaje_principal);
    return nuevo_personaje;
}

/* Recibe al enemigo y al personaje principal junto con un puntero a funcion de la batalla
*  Dos indices que comienzan en 0 y un flag para ver si la batalla debe ser simulada o no
*  Mientras sea simulada no se mostraran los menus ni los resultados de cada batalla
*  
*  Devuelve 1 si gana el persona principal, -1 si gana el enemigo.
*/
int iniciar_batalla (entrenador_t* enemigo, personaje_t* jugador, int (*funcion_batalla) (void*, void*), int indice_pokemon_personaje, int indice_pokemon_enemigo, bool es_simulada) {
    if (indice_pokemon_personaje >= CANTIDAD_PARTY){
        return DERROTA;
    } else if (indice_pokemon_enemigo >= lista_elementos(enemigo->lista_pokemones)) {
        return VICTORIA;
    }

    limpiar_consola();
    
    //Se guarda el mensaje con el nombre del pokemon ganador y el pokemon en cuestion
    char mensaje_ganador[100] = "";
    pokemon_t* pokemon_ganador = NULL;
    char input = ESPERANDO_INPUT;

    pokemon_t* pokemon_personaje = lista_elemento_en_posicion(jugador->lista_pokemones_obtenidos, (size_t)indice_pokemon_personaje);
    pokemon_t* pokemon_enemigo = lista_elemento_en_posicion(enemigo->lista_pokemones, (size_t)indice_pokemon_enemigo);

    if (!es_simulada) {
        dibujar_menu_batalla(enemigo, jugador, pokemon_personaje, pokemon_enemigo);
        if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_PRIMERO) {
            
            //Avanzamos el indice del enemigo para que la proxima batalla sea contra el proximo pokemon
            pokemon_enemigo->vivo = false;
            indice_pokemon_enemigo++;
            
            //Mejora las caracteristicas del pokemon del personaje principal
            mejorar_atributos_pokemon(pokemon_personaje);

            strcpy(mensaje_ganador, "El ganador es %s y obtiene +1 en todas sus caracterisiticas\n");
            pokemon_ganador = pokemon_personaje;
            printf(mensaje_ganador, pokemon_personaje->nombre);
        } else if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_SEGUNDO) {
            //Avanzamos el indice del personaje para que la proxima batalla sea contra el proximo pokemon
            pokemon_personaje->vivo = false;
            indice_pokemon_personaje++;
            
            strcpy(mensaje_ganador, "El ganador es %s\n");
            pokemon_ganador = pokemon_enemigo;
            printf(mensaje_ganador, pokemon_enemigo->nombre);
        }
        dibujar_proximo_combate();
        input = obtener_caracter();

    } else { //Entra en caso de que la batalla deba ser simulada
        if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_PRIMERO) {
            indice_pokemon_enemigo++;
        } else if (funcion_batalla(pokemon_personaje, pokemon_enemigo) == GANO_SEGUNDO) {
            indice_pokemon_personaje++;
        }
    }

    // Dibujamos el menu hasta que el usuario continue con la proxima batalla (solamente si no es simulada)
    while (input != 'N' && input != 'n' && !es_simulada) {
        limpiar_consola();
        dibujar_menu_batalla(enemigo, jugador, pokemon_personaje, pokemon_enemigo);
        printf(mensaje_ganador, pokemon_ganador->nombre);
        dibujar_proximo_combate();
        input = obtener_caracter();
    }

    // Llamada recursiva con nuevos indices
    return iniciar_batalla(enemigo, jugador, funcion_batalla, indice_pokemon_personaje, indice_pokemon_enemigo, es_simulada);    
}

/* Recibe al lider del gimnasio y al jugador principal, permite tomar un pokemon del lider.
*  Crea un nuevo pokemon y copia sus caracteristicas a las del lider, y lo agrega a la party del personaje.
*  En caso de error se queda todo como estaba
*/
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

    //Tomo el pokemon y luego lo copio para insertarlo en la party del personaje
    pokemon_t* pokemon_seleccionado = lista_elemento_en_posicion(entrenador->lista_pokemones, (size_t)seleccion - 1);
    pokemon_t* nuevo_pokemon = crear_pokemon();
    copiar_pokemon(pokemon_seleccionado, nuevo_pokemon);
    lista_insertar(jugador->lista_pokemones_obtenidos, nuevo_pokemon);
}
/* Recibe un heap y la ruta del archivo en cierto formato
*  Segun la lectura del archivo crea un gimnasio o varios y los inserta dentro del heap pasado por parametro
*  Si todo sale bien devuelve 1 si no, -1.
*/
int cargar_gimnasio (heap_t* heap,const char* ruta_archivo) {
    char tipo_lectura;

    //Flags para ver si es la primera lectura de cada estructura
    bool primer_entrenador = true;
    bool primer_pokemon = true;
    bool primer_gimnasio = true;

    FILE* archivo_gimnasios = fopen(ruta_archivo, "r");
    if (!archivo_gimnasios) {
        printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
        return ERROR;
    }

    gimnasio_t* nuevo_gimnasio = gimnasio_crear();
    entrenador_t* nuevo_entrenador = crear_entrenador();
    pokemon_t* nuevo_pokemon = crear_pokemon();


    int leidos = fscanf(archivo_gimnasios, "%c;", &tipo_lectura);

    if (leidos != 1) {
        destruir_gimnasio(nuevo_gimnasio);
        destruir_pokemon(nuevo_pokemon);
        destruir_entrenador(nuevo_entrenador);
        fclose(archivo_gimnasios);
        printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
        return ERROR;
    }

    while (leidos != EOF) {
        if (tipo_lectura == 'G') {
            if (!primer_gimnasio) {
                nuevo_gimnasio = gimnasio_crear();
            }
            leidos = fscanf(archivo_gimnasios, FORMATO_LECTURA_GIMNASIO, nuevo_gimnasio->nombre, &nuevo_gimnasio->dificultad, &nuevo_gimnasio->id_funcion);
            if (leidos != 3 && primer_entrenador && primer_gimnasio && primer_pokemon) {
                destruir_gimnasio(nuevo_gimnasio);
                destruir_pokemon(nuevo_pokemon);
                destruir_entrenador(nuevo_entrenador);
                fclose(archivo_gimnasios);
                return ERROR;
            } else if (leidos != 3) {
                destruir_heap(heap);
            }
            primer_gimnasio = false;
            heap_insertar(heap, nuevo_gimnasio);

        } else if (tipo_lectura == 'L' || tipo_lectura == 'E') {
            if (!primer_entrenador) {
                nuevo_entrenador = crear_entrenador();
            }
            leidos = fscanf(archivo_gimnasios, FORMATO_LECTURA_ENTRENADOR, nuevo_entrenador->nombre);
            if (leidos != 1) {
                destruir_entrenador(nuevo_entrenador);
                fclose(archivo_gimnasios);
                printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
                return ERROR;
            }
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
            if (leidos != 4) {
                destruir_pokemon(nuevo_pokemon);
                fclose(archivo_gimnasios);
                printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
                return ERROR;
            }
            lista_insertar(nuevo_entrenador->lista_pokemones, nuevo_pokemon);
            primer_pokemon = false;
        }
        leidos = fscanf(archivo_gimnasios, "%c;", &tipo_lectura);
    }
    
    if (primer_gimnasio) {
        destruir_gimnasio(nuevo_gimnasio);
        printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
        return ERROR;
    }
    
    if (primer_entrenador) {
        destruir_entrenador(nuevo_entrenador);
        printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
        return ERROR;   
    }
    
    if (primer_pokemon) {
        destruir_pokemon(nuevo_pokemon);
        printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
        return ERROR;
    }
    
    fclose(archivo_gimnasios);
    printf("Gimnasio/s cargado/s con exito!\n\n");
    return OK;
}

// Chequea si el usuario quiere iniciar el juego segun las teclas pulsadas
bool iniciar_juego (char ingreso_usuario) {
    return (ingreso_usuario == 'I' || ingreso_usuario == 'i' || ingreso_usuario == 'S' || ingreso_usuario == 's');
}

// Chequea si el usuario quiere reiniciar una batalla segun las teclas pulsadas
bool reiniciar_batalla(char ingreso_usuario) {
    return (ingreso_usuario == 'R' || ingreso_usuario == 'r');
}

// Recibe el gimnasio actual de la partida y devuelve al siguiente entrenador luego de derrotar a uno
entrenador_t* pasar_a_siguiente_entrenador (gimnasio_t* gimnasio_actual) {
    lista_desapilar(gimnasio_actual->pila_entrenadores);
    entrenador_t* entrenador_siguiente = lista_tope(gimnasio_actual->pila_entrenadores);

    return entrenador_siguiente;
}

gimnasio_t* pasar_a_siguiente_gimnasio (gimnasio_t* gimnasio_actual, heap_t* heap_gimnasios) {
    destruir_gimnasio(gimnasio_actual);
    gimnasio_t* proximo_gimnasio = heap_borrar_raiz(heap_gimnasios);

    return proximo_gimnasio;
}

int main () {
    // Flags utiles en el programa
    bool fue_prestado = false;
    bool cargo_jugador = false;
    bool gimnasios_cargados = false;
    bool es_simulada = false;

    // Array que contiene punteros a las funciones de batalla
    int (*array[]) (void*, void*) = {funcion_batalla_1, funcion_batalla_2, funcion_batalla_3, funcion_batalla_4, funcion_batalla_5};

    heap_t* heap_gimnasios = heap_crear(comparador_de_gimnasios, destruir_gimnasio);
    char input = ESPERANDO_INPUT;
    int resultado_batalla;
    personaje_t* ash;

    printf(BLANCO_NEGRITA "%s", MENSAJE_BIENVENIDA);
    dibujar_menu_inicio(cargo_jugador, gimnasios_cargados);

    // Loop del menu inicial
    while (!iniciar_juego(input)) {
        input = obtener_caracter();

        //Carga personaje
        if ((input == 'E' || input == 'e') && !cargo_jugador) {
            char ingreso[100];
            printf("\nPonga el nombre del archivo para cargar el/los gimnasio (Ej: ""nombre.txt""): ");
            fgets(ingreso, 100, stdin);

            //Elimino el "Enter" final del usuario en su ingresodel archivo
            ingreso[strcspn(ingreso, "\n")] = 0;

            limpiar_consola();
            ash = cargar_jugador_principal(ingreso);
            if (!ash) {
                printf("Jugador no pudo ser cargado\n");
            } else {
                cargo_jugador = true;
                printf("Jugador cargado con exito!\n");
            }
        }

        // Pido el nombre del archivo y carga gimnasio/s
        if (input == 'A' || input == 'a') {
            char ingreso[100];
            printf("\nPonga el nombre del archivo para cargar el/los gimnasio (Ej: ""nombre.txt""): ");
            fgets(ingreso, 100, stdin);
            
            //Elimino el "Enter" final del usuario en su ingresodel archivo
            ingreso[strcspn(ingreso, "\n")] = 0;
            
            limpiar_consola();
            int validez = cargar_gimnasio(heap_gimnasios, ingreso);
            if (validez == -1) {
                printf("Lo sentimos no se pudo cargar el archivo correctamente\n");
            }
            if (!heap_vacio(heap_gimnasios)) {
                gimnasios_cargados = true;
            }
        }
        
        //Chequeo que el jugador haya cargado al personaje y por lo menos un gimnasio
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
    
    // Primer gimnasio
    gimnasio_t* gimnasio_actual = heap_borrar_raiz(heap_gimnasios);

    // Chequeo para ver si la batalla debe ser simulada
    if (input == 'S' || input == 's') {
        es_simulada = true;
    }

    limpiar_consola();

    // Loop principal del juego, mientras haya gimnasios, continua jugando
    while (gimnasio_actual) {

        //Loop para el menu de batalla, se muestra mientras no sea una partida simulada
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

        //Entrenador con el que se debe luchar
        entrenador_t* entrenador_actual = lista_tope(gimnasio_actual->pila_entrenadores);

        // Loop para el gimnasio, se ejecuta mientras haya entrenadores con los que batallar
        while (!lista_vacia(gimnasio_actual->pila_entrenadores)) {
            // Se inicia la batalla segun el id del gimnasio
            resultado_batalla = iniciar_batalla(entrenador_actual, ash, array[gimnasio_actual->id_funcion - 1], 0, 0, es_simulada);

            // En caso de victoria contra el entrenador actual
            if (resultado_batalla == VICTORIA) {
                limpiar_consola();
                if (!es_simulada) {

                    // En caso de que derrotemos al lider, se notifica y saltamos lo demas para avanzar de gimnasio
                    if (strcmp(entrenador_actual->nombre, gimnasio_actual->nombre_lider) == 0) {
                        printf("Has derrotado al lider de gimnasio %s y te entrega la medalla pulsa cualquier tecla para continuar\n\n", entrenador_actual->nombre);
                        input = obtener_caracter();
                        break;
                    } else {
                        printf("Has derrotado al entrenador %s puedes apretar N para continuar con la siguiente batalla\n\n", entrenador_actual->nombre);
                    }
                    input = obtener_caracter();
                }

                // Si no fue la ultima batalla del gimnasio, curamos a los pokemon y avanzamos de entrenador
                curar_pokemon(ash);
                destruir_entrenador(entrenador_actual);
                entrenador_actual = pasar_a_siguiente_entrenador (gimnasio_actual);

            //En caso de derrota contra el entrenador
            } else if (resultado_batalla == DERROTA && !es_simulada) {

                // Loop para el menu de derrota
                while (!reiniciar_batalla(input)) {
                    limpiar_consola();
                    printf("Has perdido con el entrenador %s\n\n", entrenador_actual->nombre);
                    dibujar_menu_derrota();
                    input = obtener_caracter();
                    if (input == 'C' || input == 'c') {
                        limpiar_consola();
                        cambiar_party(ash);
                    } else if (input == 'F' || input == 'f') {
                        limpiar_consola();
                        printf(MENSAJE_DERROTA);
                        imprimir_ditto();

                        destruir_gimnasio(gimnasio_actual);
                        destructor_personaje(ash);
                        destruir_heap(heap_gimnasios);
                        return 0;
                    }
                }
                // En caso de reintentar, se curan pokemon de ambos entrenadores y continua
                curar_pokemon_enemigo(entrenador_actual);
                curar_pokemon(ash);
                input = ESPERANDO_INPUT;
            
            // En caso de derrota en una batalla simulada, se termina el juego
            } else {
                limpiar_consola();
                printf("Has perdido con el entrenador %s en %s\n\n", entrenador_actual->nombre, gimnasio_actual->nombre);
                imprimir_ditto();
                destruir_gimnasio(gimnasio_actual);
                destructor_personaje(ash);
                destruir_heap(heap_gimnasios);
                return 0;
            }

        }

        // Al terminar el gimnasio y no ser una partida simulada, se despliega el menu de victoria
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

            //Avanza al proximo gimnasio
            } else if (input == 'N' || input == 'n') {
                gimnasio_actual = pasar_a_siguiente_gimnasio(gimnasio_actual, heap_gimnasios);
                curar_pokemon(ash);
                break;
            }
        }

        // Actualizamos el flag ya que sirve para el proximo gimnasio
        fue_prestado = false;

        // En caso de ser una partida simulada, simplemente avanzo
        if (es_simulada) {
            gimnasio_actual = pasar_a_siguiente_gimnasio(gimnasio_actual, heap_gimnasios);
        }
    }

    // En este punto el jugador consiguio la victoria, se imprime el mensaje final y termina el juego
    destructor_personaje(ash);
    destruir_heap(heap_gimnasios);
    limpiar_consola();
    printf("%s\n", MENSAJE_VICTORIA);

}
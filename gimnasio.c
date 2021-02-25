#include "gimnasio.h"

gimnasio_t* gimnasio_crear () {
    gimnasio_t* gimnasio = malloc(sizeof(gimnasio_t));
    if (!gimnasio) {
        return NULL;
    }

    gimnasio->pila_entrenadores = lista_crear();
    return gimnasio;
}

int comparador_de_gimnasios (void* gimnasio1, void* gimnasio2) {
    
    if (((gimnasio_t*)gimnasio1)->dificultad > ((gimnasio_t*)gimnasio2)->dificultad) {
        return 1;
    } 

    if (((gimnasio_t*)gimnasio2)->dificultad > ((gimnasio_t*)gimnasio1)->dificultad) {
        return -1;
    }
    return 0;
}

void destruir_gimnasio (void* gimnasio) {
    while (!lista_vacia(((gimnasio_t*)gimnasio)->pila_entrenadores)) {
        entrenador_t* entrenador = lista_tope(((gimnasio_t*)gimnasio)->pila_entrenadores);
        destruir_entrenador(entrenador);
        lista_desapilar(((gimnasio_t*)gimnasio)->pila_entrenadores);
    }

    lista_destruir(((gimnasio_t*)gimnasio)->pila_entrenadores);
    free(gimnasio);
}
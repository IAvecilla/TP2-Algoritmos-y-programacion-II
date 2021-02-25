#include <stdlib.h>
#include "lista.h"

#define ERROR -1
#define EXITO 0

lista_t* lista_crear() {
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) {
        return NULL;
    }

    //Inicializo los valores de la lista
    lista->cantidad = 0;
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    
    return lista;
}

int lista_insertar(lista_t* lista, void* elemento) {
    if (!lista) {
        return ERROR;
    }

    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo) {
        return ERROR;
    }

    //Inicializo los valores del nodo a insertar
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;

    //Chequeo si la lista esta vacia y este es el primer elemento a insertar
    if (lista->cantidad == 0) {
        lista->nodo_inicio = nuevo_nodo;
    
    } else {
        nodo_t* aux = lista->nodo_inicio;
        //Avanzo el auxiliar hasta el ultimo nodo de la lista
        while (aux->siguiente) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo_nodo;
    }  

    lista->cantidad++;
    return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {
    if (!lista) {
        return ERROR;
    }

    //Si la posicion es mayor que el largo de la lista, inserto al final
    if (posicion >= lista->cantidad) {
        return lista_insertar(lista, elemento);
    }

    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo) {
        return ERROR;
    }
    
    nuevo_nodo->elemento = elemento;
    nodo_t* aux = lista->nodo_inicio;

    //Chequeo si la posicion pasada es 0 y va a ser mi nuevo nodo inicio
    if (posicion == 0) {
        lista->nodo_inicio = nuevo_nodo;
        lista->nodo_inicio->siguiente = aux;

    } else {
        //Avanzo hasta el nodo que va a preceder a el nuevo nodo
        for (int i = 1; i < posicion; i++) {
            aux = aux->siguiente;
        }

        //Lo inserto entre medio de los dos elementos 
        nuevo_nodo->siguiente = aux->siguiente;
        aux->siguiente = nuevo_nodo;
    }

    lista->cantidad++;
    return EXITO;
}

int lista_borrar(lista_t* lista) {
    if (!lista || lista->cantidad == 0) {
        return ERROR;
    }

    //Chequeo si unicamente tengo un elemento, si es asi mi lista queda vacia
    if (lista->cantidad == 1) {
        free(lista->nodo_inicio);
        lista->nodo_inicio = NULL;

    } else {
        nodo_t* aux = lista->nodo_inicio;

        //Avanzo hasta el nodo anterior al ultimo y libero el ultimo 
        while (aux->siguiente->siguiente) {
            aux = aux->siguiente;
        }
        free(aux->siguiente);

        //Ahora el anterior donde estaba parado, es el nuevo ultimo
        aux->siguiente = NULL;
    }

    lista->cantidad--;
    return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion) {
    if (!lista || lista->cantidad == 0) {
        return ERROR;
    }

    //Si la posicion es mayor que el largo de la lista, borro el ultimo
    if (posicion >= lista->cantidad) {
        return lista_borrar(lista);
    }

    //Creo dos auxiliares, uno apuntando al primer nodo y otro al segundo
    nodo_t* aux = lista->nodo_inicio;
    nodo_t* aux_prox = lista->nodo_inicio->siguiente;

    //Chequeo si la posicion es 0 y reasigno el nodo inicio al elemento que le seguia
    if (posicion == 0) {
        free(lista->nodo_inicio);
        lista->nodo_inicio = aux_prox;

    } else {
        //Avanzo mis dos punteros a la vez, hasta que uno llegue al anteriro al nodo a eliminar
        //Y el otro que apunte al nodo a eliminar
        for(int i = 1; i < posicion; i++) {
            aux = aux->siguiente;
            aux_prox = aux_prox->siguiente;
        }

        //Ahora reasigno el siguiente de mi nodo anterior al eliminado
        aux->siguiente = aux_prox->siguiente;
        free(aux_prox);
    }

    lista->cantidad--;
    return EXITO;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion) {
    if (!lista) {
        return NULL;
    }

    if (posicion >= lista->cantidad) {
        return NULL;
    }

    nodo_t* aux = lista->nodo_inicio;

    //Avanzo mi auxiliar hasta el nodo que esta en la posicion
    for (int i = 0; i < posicion; i++) {
        aux = aux->siguiente;
    }

    return aux->elemento;
}

void* lista_ultimo(lista_t* lista) {
    if (!lista || lista->cantidad == 0) {
        return NULL;
    }

    nodo_t* aux = lista->nodo_inicio;

    //Avanzo hasta el ultimo nodo de la lista
    while (aux->siguiente) {
        aux = aux->siguiente;
    }

    return aux->elemento;
}

bool lista_vacia(lista_t* lista) {
    if (!lista) {
        return true;
    }

    return lista->cantidad == 0;
}

size_t lista_elementos(lista_t* lista) {
    if (!lista) {
        return EXITO;
    }

    return lista->cantidad;
}

int lista_apilar(lista_t* lista, void* elemento) {
    if (!lista) {
        return ERROR;
    }

    //Lista apilar es lo mismo que insertar en la posicion 0 de una lista
    return lista_insertar_en_posicion(lista, elemento, 0);
}

int lista_desapilar(lista_t* lista) {
    if (!lista || lista->cantidad == 0) {
        return ERROR;
    }

    //Lista apilar es lo mismo que borrar de la posicion 0 de una lista
    return lista_borrar_de_posicion(lista, 0);
}

void* lista_tope(lista_t* lista) {
    if (!lista || lista->cantidad == 0) {
        return NULL;
    }

    return lista->nodo_inicio->elemento;

}

int lista_encolar(lista_t* lista, void* elemento) {
    if (!lista) {
        return ERROR;
    }

    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo) {
        return ERROR;
    }

    //Inicializo los valores del nodo a encolar
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;

    //Chequeo si la cola esta vacia y el nuevo nodo sera mi primer elemento
    if (lista->cantidad == 0) {
        lista->nodo_inicio = nuevo_nodo;
        lista->nodo_fin = nuevo_nodo;

    } else {
        lista->nodo_fin->siguiente = nuevo_nodo;
        lista->nodo_fin = nuevo_nodo;
    }

    lista->cantidad++;
    return EXITO;
}

int lista_desencolar(lista_t* lista) {
    if (!lista || lista->cantidad == 0) {
        return ERROR;
    }

    //Chequeo si mi lista tiene un solo elemento, si es asi mi cola queda vacia
    if (lista->cantidad == 1) {
        free(lista->nodo_inicio);
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;

    } else {
        //Reasigno mi nodo inicio al siguiente en la cola y libero el primero
        nodo_t* aux = lista->nodo_inicio;
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
        free(aux);
    }

    lista->cantidad--;
    return EXITO;
}

void* lista_primero(lista_t* lista) {
    if (!lista || lista->cantidad == 0) {
        return NULL;
    }

    return lista->nodo_inicio->elemento;
}

void lista_destruir(lista_t* lista) {
    if (!lista) {
        return;
    }

    nodo_t* aux = lista->nodo_inicio;
    /*Avanzo el nodo inicio hasta que recorra todos los elementos
    A su vez el auxiliar lo recorre 1 posicion detras del puntero nodo inicio y va liberando
    todos los nodos de la lista*/
    while (lista->nodo_inicio) {
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
        free(aux);
        aux = lista->nodo_inicio;
    }

    free (lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista) {
    if (!lista) {
        return NULL;
    }

    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if (!iterador) {
        return NULL;
    }

    //Inicializo los valores de mi iterador
    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;

    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
    if (!iterador) {
        return false;
    }

    if (!iterador->corriente) {
        return false;
    }

    //Chequeo si ya me quede sin nodos para iterar
    return iterador->corriente != NULL;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador) {
    if (!iterador) {
        return false;
    }
    
    //Compruebo que la lista no este vacia
    if (!iterador->corriente) {
        return false;
    } else if (!iterador->corriente->siguiente) { //Chequeo si estoy en ultimo elemento, si es asi itero el ultimo elemento y dejo el iterador apuntando a NULL
        iterador->corriente = iterador->corriente->siguiente;
        return false;
    }
    //Avanzo el iterador y chequeo si ya recorri todos los nodos en la lista
    iterador->corriente = iterador->corriente->siguiente;

    return true;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
    if (!iterador) {
        return NULL;
    }
    //Chequeo si no tengo un nodo valido para poder devolver el elemento
    if (!iterador->corriente) {
        return NULL;
    }

    return iterador->corriente->elemento;
}


void lista_iterador_destruir(lista_iterador_t* iterador) {
    if (!iterador) {
        return;
    }

    free(iterador);
}


size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto) {
    if (!lista || !funcion) {
        return EXITO;
    }

    size_t elementos_recorridos = 0;
    nodo_t* aux = lista->nodo_inicio;

    //Recorro los nodos de la lista hasta que no tengo mas elementos
    for (int i = 0; aux; i++) {
        
        //Chequeo si la funcion retorna falso, si es asi, dejo de recorrer
        if (!funcion(aux->elemento, contexto))
            return elementos_recorridos;
        
        elementos_recorridos++;
        aux = aux->siguiente;
    }

    return elementos_recorridos;
}

int lista_swap (lista_t* lista, int i, int j) {
    if (i >= lista_elementos(lista) || j >= lista_elementos(lista)) {
        return ERROR;
    }

    if (j == i) {
        return EXITO;
    }

    nodo_t* aux_nodo_1 = lista->nodo_inicio;
    nodo_t* aux_nodo_2 = lista->nodo_inicio;

    while (i) {
        aux_nodo_1 = aux_nodo_1->siguiente;
        i--;
    }

    while (j) {
        aux_nodo_2 = aux_nodo_2->siguiente;
        j--;
    }

    void* aux_elemento = aux_nodo_1->elemento;
    aux_nodo_1->elemento = aux_nodo_2->elemento;
    aux_nodo_2->elemento = aux_elemento;

    return EXITO;
}
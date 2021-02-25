#include "abb.h"

#define ERROR -1
#define EXITO 0

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {
    if (!comparador) {
        return NULL;
    }

    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol) {
        return NULL;
    }

    arbol->comparador = comparador;
    arbol->destructor = destructor;
    arbol->nodo_raiz = NULL;

    return arbol;
}

int insertar_nodo (abb_comparador comparador, nodo_abb_t* raiz, nodo_abb_t* nodo_a_insertar) {
    int comparacion = comparador(raiz->elemento , nodo_a_insertar->elemento);

    if (comparacion >= 1 || comparacion == 0) {

        if (!raiz->izquierda) {
            raiz->izquierda = nodo_a_insertar;
            return EXITO;
        }
        return insertar_nodo(comparador, raiz->izquierda, nodo_a_insertar);

    } else if (comparacion <= -1) {

        if (!raiz->derecha) {
            raiz->derecha = nodo_a_insertar;
            return EXITO;
        }
        return insertar_nodo(comparador, raiz->derecha, nodo_a_insertar);
    }

    return EXITO;
}

int arbol_insertar(abb_t* arbol, void* elemento) {
    if (!arbol) {
        return ERROR;
    }

    nodo_abb_t* nuevo_nodo = calloc(1, sizeof(nodo_abb_t));
    if (!nuevo_nodo) {
        return ERROR;
    }

    nuevo_nodo->elemento = elemento;

    if (!arbol->nodo_raiz) {
        arbol->nodo_raiz = nuevo_nodo;
        return EXITO;
    }

    return insertar_nodo (arbol->comparador , arbol->nodo_raiz, nuevo_nodo);
}

void* buscar_nodo (abb_comparador comparador, nodo_abb_t* raiz, void* elemento_buscado) {

    if (comparador(raiz->elemento , elemento_buscado) == 0) {
        return raiz->elemento;
    }

    if (comparador(raiz->elemento , elemento_buscado) >= 1) {
        if (!raiz->izquierda) {
            return NULL;
        }
        return buscar_nodo(comparador, raiz->izquierda, elemento_buscado);
    }

    if (!raiz->derecha) {
        return NULL;
    }

    return buscar_nodo(comparador, raiz->derecha, elemento_buscado);
}

void* arbol_buscar(abb_t* arbol, void* elemento) {
    if (!arbol || !arbol->nodo_raiz) {
        return NULL;
    }

    return buscar_nodo (arbol->comparador, arbol->nodo_raiz, elemento);
}

void* arbol_raiz(abb_t* arbol) {
    if (!arbol || !arbol->nodo_raiz) {
        return NULL;
    }

    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol) {

    return !arbol || !arbol->nodo_raiz;
}

size_t recorrer_inorden (nodo_abb_t* raiz, void** array, size_t tamanio_array , size_t* elementos_recorridos) {
    if (!raiz || (*elementos_recorridos) >= tamanio_array) {
        return *elementos_recorridos;
    }

    recorrer_inorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);
    if ((*elementos_recorridos) < tamanio_array){
        array[*elementos_recorridos] = raiz->elemento;
        (*elementos_recorridos)++;
    }
    recorrer_inorden (raiz->derecha, array, tamanio_array, elementos_recorridos);

    return *elementos_recorridos;
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {
    if (!arbol || !array || !tamanio_array) {
        return 0;
    }
    size_t contador = 0;

    return recorrer_inorden (arbol->nodo_raiz, array, tamanio_array, &contador);
}

size_t recorrer_preorden (nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t* elementos_recorridos) {
    if (!raiz || (*elementos_recorridos) >= tamanio_array) {
        return *elementos_recorridos;
    }

    if ((*elementos_recorridos) < tamanio_array){
        array[*elementos_recorridos] = raiz->elemento;
        (*elementos_recorridos)++;
    }
    recorrer_preorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);
    recorrer_preorden(raiz->derecha, array, tamanio_array, elementos_recorridos);

    return *elementos_recorridos;
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array) {
    if (!arbol || !array) {
        return 0;
    }
    size_t contador = 0;

    return recorrer_preorden (arbol->nodo_raiz, array, tamanio_array, &contador);

}

size_t recorrer_postorden (nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t* elementos_recorridos) {
    if (!raiz || (*elementos_recorridos) >= tamanio_array) {
        return *elementos_recorridos;
    }

    recorrer_postorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);
    recorrer_postorden(raiz->derecha, array, tamanio_array, elementos_recorridos);
    if ((*elementos_recorridos) < tamanio_array){
        array[*elementos_recorridos] = raiz->elemento;
        (*elementos_recorridos)++;
    }

    return *elementos_recorridos;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array) {
    if (!arbol || !array) {
        return 0;
    }
    size_t contador = 0;

    return recorrer_postorden (arbol->nodo_raiz, array, tamanio_array, &contador);
}

void eliminar_nodos (abb_t* arbol, nodo_abb_t* raiz) {
    if (!raiz) {
        return;
    }

    eliminar_nodos (arbol, raiz->izquierda);
    eliminar_nodos (arbol, raiz->derecha);
    if (arbol->destructor) {
        arbol->destructor(raiz->elemento);
    }
    free(raiz);

    return;
}

void arbol_destruir(abb_t* arbol) {
    if (!arbol){
        return;
    }

    eliminar_nodos(arbol, arbol->nodo_raiz);
    free(arbol);
}

nodo_abb_t* reemplazar_sucesor_inorden (nodo_abb_t* actual) {
  nodo_abb_t* aux = actual->izquierda;
  nodo_abb_t* nuevo_nodo_hoja = NULL;
  while (aux->derecha) {
      nuevo_nodo_hoja = aux;
      aux = aux->derecha;
  }
  aux->derecha = actual->derecha;
  if (nuevo_nodo_hoja) {
      nuevo_nodo_hoja->derecha = NULL;
  }
  if (actual->izquierda != aux) {
      aux->izquierda = actual->izquierda;
  }

  return aux;
}

nodo_abb_t* borrar_nodo (abb_liberar_elemento destructor ,abb_comparador comparador, nodo_abb_t* raiz, void* elemento_a_borrar) {

     if (comparador(raiz->elemento , elemento_a_borrar) == 0) {
        if (!raiz->derecha && !raiz->izquierda) {
            if (destructor) {
                destructor(raiz->elemento);
            }
            free(raiz);
            return NULL;
        }

        if (!raiz->izquierda) {
            nodo_abb_t* aux = raiz->derecha;
            if (destructor){
                destructor(raiz->elemento);
            }
            free(raiz);
            return aux;
        }

        if (!raiz->derecha) {
            nodo_abb_t* aux = raiz->izquierda;
            if (destructor) {
                destructor(raiz->elemento);
            }
            free(raiz);
            return aux;
        }

        nodo_abb_t* aux = reemplazar_sucesor_inorden(raiz);
        if (destructor) {
            destructor(raiz->elemento);
        }
        free(raiz);
        return aux;

    } else if (comparador(raiz->elemento , elemento_a_borrar) >= 1) {
        raiz->izquierda = borrar_nodo(destructor, comparador, raiz->izquierda, elemento_a_borrar);
    } else if (comparador(raiz->elemento , elemento_a_borrar) <= -1) {
        raiz->derecha = borrar_nodo(destructor, comparador, raiz->derecha, elemento_a_borrar);
    }

    return raiz;
}

int arbol_borrar(abb_t* arbol, void* elemento) {
    if (!arbol || !arbol->nodo_raiz) {
        return ERROR;
    }

    if (!buscar_nodo(arbol->comparador,arbol->nodo_raiz, elemento)) {
        return ERROR;
    }

    if (arbol->comparador(arbol->nodo_raiz->elemento, elemento) == 0){
        arbol->nodo_raiz = borrar_nodo(arbol->destructor,arbol->comparador, arbol->nodo_raiz, elemento);
        return EXITO;
    }
    borrar_nodo(arbol->destructor,arbol->comparador, arbol->nodo_raiz, elemento);
    return EXITO;

}

size_t iterador_interno_inorden (nodo_abb_t* raiz, bool (*funcion)(void*,void*), void* extra, size_t* elementos_recorridos, bool* funcion_verdadera) {
    if (!raiz) {
        return *elementos_recorridos;
    }
    if (!(*funcion_verdadera)){
      iterador_interno_inorden (raiz->izquierda, funcion, extra, elementos_recorridos, funcion_verdadera);
    }
    if (!(*funcion_verdadera)) {
      if (funcion(raiz->elemento, extra)) {
        *funcion_verdadera = true;
        (*elementos_recorridos)++;
      }
    }
    if (!(*funcion_verdadera)) {
        (*elementos_recorridos)++;
        iterador_interno_inorden (raiz->derecha, funcion, extra,  elementos_recorridos, funcion_verdadera);
    }

    return *elementos_recorridos;
}

size_t iterador_interno_preorden (nodo_abb_t* raiz, bool (*funcion)(void*,void*), void* extra, size_t* elementos_recorridos, bool* funcion_verdadera) {
    if (!raiz) {
        return *elementos_recorridos;
    }
    if (funcion(raiz->elemento, extra)) {
        *funcion_verdadera = true;
        (*elementos_recorridos)++;
        return *elementos_recorridos;
    }

    if (!(*funcion_verdadera)) {
        (*elementos_recorridos)++;
        if (!(*funcion_verdadera)) {
          iterador_interno_preorden (raiz->izquierda, funcion, extra, elementos_recorridos, funcion_verdadera);
        }
        if (!(*funcion_verdadera)){
          iterador_interno_preorden (raiz->derecha, funcion, extra,  elementos_recorridos, funcion_verdadera);
        }
    }

    return *elementos_recorridos;
}

size_t iterador_interno_postorden (nodo_abb_t* raiz, bool (*funcion)(void*,void*), void* extra, size_t* elementos_recorridos, bool* funcion_verdadera) {
    if (!raiz || *funcion_verdadera) {
        return *elementos_recorridos;
    }

    if (!(*funcion_verdadera)) {
      iterador_interno_postorden (raiz->izquierda, funcion, extra, elementos_recorridos, funcion_verdadera);
    }
    if (!(*funcion_verdadera)) {
        iterador_interno_postorden (raiz->derecha, funcion, extra,  elementos_recorridos, funcion_verdadera);
        if (!(*funcion_verdadera) && funcion(raiz->elemento, extra)){
            *funcion_verdadera = true;
            (*elementos_recorridos)++;
            return *elementos_recorridos;
        }
        if (!(*funcion_verdadera)) {
            (*elementos_recorridos)++;
        }
    }
    return *elementos_recorridos;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {
    if (!arbol || !funcion) {
        return 0;
    }

    size_t contador = 0;
    bool funcion_verdadera = false;

    switch (recorrido) {
        case ABB_RECORRER_INORDEN:
            return iterador_interno_inorden(arbol->nodo_raiz, funcion, extra, &contador, &funcion_verdadera);
        case ABB_RECORRER_PREORDEN:
            return iterador_interno_preorden(arbol->nodo_raiz, funcion, extra, &contador, &funcion_verdadera);
        case ABB_RECORRER_POSTORDEN:
            return iterador_interno_postorden(arbol->nodo_raiz, funcion, extra, &contador, &funcion_verdadera);
        default:
            return 0;
    }
}

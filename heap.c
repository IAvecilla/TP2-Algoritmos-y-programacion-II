#include "heap.h"
#include <stdio.h>

#define ERROR -1
#define EXITO 0

//extern void mostrar_arbol(void* buffer, int n);

struct heap {
  heap_comparador comparador;
  heap_liberar_elemento destructor;
  void** vector;
  int tope;
};

//COMPARADOR CREADO PARA HACER PRUEBAS CON NUMEROS ENTEROS
/*
int comparar_numeros(void* a, void* b) {
    if (a > b) {
      return 1;
    }

    if (b > a) {
      return -1;
    }

    return 0;
}*/

heap_t* heap_crear (heap_comparador comparador, heap_liberar_elemento destructor) {
    if (!comparador) {
      return NULL;
    }

    heap_t* heap = calloc(1, sizeof(heap_t));
    if (!heap) {
      return NULL;
    }

    heap->comparador = comparador;
    heap->destructor = destructor;

    return heap;
}

int posicion_padre (int n) {
  return (n-1)/2;
}

int posicion_hijo_derecho (int n) {
  return 2*n+2;
}

int posicion_hijo_izquierdo (int n) {
  return 2*n+1;
}

void swap (void** vector, int i, int j) {
  void* aux = vector[i];
  vector[i] = vector[j];
  vector[j] = aux;
}

void sift_up (heap_t* heap, int n) {
    int pos_padre = posicion_padre(n);

    if (n == 0) {
      return;
    }

    if (heap->comparador(heap->vector[pos_padre], heap->vector[n]) == 1) {
        swap(heap->vector, pos_padre, n);
        sift_up (heap, pos_padre);
    }
}

void sift_down (heap_t* heap, int n) {
    int pos_izquierdo = posicion_hijo_izquierdo(n);
    int pos_derecho = posicion_hijo_derecho(n);
    int pos_menor;

    if (pos_izquierdo >= heap->tope) {
      return;
    }

    if (heap->comparador(heap->vector[pos_izquierdo], heap->vector[pos_derecho]) == -1) {
      pos_menor = pos_izquierdo;
    } else {
      pos_menor = pos_derecho;
    }

    if (heap->comparador(heap->vector[n], heap->vector[pos_menor]) == 1) {
        swap(heap->vector, n, pos_menor);
        sift_down(heap, pos_menor);
    }
}

int heap_insertar (heap_t* heap, void* elemento) {
    if (!heap) {
      return ERROR;
    }

    void* aux = realloc(heap->vector, sizeof(elemento) * (size_t)(heap->tope + 1));
    if (!aux) {
      return ERROR;
    }

    heap->tope++;
    heap->vector = aux;
    heap->vector[heap->tope - 1] = elemento;

    sift_up(heap, heap->tope - 1);

    return EXITO;
}

void* heap_borrar_raiz (heap_t* heap) {
    if (!heap || heap->tope == 0) {
      return NULL;
    }

    void* valor = heap->vector[0];
    heap->vector[0] = heap->vector[heap->tope - 1];
    heap->tope--;

    if (heap->tope != 0) {
      sift_down(heap, 0);
    }
    return valor;
}

void destruir_heap (heap_t* heap) {
  if (heap->destructor) {
    for (int i = 0; heap->tope > 0; i++) {
      heap->destructor(heap->vector[i]);
      heap->tope--;
    }
  }

  free(heap->vector);
  free(heap);
}

bool heap_vacio (heap_t* heap) {
  return heap->tope == 0;
}

//PRUEBA DE HEAP CON NUMEROS ENTEROS

/*
int main () {
    heap_t* heap = heap_crear(comparar_numeros, NULL);

    heap_insertar(heap, (void*)1);
    heap_insertar(heap, (void*)10);
    heap_insertar(heap, (void*)25);
    heap_insertar(heap, (void*)40);
    heap_insertar(heap, (void*)23);
    heap_insertar(heap, (void*)5);
    heap_insertar(heap, (void*)7);
    heap_insertar(heap, (void*)12);
    heap_insertar(heap, (void*)17);
    heap_insertar(heap, (void*)21);

    for (int i = 0; i < 10; i++) {
      printf("%i ", heap->vector[i]);
    }

    mostrar_arbol(heap->vector, heap->tope);
    printf("Extraigo la raiz (valor %i)\n", heap_borrar_raiz(heap));
    mostrar_arbol(heap->vector, heap->tope);
    printf("Extraigo la raiz (valor %i)\n", heap_borrar_raiz(heap));
    mostrar_arbol(heap->vector, heap->tope);
    printf("Extraigo la raiz (valor %i)\n", heap_borrar_raiz(heap));
    mostrar_arbol(heap->vector, heap->tope);
    printf("Extraigo la raiz (valor %i)\n", heap_borrar_raiz(heap));
    mostrar_arbol(heap->vector, heap->tope);
    printf("Extraigo la raiz (valor %i)\n", heap_borrar_raiz(heap));

    mostrar_arbol(heap->vector,heap->tope);

    destruir_heap(heap);

    return EXITO;

}*/

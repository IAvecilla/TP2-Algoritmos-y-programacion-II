#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
  Se debe implementar la función

  int abb_a_array(abb_t* abb, int* buffer, int n);

  donde abb es un abb, e int* buffer es un bloque de memoria de tamaño n.

  La función debe guardar los valores almacenados en el arbol en el
  bloque de memoria. La representacion es la vista en clase:

  El nodo raiz va en la posicion 0. El hijo izquierdo de un nodo N va en
  2*N+1 y el derecho en 2*N+2.

  Los valores almacenados van de 1 a 99. El valor 0 representa la
  ausencia de nodo.
*/


void mostrar_arbol(int* buffer, int n){
  printf("\n");
  int niveles = 0;
  for(int restantes=n;restantes>0;restantes>>=1)
    niveles++;

  int n_nodo=0;
  for(int nivel=0;nivel<niveles;nivel++){
    int posicion = 1<<(niveles-nivel-1);

    for(int nodo=0;nodo<(1<<nivel);nodo++){
      for(int i=1;i<posicion;i++){
        if(i<=posicion/2-1 || 2*n_nodo+1 >= n || buffer[2*n_nodo+1]==0)
          printf("  ");
        else{
          if(i == posicion/2)
            printf(" ┌");
          else printf("――");
        }
      }

      if(n_nodo<n && buffer[n_nodo]!=0){
        int valor = buffer[n_nodo]<100?buffer[n_nodo]:99;
        printf("%2d", valor);
      }
      else{
        printf("  ");
      }

      for(int i=0;i<posicion;i++){
        if(i>=posicion/2 || 2*n_nodo+2 >= n ||buffer[2*n_nodo+2]==0)
          printf("  ");
        else{
          if(i == posicion/2-1)
            printf("―┐");
          else printf("――");
        }
      }
      n_nodo++;
    }
    printf("\n");
  }
}

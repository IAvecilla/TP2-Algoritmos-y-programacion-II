# Linea de compilacion y ejecucion

Para compilar el juego: gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 juego.c lista.c pokemon.c jugador_principal.c interfaz.c heap.c gimnasio.c entrenador.c batallas.c abb.c -o juego

Para poder ejecutarlo: ./juego

Para ejecutar con valgrind: valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s ./juego



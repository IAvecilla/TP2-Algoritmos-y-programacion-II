VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes 
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

juego: juego.c  lista.c pokemon.c jugador_principal.c interfaz.c heap.c gimnasio.c entrenador.c batallas.c abb.c
	gcc $(CFLAGS) juego.c  lista.c pokemon.c jugador_principal.c interfaz.c heap.c gimnasio.c entrenador.c batallas.c abb.c -o juego

jugar: juego
	./juego

valgrind: juego
	valgrind $(VFLAGS) ./juego
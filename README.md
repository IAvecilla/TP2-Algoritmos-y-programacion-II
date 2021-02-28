# Linea de compilacion y ejecucion

Para compilar el juego: gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 juego.c lista.c pokemon.c jugador_principal.c interfaz.c heap.c gimnasio.c entrenador.c batallas.c abb.c -o juego

Para poder ejecutarlo: ./juego

Para ejecutar con valgrind: valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s ./juego

# Funcionamiento

El juego se desarrolla a travez de una interfaz constituida por menus y ingresos del usuario segun la accion indicada, el usuario tiene la posibilidad de empezar una partida normal o simular una. En ambos casos es obligatorio que antes de empezar el usuario cargue el jugador principal mediante un archivo y al menos un gimnasio, que podra cargar uno por uno con distintos archivos o todos juntos mediante un solo archivo.

Una vez esto el juego deberia comenzar o simularse, en caso de jugarse normalmente el juego lo guia por una serie de menues bien explicativos, si es una simulacion el usuario se informa de si gano o perdio segun los datos iniciales de los archivos.

En un principio se cuentan con varias estructuras, la estructura pokemon contiene el nombre de los pokemon, ataque, defensa y velocidad, ademas tienen un valor booleano indicando si estan vivos o no, y un numero que representa la cantidad de mejoras actuales, teniendo un tope de 63.

La estructura del entrenador, tiene el nombre del entrenador y una lista enlazada que contiene su lista de pokemon disponibles para pelear que son todos los que se leen del archivo, sin limite.

La estructura del gimnasio tiene el nombre del gimnasio, un entero que indica la dificultad del mismo, un entero que representa la funcion de batalla que va a utilizarse para ese gimnasio, el nombre del lider de dicho gimnasio y una pila de entrenadores.

Por ultimo la estructura del personaje principal, tiene su nombre y una lista que representa sus pokemon. En esta lista estan todos los pokemon cargados del archivo, los 6 primeros corresponden a sus pokemond de batalla (party) y los demas pertenecen a la mochila del personaje.

En la estructura de entrenador y de personaje me parecio adecuado usar listas paara sus pokemon, ya que tenia que imprimirlos a todos en cierto momento, entonces debia recorrerlas elemento por elemento. En un primer momento pense en usar colas pero no queria recorrerlas enteramente y perder el sentido de lo que era una cola, y encolar y desencolar todo el tiempo no me parecia efectivo. Para el personaje en si, me parecio que usar una lista para sus pokemon ya era suficiente, facilitaba la posibilidad de intercambiar sus pokemon de party y los de la mochila ademas de ahorrar un poco de memoria en la creacion de una unica lista.Tambien estuvo la idea de usar un arbol para los pokemon de mochila ya que disminuiria el tiempo de busqueda de cada pokemon pero ocurria lo mismo que la cola, necesitaba recorrerlos todos para imprimirlos y para hacer un cambio entre la lista de party y el arbol, era muy complejo de implementar.

En la estructura de gimnasio, decidi usar una pila de sus entrenadores, ya que en el archivo aparece primero el lider del gimnasio, el cual deberia ser enfrentado ultimo, entonces la pila me invierte el orden de lectura y el lider seria el ultimo en batallar, aparte es muy comodo desapilar unicamente para pasar a otro entrenador.








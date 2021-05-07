# TP2-Algoritmos-y-programacion-II

Trabajo práctico final de la materia Algoritmos y Programación 2 de la Universidad de Buenos Aires. <br>
Implementado en lenguaje C, con archivo makefile para la ejecucion del programa <br>
Uso de memoria dinamica y estructuras de datos implementadas a lo largo de la cursada.

# Compilación y Ejecución

Para compilar el juego</br>
<code>make juego</code></br>
Para poder ejecutarlo </br> 
<code>make jugar</code></br>
Tambien se utilizó Valgrind para el control de la memoria dinamica</br>
<code>make valgrind</code></br>

Si se desea, puede saltearse la linea de compilacion, en cualquier caso <code>make jugar</code> o <code>make valgrind</code> compilaran en caso de que no se haya hecho antes.

# Funcionamiento

El juego se desarrolla a través de una interfaz constituida por menús e ingresos del usuario segun la accion indicada, el usuario tiene la posibilidad de empezar una partida normal o simular una. En ambos casos es obligatorio que antes de empezar el usuario cargue el jugador principal mediante un archivo y al menos un gimnasio, que podra cargar uno por uno con distintos archivos o todos juntos mediante un solo archivo

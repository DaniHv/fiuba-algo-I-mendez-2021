# Info

Enunciado del TP en ./TP2 - Enunciado.pdf. Compilar en Linux (dependencia utiles.o compilada para linux).

# Agregados propios

-   Es posible iniciar el juego sin la necesidad de realizar el test de personalidad, para ello se ejecuta el juego con el argumento del personaje que se quiere jugar. Es decir, en lugar de ejecutar ./juego, ejecutamos ./juego panda, ./juego pardo, ./juego polar según sea el caso.

-   Al finalizar la partida, nos da la opción de jugar de nuevo, guardando nuestra elección de personaje e iniciando el juego sin necesidad de volver a ejecutar. En caso queramos jugar nuevamente presionamos S, caso contrario presionamos ESC. (Desactivado para evitar problemas de ejecución en Chanutron)

-   En las jugadas válidas tenemos la opción de "ver ayuda", con la tecla H. Que nos dice todos los movimientos y jugadas posibles del juego. A su vez este mensaje aparece al inicio del juego. (Desactivado el delay para Chanutron)

-   Es posible reiniciar el juego sin haberlo terminado, presionando la tecla X. Esto reiniciará el juego y empezaremos jugando con el mismo personaje previamente elegido/resultante.

-   Para debuguear el mapa y los elementos en él, podemos hacer true la constante DEBUG_MAPA en osos_contra_reloj.c

# Notas adicionales casi insignificantes

-   Denominé "bonificaciones" a las ventajas que tienen ciertos personajes. Y los calculé de manera porcentual. Cumple la condición de la cátedra, pero lo hice de esta manera "más difícil", pensando en "si se modifica la suma base, que se haga automáticamente lo mismo para las bonificaciones" :).

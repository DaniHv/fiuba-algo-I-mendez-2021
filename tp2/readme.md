## Agregados propios

-   Es posible iniciar el juego sin la necesidad de realizar el test de personalidad, para ello se ejecuta el juego con el argumento del personaje que se quiere jugar. Es decir, en lugar de ejecutar ./juego, ejecutamos ./juego panda, ./juego pardo, ./juego polar según sea el caso.

-   Al finalizar la partida, nos da la opción de jugar de nuevo, guardando nuestra elección de personaje e iniciando el juego sin necesidad de volver a ejecutar. En caso queramos jugar nuevamente presionamos S, caso contrario presionamos ESC. (Desactivado para evitar problemas de ejecución en Chanutron)

-   En las jugadas válidas tenemos la opción de "ver ayuda", con la tecla H. Que nos dice todos los movimientos y jugadas posibles del juego. A su vez este mensaje aparece al inicio del juego.

-   Es posible reiniciar el juego sin haberlo terminado, presionando la tecla X. Esto reiniciará el juego y empezaremos jugando con el mismo personaje previamente elegido/resultante.

-   Para debuguear el mapa y los elementos en él, podemos hacer true la constante DEBUG_MAPA en osos_contra_reloj.c

## Puntos de mejora en el enunciado del tp:

### Tener la posibilidad de añadir constantes en el .h

Muchas de las constantes que utilizamos, se usan más de un archivo, lo que nos hace tener que repetirlas en todos los archivos en los que queramos utilizarlas.

### Visualización de elementos del mapa (Por herramientas):

La manera en la que se encuentra diseñado el struct del juego, así como también la función mostrar_juego ocasiona problemas con la visualización de los elementos en el mapa cuando se utiliza la bengala. Si bien se puede cumplir con las pautas del enunciado, resulta dificil hacer de una manera "normal" y a la vez con buenas prácticas la lógica correspondiente a la visualización de espacios vacíos (PERO ILUMINADOS) en el mapa con la bengala activada, ya que la modificación de la visualización de los obstaculos, herramientas y chloe debe ser realizada en realizar_jugada y no en mostrar_juego (Ya que este último recibe el struct juego por valor y no por referencia). Esto hace que tengamos que generar la posición aleatoria en alguna instancia de esta función y no podemos guardarla en el struct juego o algún otro lugar (Salvo variables globales, que no son buena práctica) para poder "recordarla" cuando vamos a mostrar el mapa e iluminar los espacios vacíos, puesto que estos no se encuentran en el juego, sino que los representamos únicamente al momento de crear nuestra matriz en mostrar_juego.

Posible oportunidad de solucionar el problema: Pasar el struct por referencia a mostrar_juegos, y ejecutar toda la lógica en este lugar tanto para obstáculos, herramientas, chloe así como también los elementos vacíos; o de una manera más "drástica", eliminar la necesidad del bool de visibilidad, y encargar la tarea al mostrar_juego, de manera que es posible usar la lógica en un único lugar, y trabajando sobre la matriz que creamos.

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDCYAN    "\033[1m\033[36m"

#define DELAY_CORTO 1
#define DELAY_MEDIO 5
#define DELAY_LARGO 10

const bool DEBUG_MAPA = false; // Al ser true, permite jugar con el mapa totalmente visible.

#define CHLOE 'C'
#define ARBOL 'A'
#define PIEDRA 'R'
#define KOALA 'K'
#define PILA 'B'

#define OBSTACULO 'O'
#define HERRAMIENTA 'H'

#define ARRIBA 'W'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define DERECHA 'D'

#define LINTERNA 'L'
#define BENGALA 'E'
#define VELA 'V'
#define TIEMPO 'T'
#define AYUDA 'H'
#define REINICIAR 'X'

#define ANCHO_MAPA 30
#define ALTO_MAPA 20

const static double TIEMPO_MAXIMO = 120.00;

const int MOVIMIENTOS_LINTERNA = 10;
const int MOVIMIENTOS_PILA = 5;
const double BONIFICACION_LINTERNA_PARDO = 50.0;

const int CANTIDAD_VELAS_MOCHILA = 4;
const int MOVIMIENTOS_VELA = 5;
const double BONIFICACION_VELAS_POLAR = 50.0;

const int MOVIMIENTOS_BENGALA = 3;
const int BONIFICACION_BENGALAS_PANDA = 2;

const double PENALIZACION_PIEDRA = 2.00;

const double PENALIZACION_ARBOL = 1.00;
const double BONIFICACION_ARBOL_PARDO = 50.0;

const int NULO = -1;
const static char VACIO = '-';

const int CANTIDAD_ARBOLES_MAPA = 350;
const int CANTIDAD_PIEDRAS_MAPA = 80;
const int CANTIDAD_KOALAS_MAPA = 1;
const int CANTIDAD_PILAS_MAPA = 30;
const int CANTIDAD_VELAS_MAPA = 30;
const int CANTIDAD_BENGALAS_MAPA = 10;
const int CANTIDAD_KOALAS_INVOCAR = 1;

const int JUGANDO = 0;
const int FINALIZADO = NULO;

const int MIN_TIEMPO_VISIBILIDAD_PANDA = 30;

const int MAX_DISTANCIA_BENGALA = 3;

const char EMOJI_OSCURIDAD[20] = "\U00002B1B";
const char EMOJI_CLARIDAD[20] = "\U00002B1C";
const char EMOJI_PANDA[20] = "\U0001F43C";
const char EMOJI_POLAR[20] = "\U0001F43B\U0000200D\U00002744\U0000FE0F";
const char EMOJI_PARDO[20] = "\U0001F43B";
const char EMOJI_LINTERNA[20] = "\U0001F526";
const char EMOJI_PILA[20] = "\U0001F50B";
const char EMOJI_ARBOL[20] = "\U0001F332";
const char EMOJI_PIEDRA[20] = "\U0001FAA8";
const char EMOJI_KOALA[20] = "\U0001F428";
const char EMOJI_CHLOE[20] = "\U0001F467";
const char EMOJI_VELA[20] = "\U0001F56F ";
const char EMOJI_BENGALA[20] = "\U0001F9E8";


/*
 * Pre: -
 * Post: Imprimirá por pantalla una línea separadora.
 */
void static mostrar_separador() {
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
}

/*
 * Pre: Puede recibir una fila o columna específica a mantener, caso contrario enviar 'NULO'.
 * Post: Genera una posición aleatoria en el mapa, en caso de haber recibido fila o columna, la conserva y genera la componente faltante.
 */
coordenada_t generar_posicion_aleatoria(int fila, int columna) {
    coordenada_t posicion;

    posicion.fil = (fila != NULO) ? fila : rand() % ALTO_MAPA;
    posicion.col = (columna != NULO) ? columna : rand() % ANCHO_MAPA;

    return posicion;
}

/*
 * Pre: Debe enviar una herramienta válida (LINTERNA, VELA, BENGALA).
 * Post: Busca la herramienta solicitada en la mochila, en caso encuentre, devuelve la posición de la primera de ellas.
 */
int buscar_en_mochila(personaje_t personaje, char herramienta) {
    int posicion = NULO;
    int i = 0;

    while((i < personaje.cantidad_elementos) && (posicion == NULO)) {
        if(personaje.mochila[i].tipo == herramienta) {
            posicion = i;
        }
        i++;
    }

    return posicion;
}

/*
 * Pre: Debe enviar un tipo válido de herramienta (LINTERNA, VELA, BENGALA o PILA) y una cantidad total válida (>= 1).
 * Post: Agrega los elementos a la mochila con los usos según el personaje. En caso de ser PILA, solo suma los usos a LINTERNA.
 * Nota: Al recibir cantidad_total, puede ser usada tanto para inicializar, como para agregar al recoger.
 */
void agregar_herramienta_mochila(personaje_t* personaje, char tipo, int cantidad_total) {
    int movimientos = 0;

    switch (tipo) {
        case LINTERNA:
            if (personaje->tipo == PARDO) {
                movimientos = MOVIMIENTOS_LINTERNA + ((int) ((BONIFICACION_LINTERNA_PARDO / 100) * MOVIMIENTOS_LINTERNA));
            } else {
                movimientos = MOVIMIENTOS_LINTERNA;
            }
            break;

        case VELA:
            movimientos = MOVIMIENTOS_VELA;
            break;

        case BENGALA:
            movimientos = MOVIMIENTOS_BENGALA;
            break;

        case PILA:
            movimientos = MOVIMIENTOS_PILA;
            break;
    }

    for (int i = 0; i < cantidad_total; i++) {
        if (tipo != PILA) {
            personaje->mochila[personaje->cantidad_elementos].tipo = tipo;
            personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = movimientos;

            personaje->cantidad_elementos ++;
        } else {
            int posicion_linterna = buscar_en_mochila((*personaje), LINTERNA);

            personaje->mochila[posicion_linterna].movimientos_restantes += movimientos;
        }
    }
}

/*
 * Pre:
 * Post: Posiciona al personaje elegido en una posición aleatoria en la primera columna e inicializa los valores (Tiempo perdido 0, sin elemento en uso y ultimo movimiento derecha).
 *       Llena la mochila, todos con 1 linterna y con la cantidad de velas CANTIDAD_VELAS (Con + BONIFICACION_VELAS_POLAR% para POLAR) y BONIFICACION_BENGALAS_PANDA para PANDA.
 */
void inicializar_personaje(personaje_t* personaje, char tipo_personaje) {
    coordenada_t posicion = generar_posicion_aleatoria(NULO, 0);

    personaje->tipo = tipo_personaje;
    personaje->posicion = posicion;
    personaje->elemento_en_uso = NULO;
    personaje->tiempo_perdido = 0;
    personaje->cantidad_elementos = 0;
    personaje->ultimo_movimiento = DERECHA;

    switch(tipo_personaje) {
        case PANDA:
            agregar_herramienta_mochila(personaje, LINTERNA, 1);
            agregar_herramienta_mochila(personaje, VELA, CANTIDAD_VELAS_MOCHILA);
            agregar_herramienta_mochila(personaje, BENGALA, BONIFICACION_BENGALAS_PANDA);
            break;

        case PARDO:
            agregar_herramienta_mochila(personaje, LINTERNA, 1);
            agregar_herramienta_mochila(personaje, VELA, CANTIDAD_VELAS_MOCHILA);
            break;

        case POLAR:
            agregar_herramienta_mochila(personaje, LINTERNA, 1);
            agregar_herramienta_mochila(personaje, VELA, (int) (CANTIDAD_VELAS_MOCHILA + ((BONIFICACION_VELAS_POLAR / 100) * CANTIDAD_VELAS_MOCHILA)));
            break;
    }
}

/*
 * Pre:
 * Post: Posiciona a chloe en una posición aleatora a lo largo del mapa,
 *       diferente a la posición inicial del personaje.
 */
void posicionar_chloe(juego_t* juego) {
    coordenada_t posicion = generar_posicion_aleatoria(NULO, NULO);

    while((juego->personaje.posicion.fil == posicion.fil) && (juego->personaje.posicion.col == posicion.col)) {
        posicion = generar_posicion_aleatoria(NULO, NULO);
    }

    juego->amiga_chloe = posicion;
}

/*
 * Pre: Debe recibir un tipo de elemento VÁLIDO (OBSTACULO o HERRAMIENTA) a buscar.
 * Post: Devuelve el tipo de elemento ubicado en la posición recibida,
 *       en caso no encuentre ninguna, devolverá VACIO
 */
char detectar_elemento(juego_t juego, char tipo_elemento, coordenada_t posicion) {
    char elemento = VACIO;
    int i = 0;

    switch (tipo_elemento) {
        case OBSTACULO:
            while((i < juego.cantidad_obstaculos) && (elemento == VACIO)) {
                if((juego.obstaculos[i].posicion.fil == posicion.fil) && (juego.obstaculos[i].posicion.col == posicion.col)) {
                    elemento = juego.obstaculos[i].tipo;
                }

                i++;
            }
            break;

        case HERRAMIENTA:
            while((i < juego.cantidad_herramientas) && (elemento == VACIO)) {
                if((juego.herramientas[i].posicion.fil == posicion.fil) && (juego.herramientas[i].posicion.col == posicion.col)) {
                    elemento = juego.herramientas[i].tipo;
                }

                i++;
            }
            break;
    }

    return elemento;
}

/*
 * Pre: -
 * Post: Detecta si hay un obstáculo, herramienta, el personaje o chloe en la posición dada,
 *       si está vacía devuelve TRUE, caso contrario FALSE.
 */
bool posicion_valida(juego_t juego, coordenada_t posicion) {
    char obstaculo = detectar_elemento(juego, OBSTACULO, posicion);
    char herramienta = detectar_elemento(juego, HERRAMIENTA, posicion);

    return (
        !((juego.personaje.posicion.fil == posicion.fil) && (juego.personaje.posicion.col == posicion.col)) &&
        !((juego.amiga_chloe.fil == posicion.fil) && (juego.amiga_chloe.col == posicion.col)) &&
        (obstaculo == VACIO) &&
        (herramienta == VACIO)
    );
}

/*
 * Pre: Debe recibir un tipo de elemento válido: (OBSTACULO, HERRAMIENTA), un elemento válido:
 *      (ARBOL, PIEDRA, KOALA para OBSTACULO y PILA, VELA, BENGALA para HERRAMIENTA).
 *      Y tambien una cantidad válida (>= 1).
 * Post: Posiciona la cantidad de obstáculos solicitada en juego, asegurandose
 *       de colocarlos en una posición vacía mediante posicion_valida().
 */
void posicionar_elementos(juego_t* juego, char tipo_elemento, char elemento, int cantidad) {
    for(int i = 0; i < cantidad; i++) {
        coordenada_t posicion = generar_posicion_aleatoria(NULO, NULO);

        while(!posicion_valida((*juego), posicion)) {
            posicion = generar_posicion_aleatoria(NULO, NULO);
        }

        switch (tipo_elemento) {
            case OBSTACULO:
                juego->obstaculos[juego->cantidad_obstaculos].tipo = elemento;
                juego->obstaculos[juego->cantidad_obstaculos].posicion = posicion;
                juego->obstaculos[juego->cantidad_obstaculos].visible = false;
                juego->cantidad_obstaculos ++;
                break;
            
            case HERRAMIENTA:
                juego->herramientas[juego->cantidad_herramientas].tipo = elemento;
                juego->herramientas[juego->cantidad_herramientas].posicion = posicion;
                juego->herramientas[juego->cantidad_herramientas].visible = false;
                juego->cantidad_herramientas ++;
                break;
        }
    }
}

/*
 * Pre: -
 * Post: Muestra al usuario todas las acciones que puede realizar: movimientos, herramientas, otros.
 */
void mostrar_ayuda() {
    system("clear");

    mostrar_separador();
    printf("Para moverte a través del mapa debes presionar: \n");
    printf("[" BOLDGREEN "%c" RESET "] Arriba. \n", ARRIBA);
    printf("[" BOLDGREEN "%c" RESET "] Izquierda. \n", IZQUIERDA);
    printf("[" BOLDGREEN "%c" RESET "] Abajo. \n", ABAJO);
    printf("[" BOLDGREEN "%c" RESET "] Derecha. \n", DERECHA);

    printf("\n");

    printf("A lo largo den mapa tendrás los siguientes obstáculos: \n");
    printf("(%s) Árboles - Suman %.0f %s a tu tiempo perdido. \n", EMOJI_ARBOL, PENALIZACION_ARBOL, ((PENALIZACION_ARBOL == 1) ? "segundo" : "segundos"));
    printf("(%s) Piedras - Suman %.0f %s a tu tiempo perdido. \n", EMOJI_PIEDRA, PENALIZACION_PIEDRA, ((PENALIZACION_PIEDRA == 1) ? "segundo" : "segundos"));
    printf("(%s) Koalas - Te envían a la primera columna del mapa. \n", EMOJI_KOALA);

    printf("\n");

    printf("Sobre tu mochila y las herramientas: \n");
    printf("(%s) [" BOLDGREEN "%c" RESET "] Linterna - Ilumina todo el bosque que tengas en frente. \n", EMOJI_LINTERNA, LINTERNA);
    printf("(%s) [" BOLDGREEN "%c" RESET "] Vela - Ilumina tu alrededor. \n", EMOJI_VELA, VELA);
    printf("(%s) [" BOLDGREEN "%c" RESET "] Bengala - Ilumina donde quiere en un radio de %i posiciones. \n", EMOJI_BENGALA, BENGALA, MAX_DISTANCIA_BENGALA);
    printf("(%s) Pila - Te suma %i movimientos a tu linterna \n", EMOJI_PILA, MOVIMIENTOS_PILA);

    printf("\n");

    printf("Tienes límite de %.0f segundos para conseguir a Chloe! Si quieres ver tu tiempo actual presiona: (%c) \n" RESET, TIEMPO_MAXIMO, TIEMPO);

    printf("\n");

    printf("Si quieres reiniciar el juego, presiona (" BOLDGREEN "%c" RESET ") \n", REINICIAR);
    mostrar_separador();

    // sleep(DELAY_LARGO);
}

/*
 * Pre: Debe recibir un tipo de personaje válido (PANDA, PARDO, POLAR)
 * Post: Empezará el juego con todos los valores iniciales según el personaje elegido: personaje/mochila, posiciona obstáculos y herramientas en el mapa,
 *       muestra el mensaje de ayuda inicial y da inicio al cronómetro.
 */
void inicializar_juego(juego_t* juego, char tipo_personaje) {
    inicializar_personaje((&juego->personaje), tipo_personaje);
    posicionar_chloe(juego);
    juego->chloe_visible = false;

    juego->cantidad_obstaculos = 0;
    posicionar_elementos(juego, OBSTACULO, ARBOL, CANTIDAD_ARBOLES_MAPA);
    posicionar_elementos(juego, OBSTACULO, PIEDRA, CANTIDAD_PIEDRAS_MAPA);
    posicionar_elementos(juego, OBSTACULO, KOALA, CANTIDAD_KOALAS_MAPA);

    juego->cantidad_herramientas = 0;
    posicionar_elementos(juego, HERRAMIENTA, PILA, CANTIDAD_PILAS_MAPA);
    posicionar_elementos(juego, HERRAMIENTA, VELA, CANTIDAD_VELAS_MAPA);
    posicionar_elementos(juego, HERRAMIENTA, BENGALA, CANTIDAD_BENGALAS_MAPA);

    mostrar_ayuda();

    iniciar_cronometro();
}

/*
 * Pre: -
 * Post: Detecta si el juego está finalizado si se llegó a chloe, caso afirmativo devuelve TRUE, contrario FALSE.
 */
int estado_juego(juego_t juego) {
    int estado = JUGANDO;

    if (juego.personaje.posicion.col == juego.amiga_chloe.col && juego.personaje.posicion.fil == juego.amiga_chloe.fil) {
        estado = FINALIZADO;
    }

    return estado;
};

/*
 * Pre: Debe recibir una herramienta válida (PILA, VELA o BENGALA)
 * Post: Eliminará la herramienta del piso y la guardará en la mochila. En caso de ser una pila, sumará USOS_LINTERNA_PILA a la linterna.
 */
void recoger_herramienta(juego_t* juego) {
    char herramienta = detectar_elemento((*juego), HERRAMIENTA, juego->personaje.posicion);

    if (herramienta != VACIO) {
        for(int i = 0; i < juego->cantidad_herramientas; i++) {
            if ((juego->personaje.posicion.fil == juego->herramientas[i].posicion.fil) && (juego->personaje.posicion.col == juego->herramientas[i].posicion.col)) {
                juego->cantidad_herramientas --;
                juego->herramientas[i] = juego->herramientas[juego->cantidad_herramientas];
            }
        }

        switch (herramienta) {
            case PILA:
                agregar_herramienta_mochila((&juego->personaje), herramienta, 1);
                printf("Has encontrado una Pila %s! Sumaste %i movimientos a tu linterna. \n", EMOJI_PILA, MOVIMIENTOS_PILA);
                break;

            case VELA:
                agregar_herramienta_mochila((&juego->personaje), herramienta, 1);
                printf("Has encontrado una Vela %s! \n", EMOJI_VELA);
                break;

            case BENGALA:
                agregar_herramienta_mochila((&juego->personaje), herramienta, 1);
                printf("Has encontrado una Bengala %s! \n", EMOJI_BENGALA);
                break;
        }

        // sleep(DELAY_CORTO);
    }
}

/*
 * Pre: Debe ser ejecuta cuando se tiene una herramienta en uso.
 * Post: Disminuye un movimiento a la herramienta en uso, si llega a cero, elimina la herramienta.
 */
void desgastar_herramienta(personaje_t* personaje) {
    if (personaje->elemento_en_uso != NULO) {
        if (personaje->mochila[personaje->elemento_en_uso].movimientos_restantes == 0) {
            if (personaje->mochila[personaje->elemento_en_uso].tipo != LINTERNA) {
                personaje->cantidad_elementos --;
                personaje->mochila[personaje->elemento_en_uso] = personaje->mochila[personaje->cantidad_elementos];
            }
            personaje->elemento_en_uso = NULO;

            printf("Se han acabado los movimientos de tu herramienta en uso. \n");
        } else {
            personaje->mochila[personaje->elemento_en_uso].movimientos_restantes --;
        }
    }
}

/*
 * Pre: Se debe recibir posiciones válidas del mapa para tener un resultado correcto.
 * Post: Determina si una posicion es adyacente a la posicion de referencia.
 */
bool posicion_adyacente(coordenada_t posicion_referencia, coordenada_t posicion_evaluar) {
    return (
        ((posicion_referencia.fil - 1 <= posicion_evaluar.fil) && (posicion_referencia.fil + 1 >= posicion_evaluar.fil)) &&
        ((posicion_referencia.col - 1 <= posicion_evaluar.col) && (posicion_referencia.col + 1 >= posicion_evaluar.col))
    );
}

/*
 * Pre: Se debe recibir posiciones válidas del mapa para tener un resultado correcto.
 * Post: Determina la distancia manhattan entre dos puntos y la devuelve.
 */
int distancia_manhattan(coordenada_t posicion_referencia, coordenada_t posicion_evaluar) {
    return (abs(posicion_referencia.fil - posicion_evaluar.fil) + abs(posicion_referencia.col - posicion_evaluar.col));
}


/*
 * Pre: Se debe recibir posiciones válidas del mapa para tener un resultado correcto.
 * Post: Determina si una posición a evaluar debe ser visible (TRUE) o no (FALSE),
 *       según la herramienta que tenga activada el personaje, su último movimiento y posicion_referencia
 *       (A usar como posición actual del personaje en Linterna y Vela, y diferente en Bengala).
 */
bool es_visible(char herramienta_en_uso, char ultimo_movimiento, coordenada_t posicion_referencia, coordenada_t posicion_evaluar) {
    bool visible = false;

    switch (herramienta_en_uso) {
        case LINTERNA:
            switch(ultimo_movimiento) {
                case ARRIBA:
                    if ((posicion_referencia.col == posicion_evaluar.col) && posicion_referencia.fil > posicion_evaluar.fil) {
                        visible = true;
                    }
                    break;
                
                case IZQUIERDA:
                    if ((posicion_referencia.fil == posicion_evaluar.fil) && posicion_referencia.col > posicion_evaluar.col) {
                        visible = true;
                    }
                    break;

                case ABAJO:
                    if ((posicion_referencia.col == posicion_evaluar.col) && posicion_referencia.fil < posicion_evaluar.fil) {
                        visible = true;
                    }
                    break;

                case DERECHA:
                    if ((posicion_referencia.fil == posicion_evaluar.fil) && posicion_referencia.col < posicion_evaluar.col) {
                        visible = true;
                    }
                    break;
            }
            break;

        case VELA:
            visible = posicion_adyacente(posicion_referencia, posicion_evaluar);
            break;

        case BENGALA:
            visible = distancia_manhattan(posicion_referencia, posicion_evaluar) <= MAX_DISTANCIA_BENGALA;
            break;
    }

    return visible;
}

/*
 * Pre: -
 * Post: Actualiza la visibilidad de los obstáculos, herramientas y chloe si está en uso alguna herramienta,
 *       detectando si la posicion en cuestión es visible con es_visible().
 */
void modificar_visibilidad(juego_t* juego) {
    char herramienta_en_uso = juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo;
    coordenada_t posicion_referencia = (herramienta_en_uso == BENGALA) ? generar_posicion_aleatoria(NULO, NULO) : juego->personaje.posicion;

    for (int i = 0; i < juego->cantidad_obstaculos; i++) {
        juego->obstaculos[i].visible = es_visible(herramienta_en_uso, juego->personaje.ultimo_movimiento, posicion_referencia, juego->obstaculos[i].posicion);
    }

    for (int i = 0; i < juego->cantidad_herramientas; i++) {
        juego->herramientas[i].visible = es_visible(herramienta_en_uso, juego->personaje.ultimo_movimiento, posicion_referencia, juego->herramientas[i].posicion);
    }

    juego->chloe_visible = es_visible(herramienta_en_uso, juego->personaje.ultimo_movimiento, posicion_referencia, juego->amiga_chloe);
}

/*
 * Pre: -
 * Post: Penaliza al jugador si está sobre un obstáculo, sumando al tiempo perdido. E informa al usuario con un mensaje.
 *       ARBOL -> Suma al tiempo perdido PENALIZACION_ARBOL (Si es Pardo, - BONIFICACION_ARBOL_PARDO%)
 *       PIEDRA -> Suma al tiempo perdido PENALIZACION_PIEDRA (Si es Polar, no suma nada)
 *       KOALA -> Envía a columna 0 en una posición aleatoria
 */
void penalizar_obstaculo(juego_t* juego) {
    char obstaculo = detectar_elemento((*juego), OBSTACULO, juego->personaje.posicion);

    if (obstaculo != VACIO) {
        double penalizacion = 0;

        switch (obstaculo) {
            case ARBOL:
                if (juego->personaje.tipo == PARDO) {
                    penalizacion = PENALIZACION_ARBOL - ((BONIFICACION_ARBOL_PARDO / 100) * PENALIZACION_ARBOL);
                    printf("Bah, ¿viste venir ese árbol %s? Al menos eres Pardo y solo perdiste %.0f %s de tiempo! \n", EMOJI_ARBOL, penalizacion, (penalizacion == 1 ? "segundo" : "segundos"));
                } else {
                    penalizacion = PENALIZACION_ARBOL;
                    printf("Bah, ¿viste venir ese árbol %s? Has perdido %.0f %s de tiempo :( \n", EMOJI_ARBOL, penalizacion, (penalizacion == 1 ? "segundo" : "segundos"));
                }
                break;

            case PIEDRA:
                if (juego->personaje.tipo == POLAR) {
                    printf("Las piedras %s no pueden contra Polar, sigamos nuestro camino :) \n", EMOJI_PIEDRA);
                } else {
                    penalizacion = PENALIZACION_PIEDRA;
                    printf("Las piedras son enemigas no amigas %s! Te hizo perder %.0f %s de tiempo :( \n", EMOJI_PIEDRA, penalizacion, (penalizacion == 1 ? "segundo" : "segundos"));
                }
                break;

            case KOALA:
                juego->personaje.ultimo_movimiento = DERECHA;
                juego->personaje.posicion = generar_posicion_aleatoria(NULO, 0);

                printf("Los Koalas %s son una desgracia para nuestra misión! Empezamos desde la columna inicial :( \n", EMOJI_KOALA);

                recoger_herramienta(juego);
                penalizar_obstaculo(juego);

                break;
        }

        juego->personaje.tiempo_perdido += penalizacion;

        // sleep(DELAY_CORTO);
    }
}

/*
 * Pre: Debe recibir un movimiento previamente validado (ARRIBA, IZQUIERDA, ABAJO, DERECHA).
 * Post: Detecta si el movimiento realizado es válido (no puede salir del mapa de juego), si lo es, realiza el movimiento
 *       y detecta si recoge una herramienta o es penalizado por un obstáculo, y si tiene herramienta en uso, descuenta
 *       movimiento y actualiza la visibilidad del mapa.
 *       Si no es válido, informa al usuario que hizo un movimiento inválido.
 */
void hacer_movimiento(juego_t* juego, char jugada) {
    bool movimiento_valido = false;

    switch (jugada) {
        case ARRIBA:
            if (juego->personaje.posicion.fil != 0) {
                juego->personaje.posicion.fil --;
                movimiento_valido = true;
            }
            break;

        case IZQUIERDA:
            if (juego->personaje.posicion.col != 0) {
                juego->personaje.posicion.col --;
                movimiento_valido = true;
            }
            break;

        case ABAJO:
            if ((juego->personaje.posicion.fil + 1) != ALTO_MAPA) {
                juego->personaje.posicion.fil ++;
                movimiento_valido = true;
            }
            break;

        case DERECHA:
            if ((juego->personaje.posicion.col + 1) != ANCHO_MAPA) {
                juego->personaje.posicion.col ++;
                movimiento_valido = true;
            }
            break;
    }

    if (movimiento_valido) {
        juego->personaje.ultimo_movimiento = jugada;

        recoger_herramienta(juego);
        penalizar_obstaculo(juego);
    } else {
        printf("No puedes salirte del mapa :( \n");
    }
}


/*
 * Pre: Recibe una herramienta válida (LINTERNA, VELA o BENGALA).
 * Post: Si tiene herramienta activa la cambia por la deseada (excepto BENGALA, que no se puede cambiar hasta que se acabe), o la desactiva en caso de ingresar la misma que ya tenía.
 *       Si no tiene herramienta activa o cambia por una nueva, se busca en la mochila, si tiene la empieza a utilizar, gasta un uso y modifica la visibilidad de los elementos.
 */
void usar_herramienta(juego_t* juego, char herramienta) {
    if (juego->personaje.elemento_en_uso != NULO) {
        if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA) {
            printf("No puedes dejar de usar tu bengala antes de que se acabe! \n");
        } else {
            if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == herramienta) {
                juego->personaje.elemento_en_uso = NULO;
                printf("Has dejado de usar tu herramienta. \n");
            } else {
                juego->personaje.elemento_en_uso = NULO;
                usar_herramienta(juego, herramienta);
            }
        };
    } else {
        int posicion_herramienta = buscar_en_mochila(juego->personaje, herramienta);

        if (posicion_herramienta != NULO) {
            switch (herramienta) {
                case LINTERNA:
                    if (juego->personaje.mochila[posicion_herramienta].movimientos_restantes > 0) {
                        posicionar_elementos(juego, OBSTACULO, KOALA, 1);
                        juego->personaje.elemento_en_uso = posicion_herramienta;
                        printf("¡Empezaste a usar tu linterna! \n");
                    } else {
                        printf("No te quedan movimientos a tu linterna, consigue más pilas! \n");
                    }
                    break;

                case VELA:
                    juego->personaje.elemento_en_uso = posicion_herramienta;
                    printf("Empezaste a usar una Vela, ¡hágase la luz a tu al rededor! \n");
                    break;

                case BENGALA:
                    juego->personaje.elemento_en_uso = posicion_herramienta;
                    printf("¿Una bengala que ilumina en donde quiera? No es lo que esperaba pero estoy satisfecho. \n");
                    break;
            }
        } else {
            printf("No puedes usar herramientas que no tienes :( \n");
        }
    }
}

/*
 * Pre: -
 * Post: Muestra al usuario su tiempo total actual (tiempo actual + tiempo perdido). En caso su tiempo sea mayor al máximo para ganar, se le da la opción de reiniciar el juego.
 */
void mostrar_tiempo(double tiempo_perdido) {
    double tiempo = tiempo_actual() + tiempo_perdido;

    if (tiempo < TIEMPO_MAXIMO) {
        printf("Tu tiempo actual es de %f \n", tiempo);
    } else {
        printf("Has excedido el tiempo máximo de %.0f segundos, llevas %.0f. Puedes seguir jugando o puedes presionar (%c) para reiniciar el juego. \n", TIEMPO_MAXIMO, tiempo, REINICIAR);
    }
}

/*
 * Pre: -
 * Post: Reinicia nuevamente el juego creando todo desde 0, con el mismo personaje.
 */
void reiniciar_juego(juego_t* juego) {
    detener_cronometro();

    system("clear");
    mostrar_separador();
    printf("Vamos a reiniciar el juego! \n");
    mostrar_separador();
    printf("\n");

    inicializar_juego(juego, juego->personaje.tipo);
}

/*
 * Pre: -
 * Post: Detecta la jugada/movimiento hecha por el usuario y ejecuta el movimiento, uso de herramienta, muestra tiempo, ayuda o reinicia el juego según sea el caso solicitado.
 *       Además, si el personaje es PANDA, detecta cuando puede empezar a ver a Chloe
 */
void realizar_jugada(juego_t* juego, char jugada) {
    if(jugada == ARRIBA || jugada == IZQUIERDA || jugada == ABAJO || jugada == DERECHA) {
        hacer_movimiento(juego, jugada);
    } else if (jugada == LINTERNA || jugada == VELA || jugada == BENGALA) {
        usar_herramienta(juego, jugada);
    } else if (jugada == TIEMPO) {
        mostrar_tiempo(juego->personaje.tiempo_perdido);
    } else if (jugada == AYUDA) {
        mostrar_ayuda();
    } else if (jugada == REINICIAR) {
        reiniciar_juego(juego);
    }

    desgastar_herramienta((&juego->personaje));
    modificar_visibilidad(juego);

    if (juego->personaje.tipo == PANDA && juego->personaje.tiempo_perdido >= MIN_TIEMPO_VISIBILIDAD_PANDA) {
        juego->chloe_visible = true;
    };
};

/*
 * Pre: -
 * Post: Llena la matriz con todos los elementos visibles: obstaculos, herramientas, chloe y el personaje.
 */
void llenar_mapa(juego_t juego, char mapa[ALTO_MAPA][ANCHO_MAPA]) {
    for (int i = 0; i < ALTO_MAPA; i++) {
        for (int j = 0; j < ANCHO_MAPA; j++) {
            mapa[i][j] = VACIO;
        }
    }

    for (int i = 0; i < juego.cantidad_obstaculos; i++) {
        if (juego.obstaculos[i].visible || DEBUG_MAPA) {
            mapa[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].tipo;
        }
    }

    for (int i = 0; i < juego.cantidad_herramientas; i++) {
        if (juego.herramientas[i].visible || DEBUG_MAPA) {
            mapa[juego.herramientas[i].posicion.fil][juego.herramientas[i].posicion.col] = juego.herramientas[i].tipo;
        }
    }

    if (juego.chloe_visible || DEBUG_MAPA) {
        mapa[juego.amiga_chloe.fil][juego.amiga_chloe.col] = CHLOE;
    }

    mapa[juego.personaje.posicion.fil][juego.personaje.posicion.col] = juego.personaje.tipo;
}

/*
 * Pre: Debe recibir el mapa previamente lleno de los elementos que son visibles.
 * Post: Muestra al usuario el mapa de juego, con todos los elementos que puede ver, y su posición actual.
 */
void mostrar_mapa(juego_t juego, char mapa[ALTO_MAPA][ANCHO_MAPA]) {
    mostrar_separador();
    printf(BOLDGREEN "Mapa del juego: \n" RESET);
    mostrar_separador();
    
    printf("   ");
    for (int i = 0; i < ANCHO_MAPA; i++) {
        printf("%2d ", (int) i + 1);
    }
    printf("\n");

    for (int i = 0; i < ALTO_MAPA; i++) {
        printf("%2d ", (int) i + 1);

        for (int j = 0; j < ANCHO_MAPA; j++) {
            coordenada_t posicion = { .fil = i, .col = j };
            char herramienta_en_uso = juego.personaje.mochila[juego.personaje.elemento_en_uso].tipo;
            char emoji[20] = "";
            strcpy(emoji, EMOJI_OSCURIDAD);

            switch (mapa[i][j]) {
                case CHLOE:
                    strcpy(emoji, EMOJI_CHLOE);
                    break;

                case PANDA:
                    strcpy(emoji, EMOJI_PANDA);
                    break;
                
                case PARDO:
                    strcpy(emoji, EMOJI_PARDO);
                    break;
                
                case POLAR:
                    strcpy(emoji, EMOJI_POLAR);
                    break;

                case ARBOL:
                    strcpy(emoji, EMOJI_ARBOL);
                    break;

                case PIEDRA:
                    strcpy(emoji, EMOJI_PIEDRA);
                    break;
                
                case KOALA:
                    strcpy(emoji, EMOJI_KOALA);
                    break;
                
                case PILA:
                    strcpy(emoji, EMOJI_PILA);
                    break;
                
                case VELA:
                    strcpy(emoji, EMOJI_VELA);
                    break;
                
                case BENGALA:
                    strcpy(emoji, EMOJI_BENGALA);
                    break;

                default:
                    if ((herramienta_en_uso != BENGALA && es_visible(herramienta_en_uso, juego.personaje.ultimo_movimiento, juego.personaje.posicion, posicion)) || DEBUG_MAPA) {
                        strcpy(emoji, EMOJI_CLARIDAD);
                    }
            }

            printf("%s ", emoji);
        }

        printf("\n");
    }
}

/*
 * Pre: -
 * Post: Muestra al usuario el contenido actual de su mochila: herramientas disponibles y sus movimientos restantes.
 */
void mostrar_mochila(juego_t juego) {
    mostrar_separador();
    printf(BOLDGREEN "Elementos en tu mochila: \n" RESET);
    printf("\n");

    for(int i = 0; i < juego.personaje.cantidad_elementos; i++) {
        switch(juego.personaje.mochila[i].tipo) {
            case LINTERNA:
                printf("[" BOLDCYAN "%i" RESET "] %s Linterna (" BOLDGREEN "%c" RESET "), movimientos restantes: " BOLDBLUE "%i" RESET " \n", i + 1, EMOJI_LINTERNA, LINTERNA, juego.personaje.mochila[i].movimientos_restantes);
                break;

            case VELA:
                printf("[" BOLDCYAN "%i" RESET "] %s Vela (" BOLDGREEN "%c" RESET "), movimientos restantes: " BOLDBLUE "%i" RESET " \n", i + 1, EMOJI_VELA, VELA, juego.personaje.mochila[i].movimientos_restantes);
                break;

            case BENGALA:
                printf("[" BOLDCYAN "%i" RESET "] %s Bengala (" BOLDGREEN "%c" RESET "), movimientos restantes: " BOLDBLUE "%i" RESET " \n", i + 1, EMOJI_BENGALA, BENGALA, juego.personaje.mochila[i].movimientos_restantes);
                break;
        }
    }

    mostrar_separador();
}

/*
 * Pre: -
 * Post: Crea la matriz de nuestro mapa, la llena con los objetos visibles y la muestra, y muestra la información de su mochila.
 */
void mostrar_juego(juego_t juego) {
    char mapa[ALTO_MAPA][ANCHO_MAPA];

    llenar_mapa(juego, mapa);

    system("clear");
    mostrar_mapa(juego, mapa);
    mostrar_mochila(juego);
};

/*
 * Pre: -
 * Post: Detecta si la jugada hecha por el usuario es válida en caso lo sea devuelve TRUE, caso contrario FALSE.
 *       Jugadas válidas: (ARRIBA, IZQUIERDA, ABAJO, DERECHA, VELA, TIEMPO, BENGALA, LINTERNA, AYUDA, REINICIAR),
 */
bool es_jugada_valida(char jugada) {
    return (
        jugada == ARRIBA ||
        jugada == IZQUIERDA ||
        jugada == ABAJO ||
        jugada == DERECHA ||
        jugada == LINTERNA ||
        jugada == VELA ||
        jugada == BENGALA ||
        jugada == TIEMPO ||
        jugada == AYUDA ||
        jugada == REINICIAR
    );
};

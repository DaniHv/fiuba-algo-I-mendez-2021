#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

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

#define ANCHO_MAPA 20
#define ALTO_MAPA 30

#define ARBOL 'A'
#define PIEDRA 'R'
#define KOALA 'K'

const int CANTIDAD_ARBOLES = 350;
const int CANTIDAD_PIEDRAS = 80;
const int CANTIDAD_KOALAS = 1;
const int CANTIDAD_PILAS = 30;
const int CANTIDAD_VELAS = 30;
const int CANTIDAD_VENGALAS = 10;

const int JUGANDO = 0;
const int FINALIZADO = -1;

const int MIN_TIEMPO_VISIBILIDAD_PANDA = 30;

/*
 * Pre:
 * Post:
 */
coordenada_t generar_posicion_aleatoria(int fila, int columna) {
    coordenada_t posicion;

    posicion.fil = (fila != -1) ? fila : rand() % 20 + 10;
    posicion.col = (columna != -1) ? columna : rand() % 20 + 10;

    return posicion;
}

/*
 * Pre:
 * Post:
 */
void posicionar_personaje() {

}

/*
 * Pre:
 * Post:
 */
void posicionar_obstaculos(juego_t* juego, char tipo, int cantidad) {
    juego->cantidad_obstaculos += cantidad;
}

/*
 * Pre:
 * Post:
 */
void posicionar_herramientas(juego_t* juego, char tipo, int cantidad) {
    int i = 0;

    while(i < cantidad) {
        
    }

    juego->cantidad_herramientas += cantidad;
}

void inicializar_juego(juego_t* juego, char tipo_personaje) {
    posicionar_obstaculos(juego, ARBOL, CANTIDAD_ARBOLES);
    posicionar_obstaculos(juego, PIEDRA, CANTIDAD_PIEDRAS);
    posicionar_obstaculos(juego, KOALA, CANTIDAD_KOALAS);

    iniciar_cronometro();
}

int estado_juego(juego_t juego) {
    int estado = JUGANDO;

    if (juego.personaje.posicion.col == juego.amiga_chloe.col && juego.personaje.posicion.fil == juego.amiga_chloe.fil) {
        estado = FINALIZADO;
    }

    return estado;
};

/*
 * Pre:
 * Post:
 */
char detectar_obstaculo(coordenada_t coordenada) {

}

/*
 * Pre:
 * Post:
 */
void hacer_movimiento(juego_t* juego, char jugada) {
    switch (jugada) {
        case ARRIBA:
            break;
        
        case IZQUIERDA:
            break;

        case ABAJO:
            break;

        case DERECHA:
            break;
    }

    char obstaculo = detectar_obstaculo(juego->personaje->coordenada);

    switch (obstaculo) {
        case ARBOL:
            break;
        
        case PIEDRA:
            break;

        case KOALA:
            break;
    }
}

/*
 * Pre:
 * Post:
 */
void usar_herramienta(juego_t* juego, char jugada) {
    switch (jugada) {
        case LINTERNA:
            posicionar_elementos(juego, KOALA, 1);
            break;
        case VELA:
            break;
        case BENGALA:
            break;
    }
}

/*
 * Pre:
 * Post:
 */
void mostrar_tiempo() {
    double tiempo = tiempo_actual();

    printf("Tu tiempo actual es de %d", tiempo);
}

/*
 * Pre:
 * Post:
 */
void mostrar_ayuda() {
    printf("¡Estás jugando a hecho por DaniHv!");
    printf("%c \n", ARRIBA);
    printf("%c \n", IZQUIERDA);
    printf("%c \n", ABAJO);
    printf("%c \n", DERECHA);

    printf("Tus herramientas son: \n");
    printf("%c \n", LINTERNA);
    printf("%c \n", BENGALA);
    printf("%c \n", VELA);
    printf("%c \n", TIEMPO);
}

/*
 * Pre:
 * Post:
 */
void reiniciar_juego() {

}

void realizar_jugada(juego_t* juego, char jugada) {
    if(jugada == ARRIBA || jugada == IZQUIERDA || jugada == ABAJO || jugada == DERECHA) {
        hacer_movimiento(juego, jugada);
    } else if (jugada == LINTERNA || jugada == VELA || jugada == BENGALA) {
        usar_herramienta(juego, jugada);
    } else if (jugada == TIEMPO) {
        mostrar_tiempo();
    } else if (jugada == AYUDA) {
        mostrar_ayuda();
    }

    if (juego->personaje.tipo == PANDA && juego->personaje.tiempo_perdido >= MIN_TIEMPO_VISIBILIDAD_PANDA) {
        juego->chloe_visible = true;
    };
};

void llenar_mapa(juego_t juego, char mapa[ANCHO_MAPA][ALTO_MAPA]) {
    for (int i = 0; i < juego.cantidad_obstaculos; i++) {
        elemento_del_mapa_t obstaculo = juego.obstaculos[i];
        mapa[obstaculo.posicion.fil][obstaculo.posicion.fil] = obstaculo.tipo;
    }

    
}

void mostrar_juego(juego_t juego) {
    char mapa[ANCHO_MAPA][ALTO_MAPA] = {{'-'}};

    system("clear");

    for (size_t i = 0; i < ANCHO_MAPA; i++) {
        for (size_t j = 0; j < ALTO_MAPA; j++) {
            printf("%c", mapa[i][j]);
        }

        printf("\n");
    }
};

bool es_jugada_valida(char jugada) {
    return (
        jugada == ARRIBA ||
        jugada == IZQUIERDA ||
        jugada == ABAJO ||
        jugada == ARRIBA ||
        jugada == VELA ||
        jugada == TIEMPO ||
        jugada == BENGALA ||
        jugada == LINTERNA ||
        jugada == AYUDA ||
        jugada == REINICIAR
    );
};

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

/* CONSTANTES DE COLOR PARA TERMINAL */
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDCYAN    "\033[1m\033[36m"

const int TIEMPO_MAXIMO = 120;

void mostrar_bienvenida() {
    printf("\n");
    sleep(10);
}

void mostrar_inicio() {
    printf("\n");
    sleep(10);
}

void solicitar_movimiento(char* movimiento) {
    printf("Es hora de moverse y hacer algo! Qué quieres hacer?");
    scanf(" %c", movimiento);
    (*movimiento) = (char) toupper(*movimiento);

    while(es_jugada_valida(*movimiento)) {
        printf("Jugada inválida!");
        scanf(" %c", movimiento);
        (*movimiento) = (char) toupper(*movimiento);
    }
}

void mostrar_victoria() {

}

void mostrar_derrota() {

}

int main() {
    srand ((unsigned) time(NULL));
    char personaje;
    juego_t juego;

    mostrar_bienvenida();
    personalidad_detectada(&personaje);

    mostrar_inicio();
    inicializar_juego(&juego, personaje);

    while(estado_juego(juego)) {
        char movimiento;

        solicitar_movimiento(&movimiento);
        realizar_jugada(&juego, movimiento);

        mostrar_juego(juego);
    }

    double tiempo = detener_cronometro();
    if((tiempo + juego.personaje.tiempo_perdido) < TIEMPO_MAXIMO) {
        mostrar_victoria();
    } else {
        mostrar_derrota();
    }

    return 0;
}

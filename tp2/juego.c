#include <stdio.h>
#include <stdbool.h>
#include "osos_contra_reloj.h"
#include "utiles.h"

const char PANDA = 'P';
const char POLAR = 'P';
const char PARDO = 'G';

const char ARRIBA = 'W';
const char ABAJO = 'S';
const char IZQUIERDA = 'A';
const char DERECHA = 'D';
const char LINTERNA = 'L';
const char VELA = 'V';
const char BENGALA = 'B';
const char TIEMPO = 'T';

const char PERSONAJE = 'P';
const char ARBOL = 'A';

bool movimiento_valido(char movimiento) {
  return (
    movimiento == ARRIBA || 
    movimiento == ABAJO ||
    movimiento == IZQUIERDA ||
    movimiento == DERECHA ||
    movimiento == LINTERNA ||
    movimiento == VELA ||
    movimiento == BENGALA ||
    movimiento == TIEMPO
  );
}

void solicitar_movimiento(char* movimiento) {
  printf('Es hora de moverse y hacer algo! Qué quieres hacer?');
  scanf(' %c', movimiento);

}

bool chloe_encontrada(coordenada_t personaje, coordenada_t chloe) {
  return (personaje.fil == chloe.fil && personaje.col == chloe.col);
}

int main() {
  char personaje;
  juego_t juego;

  iniciar_test_personalidad(&personaje);
  inicializar_juego(&juego, personaje);

  while(!chloe_encontrada(juego.personaje.posicion, juego.amiga_chloe)) {
    char movimiento;

    solicitar_movimiento(&movimiento);
    realizar_jugada(&juego, movimiento);

    mostrar_juego(juego);
  }

  return 0;
}

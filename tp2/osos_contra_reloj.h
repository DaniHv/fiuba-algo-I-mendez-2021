#ifndef __OSOS_CONTRA_RELOJ_H__
#define __OSOS_CONTRA_RELOJ_H__

#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>

#define MAX_OBSTACULOS 600
#define MAX_HERRAMIENTAS 600


typedef struct coordenada { 
	int fil;
	int col; 
} coordenada_t;

typedef struct elemento_del_mapa { 
	char tipo;
	coordenada_t posicion; 
	bool visible;
} elemento_del_mapa_t;

typedef struct elemento_mochila {
	char tipo;
	int movimientos_restantes;
} elemento_mochila_t;

typedef struct personaje {
	char tipo;
	coordenada_t posicion; 
	elemento_mochila_t mochila[MAX_HERRAMIENTAS];
	int cantidad_elementos;
	int elemento_en_uso; // -1 si no hay nada en uso, o posicion del vector de lo que esta en uso.
	double tiempo_perdido;
	char ultimo_movimiento;
} personaje_t;

typedef struct juego {
	elemento_del_mapa_t obstaculos[MAX_OBSTACULOS];
	int cantidad_obstaculos;
	elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS];
	int cantidad_herramientas;
	personaje_t personaje;
	coordenada_t amiga_chloe;
	bool chloe_visible;
} juego_t;


/*
 * Inicializará el juego, cargando toda la información inicial
 * y los datos del personaje. 
 */
void inicializar_juego(juego_t* juego, char tipo_personaje);

/*
 * Recibe un juego con todas sus estructuras válidas.
 *
 * El juego se dará por terminado, si el personaje encontró a Chloe. 
 * Devolverá:
 * -> 0 si el estado es jugando. 
 * -> -1 si el estado es terminado.
 */

int estado_juego(juego_t juego);

/*
 * Mueve el personaje en la dirección indicada por el usuario o habilita 
 * cualquiera de las herramientas y actualiza el juego según los elementos 
 * que haya en el camino del personaje.
 * El juego quedará en un estado válido al terminar el movimiento. 
 * El movimiento será:
 * -> W: Si el personaje debe moverse para la arriba. 
 * -> A: Si el personaje debe moverse para la izquierda.
 * -> S: Si el personaje debe moverse para la abajo.
 * -> D: Si el personaje debe moverse para la derecha.
 * -> L: Si el personaje quiere encender una linterna.
 * -> V: Si el personaje quiere encender una vela. 
 * -> E: Si el personaje quiere encender la bengala.
 * -> T: Si el personaje quiere ver el tiempo restante.
 * En caso de que querer activar una herramienta, y no tenga mas movimientos, no deberá 
 * activarse ninguna ventaja. 
 * Si se aprieta una tecla de iluminación y esta ya está siendo usada, se desactivará colocando
 * el int elemento_en_uso en -1.
 */
void realizar_jugada(juego_t* juego, char jugada);

/*
 * Mostrará el juego por pantalla.
 * Se recomienda mostrar todo lo que sea de utilidad para el jugador.
 */
void mostrar_juego(juego_t juego);

/*
 * Devolvera true en caso de ser una jugada valida (W,A,S,D,L,V,E o T)
 * Devolvera false en caso contrario.
 */
bool es_jugada_valida(char jugada);

#endif /* __OSOS_CONTRA_RELOJ_H__ */

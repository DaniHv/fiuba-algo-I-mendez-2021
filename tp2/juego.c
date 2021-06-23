#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
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

#define MAX_ARGUMENTO 10

const static char VACIO = '-';
const static char ARGUMENTO_PANDA[MAX_ARGUMENTO] = "PANDA";
const static char ARGUMENTO_PARDO[MAX_ARGUMENTO] = "PARDO";
const static char ARGUMENTO_POLAR[MAX_ARGUMENTO] = "POLAR";

const static char EMOJI_PANDA[20] = "\U0001F43C";
const static char EMOJI_POLAR[20] = "\U0001F43B\U0000200D\U00002744\U0000FE0F";
const static char EMOJI_PARDO[20] = "\U0001F43B";
const static char EMOJI_CHLOE[20] = "\U0001F467";

const static double TIEMPO_MAXIMO = 120.00;
const static char SEGUIR_JUGANDO = 'S';
const static char SALIR = 27;

const static int BONIFICACION_ARBOL_PARDO = 50;
const static int BONIFICACION_LINTERNA_PARDO = 50;
const static int BONIFICACION_VELAS_POLAR = 50;
const static int BONIFICACION_BENGALAS_PANDA = 2;

/*
 * Pre: -
 * Post: Imprimirá por pantalla una línea separadora.
 */
void static mostrar_separador() {
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
}

/*
 * Pre: -
 * Post: Convierte el string recibido en mayusculas.
 */
void convertir_string_mayusculas(char string[]) {
    for (int i = 0; i < strlen(string) ; i++) {
        string[i] = (char) toupper(string[i]);
    }
}

/*
 * Pre: -
 * Post: Permite detectar mediante argumentos de ejecución el personaje con el que se desea jugar (primer argumento, luego del ejecutable),
 *       con el fin de evitar el test de personalidad. Principalmente para pruebas rápidas de cada personaje.
 */
void detectar_argumento_personaje(int argc, char** argv, char* personaje) {
    if (argc > 1) {
        char argumento[MAX_ARGUMENTO] = "";

        strcpy(argumento, argv[1]);
        convertir_string_mayusculas(argumento);

        mostrar_separador();

        if (!strcmp(argumento, ARGUMENTO_PANDA)) {
            (*personaje) = PANDA;
            printf("Empiezas a jugar directamente con Panda! \n");
        } else if (!strcmp(argumento, ARGUMENTO_PARDO)) {
            (*personaje) = PARDO;
            printf("Empiezas a jugar directamente con Pardo! \n");
        } else if (!strcmp(argumento, ARGUMENTO_POLAR)) {
            (*personaje) = POLAR;
            printf("Empiezas a jugar directamente con Polar! \n");
        } else {
            printf("El personaje escrito (%s) es inválido, por lo tanto empezamos desde cero. \n\n", argv[1]);
        }

        mostrar_separador();
        printf("\n");
    }
}

/*
 * Pre: -
 * Post: Muestra la bienvenida inicial del juego. Explica al usuario en que consiste el juego y que tendrá que hacer para empezar a jugar.
 */
void mostrar_bienvenida_juego() {
    mostrar_separador();
    printf(GREEN "Bienvenido a \"Buscando a Chloe\"! \n" RESET);
    printf(BOLDBLUE "Desarrollado por: @DaniHv \n" RESET);
    mostrar_separador();
    printf(
        "Los osos escandalosos "BOLDBLUE"(Panda %s, Pardo %s y Polar %s)"RESET" y su amiga "BOLDYELLOW"Chloe"RESET" %s salieron al bosque a jugar."
        "Sin embargo, olvidaron la cuarentena que les prohibe estar afuera, al oscurecer y recordar la restricción, tuvieron que apurarse para regresar, pero "BOLDCYAN"Chloe sigue escondida y debemos encontrarla antes de que anochezca.\n" RESET,
        EMOJI_PANDA,
        EMOJI_PARDO,
        EMOJI_POLAR,
        EMOJI_CHLOE
    );
    printf("\n");
    printf("Antes de empezar y contarte todas las mecánicas del juego, te haremos un test de personalidad para determinar el escandaloso con el que jugarás. "BOLDCYAN"Cada uno de ellos cuenta con características particulares."RESET" \n");
    printf("\n");
    printf(BOLDGREEN "¡Ahora que ya sabes lo que viene, empecemos con el test! \n" RESET);
    mostrar_separador();

    // sleep(DELAY_LARGO);
    printf("\n");
}

/*
 * Pre: Se debe enviar un personaje válido.
 * Post: Muestra al usuario el mensaje de inicio del juego que contiene información de su personaje resultante, así como también las mecánicas, movimientos y herramientas del juego.
 */
void mostrar_inicio(char personaje) {
    mostrar_separador();

    switch (personaje) {
        case PANDA:
            printf("Bien hecho, tu escandaloso es Panda %s! Tus características especiales son: \n", EMOJI_PANDA);
            printf("1- Por ser un genio usando el GPS, podrás ver la posición de Chloe cuando sumes 30 segundos de tiempo perdido. \n");
            printf("2- A diferencia de tus hermanos, empiezas con %i bengalas en tu mochila. \n", BONIFICACION_BENGALAS_PANDA);
            break;

        case PARDO:
            printf("Bien hecho, tu escandaloso es Pardo %s! Tus características especiales son: \n", EMOJI_PARDO);
            printf("1- Tu linterna tiene un %i%% más movimientos restantes. \n", BONIFICACION_LINTERNA_PARDO);
            printf("2- Los árboles te quitan un %i%% menos de tiempo. \n", BONIFICACION_ARBOL_PARDO);
            break;

        case POLAR:
            printf("Bien hecho, tu escandaloso es Polar %s! Tus características especiales son: \n", EMOJI_POLAR);
            printf("1- Empiezas con un %i%% más de velas en tu mochila. \n", BONIFICACION_VELAS_POLAR);
            printf("2- Las piedras no te quitan tiempo. \n");
            break;
    }

    printf("\n");
    printf("¡Recuerda que no tenemos tiempo que perder! Debes hallar a Chloe %s lo más pronto posible. \n", EMOJI_CHLOE);

    mostrar_separador();

    // sleep(DELAY_LARGO);
    printf("\n");
}

/*
 * Pre: -
 * Post: Solicitará el canal al usuario hasta introducir uno válido:
 *       (ARRIBA, IZQUIERDA, ABAJO, DERECHA, LINTERNA, VELA, BENGALA, TIEMPO, AYUDA, REINICIAR),
 *       y actualiza al movimiento elegido.
 */
void solicitar_movimiento(char* movimiento) {
    printf("Es hora de moverse y hacer algo! Qué quieres hacer? \n");
    scanf(" %c", movimiento);
    (*movimiento) = (char) toupper(*movimiento);

    while(!es_jugada_valida(*movimiento)) {
        printf("Jugada inválida! Puedes ver los movimientos válidos presionando \"H\" \n");
        scanf(" %c", movimiento);
        (*movimiento) = (char) toupper(*movimiento);
    }
}

/*
 * Pre: -
 * Post: Muestra el mensaje de victoria al jugador.
 */
void mostrar_victoria() {
    mostrar_separador();
    printf(GREEN "¡Has encontrado a Chloe antes de que fuera demasiado tarde! Ve a contarle a tus hermanos para regresar juntos a casa. \n" RESET);
    mostrar_separador();
}

/*
 * Pre: -
 * Post: Muestra el mensaje de derrota al jugador.
 */
void mostrar_derrota(double tiempo) {
    mostrar_separador();
    printf("Haz perdido, te ha tomado un total de %.0f segundos. \n", tiempo);
    mostrar_separador();
}

/*
 * Pre: Debe recibir un personaje válido (PANDA, PARDO o POLAR).
 * Post: Ejecutará el juego hasta que se encuentre a Chloe, mostrará el resultado de la partida y ejecutará preguntar_seguir_jugando.
 */
void jugar(char personaje) {
    juego_t juego;

    inicializar_juego(&juego, personaje);

    while(estado_juego(juego) != -1) {
        char movimiento;

        mostrar_juego(juego);
        solicitar_movimiento(&movimiento);
        realizar_jugada(&juego, movimiento);

        sleep(DELAY_CORTO);
    }

    system("clear");

    double tiempo = detener_cronometro() + juego.personaje.tiempo_perdido;

    if(tiempo < TIEMPO_MAXIMO) {
        mostrar_victoria();
    } else {
        mostrar_derrota(tiempo);
    }
}

/*
 * Pre: -
 * Post: Pregunta al jugador si quiere seguro jugando (hasta elegir una opción correcta, SEGUIR_JUGANDO o SALIR) y actualiza valor del puntero bool enviado.
 */
void preguntar_seguir_jugando(bool* continuar) {
    char resultado = '-';
    printf("¿Quieres seguir jugando? Presiona (" BOLDGREEN "%c" RESET ") para reiniciar el juego, o (" BOLDGREEN "ESC" RESET ") para salir.  \n", SEGUIR_JUGANDO);

    scanf(" %c", &resultado);
    resultado = (char) toupper(resultado);

    while (resultado != SEGUIR_JUGANDO && resultado != SALIR) {
        printf("Debes elegir entre salir o seguir jugando. Presiona (" BOLDGREEN "%c" RESET ") para reiniciar el juego, o (" BOLDGREEN "ESC" RESET ") para salir  \n", SEGUIR_JUGANDO);
        scanf(" %c", &resultado);
        resultado = (char) toupper(resultado);
    }

    (*continuar) = (resultado == SEGUIR_JUGANDO);
}

/*
 * Pre: -
 * Post: Muestra mensaje de gracias y despedida final.
 */
void mostrar_gracias() {
    mostrar_separador();
    printf(BOLDGREEN "Gracias por haber jugado a " BOLDBLUE "\"Travesías escandalosas en cuarentena\""RESET" \n");
    printf(BOLDCYAN "Espero que te haya gustado. @DaniHv te envía saludos :) \n" RESET);
    mostrar_separador();
}

int main(int argc, char** argv) {
    char personaje = VACIO;
    bool continuar = false;

    srand ((unsigned) time(NULL));
    system("clear");

    detectar_argumento_personaje(argc, argv, &personaje);

    if (personaje == VACIO) {
        mostrar_bienvenida_juego();
        test_de_personalidad(&personaje);
        mostrar_inicio(personaje);
    }

    do {
        jugar(personaje);
        // preguntar_seguir_jugando(&continuar); // Desactivar para evitar problemas de Chanutron
    } while(continuar);

    mostrar_gracias();

    return 0;
}

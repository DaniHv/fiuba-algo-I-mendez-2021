#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "test_de_personalidad.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDCYAN    "\033[1m\033[36m"

#define ANIME 'A'
#define MUSICAPOP 'M'
#define LIMPIEZA 'L'

#define BAMBU 'B'
#define PESCADO 'P'
#define FOCAS 'F'

#define MIN_PISO 1
#define MAX_PISO 18

#define MIN_GRITO 1
#define MAX_GRITO 18

#define MULTIPLICADOR_ANIME 2
#define MULTIPLICADOR_MUSICAPOP 3
#define MULTIPLICADOR_LIMPIEZA 1

#define PUNTAJE_BAMBU 6
#define PUNTAJE_PESCADO 9
#define PUNTAJE_FOCAS 3

#define MIN_PUNTAJE_POLAR 5
#define MAX_PUNTAJE_POLAR 24
#define MIN_PUNTAJE_PANDA 25
#define MAX_PUNTAJE_PANDA 43
#define MIN_PUNTAJE_PARDO 44
#define MAX_PUNTAJE_PARDO 63

/*
 * Pre: -
 * Post: Imprimirá por pantalla una línea separadora.
 */
void static mostrar_separador() {
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
}

/*
 * Pre: -
 * Post: Imprimirá por pantalla el mensaje de bienvenida.
 */
void static mostrar_bienvenida() {
    mostrar_separador();
    printf(GREEN "Bienvenido al test escandalosíl, con una serie de preguntas podrás identificar a que escandaloso te pareces más (Panda, Polar o Pardo). Dicho esto, empecemos :).\n" RESET);
    mostrar_separador();
}

/*
 * Pre: -
 * Post: Devolverá TRUE si el canal enviado es válido (ANIME, MUSICAPOP o LIMPIEZA), si no, FALSE.
 */
bool static canal_valido(char canal) {
    return ((canal == ANIME) || (canal == MUSICAPOP) || (canal == LIMPIEZA)); 
}

/*
 * Pre: -
 * Post: Solicitará el canal al usuario hasta introducir uno válido (ANIME, MUSICAPOP o LIMPIEZA), y actualiza al canal elegido.
 */
void static solicitar_canal(char* canal) {
    printf("Vas a ver televisión un rato, pones el canal de: Anime (" GREEN "%c" RESET "), Musica Pop (" GREEN "%c" RESET "), Limpieza (" GREEN "%c" RESET ").\n", ANIME, MUSICAPOP, LIMPIEZA);
    scanf(" %c", canal);
    (*canal) = (char) toupper(*canal);

    while(!canal_valido(*canal)) {
        printf(RED "Elegiste un canal inválido!" RESET " Intenta nuevamente eligiendo entre Anime (" RED "%c" RESET "), Musica Pop (" RED "%c" RESET "), Limpieza (" RED "%c" RESET ").\n", ANIME, MUSICAPOP, LIMPIEZA);
        scanf(" %c", canal);
        (*canal) = (char) toupper(*canal);
    }

    printf(GREEN "Bien! Apenas empezamos, vamos a por la próxima.\n" RESET);
    mostrar_separador();
}

/*
 * Pre: -
 * Post: Devolverá TRUE si el alimento enviado es válido (BAMBU, PESCADO o FOCAS), si no, FALSE.
 */
bool static alimento_valido(char alimento) {
    return ((alimento == BAMBU) || (alimento == PESCADO) || (alimento == FOCAS)); 
}

/*
 * Pre: -
 * Post: Solicitará el alimento al usuario hasta introducir uno válido (BAMBU, PESCADO, FOCAS), y actualiza al alimento elegido.
 */
void static solicitar_alimento(char* alimento) {
    printf("Solo podes guardar un alimento en tu vianda: Bambú (" GREEN "%c" RESET "), Pescado (" GREEN "%c" RESET "), Focas (" GREEN "%c" RESET ").\n", BAMBU, PESCADO, FOCAS);
    scanf(" %c", alimento);
    (*alimento) = (char) toupper(*alimento);

    while(!alimento_valido(*alimento)) {
        printf(RED "Elegiste un alimento inválido!" RESET " Intenta nuevamente con Bambú (" RED "%c" RESET "), Pescado (" RED "%c" RESET ") o Focas (" RED "%c" RESET ").\n", BAMBU, PESCADO, FOCAS);
        scanf(" %c", alimento);
        (*alimento) = (char) toupper(*alimento);
    }

    printf(GREEN "¿Te dió hambre pensar en bambúes, pescados y/o focas? ¿Si? ¿No? No importa, aún no terminamos.\n" RESET);
    mostrar_separador();
}

/*
 * Pre: -
 * Post: Devolverá TRUE si el piso enviado está entre MIN_PISO y MAX_PISO (ambos inclusive), si no, FALSE.
 */
bool static piso_valido(int piso) {
    return ((piso >= MIN_PISO) && (piso <= MAX_PISO)); 
}

/*
 * Pre: -
 * Post: Solicitará el piso al usuario hasta introducir uno válido (MIN_PISO-MAX_PISO), y actualiza al piso elegido.
 */
void static solicitar_piso(int* piso) {
    printf("Te compras una torre con tus dos hermanos de %i pisos. ¿En que piso te gustaría vivir (Entre " GREEN "%i" RESET " y " GREEN "%i" RESET ")?\n", MAX_PISO, MIN_PISO, MAX_PISO);
    scanf("%i", piso);

    while(!piso_valido(*piso)) {
        printf(RED "Respuesta incorrecta!" RESET " Responde nuevamente eligiendo un piso entre " GREEN "%i" RESET " y " GREEN "%i" RESET " \n", MIN_PISO, MAX_PISO);
        scanf("%i", piso);
    }

    printf(GREEN "¿En serio eligirías un solo piso cuando toda la torre sería tuya? " BLUE "@DaniHv" GREEN " no, pero bueno, su opinión no importa :(.\n" RESET);
    mostrar_separador();
}

/*
 * Pre: -
 * Post: Devolverá TRUE si el grito enviado está entre MIN_GRITO y MAX_GRITO (ambos inclusive), si no, FALSE.
 */
bool static grito_valido(int piso) {
    return ((piso >= MIN_PISO) && (piso <= MAX_PISO)); 
}

/*
 * Pre: -
 * Post: Solicitará el grito al usuario hasta introducir uno válido (MIN_GRITO-MAX_GRITO), y actualiza al grito elegido.
 */
void static solicitar_grito(int* grito) {
    printf("¡Oh, una rata! ¿Que tan fuerte gritas del " GREEN "%i" RESET " al " GREEN "%i" RESET "? Siendo " GREEN "%i" RESET " no gritar y " GREEN "%i" RESET " desgarrarse la garganta.\n", MIN_GRITO, MAX_GRITO, MIN_GRITO, MAX_GRITO);
    scanf("%i", grito);

    while(!grito_valido(*grito)) {
        printf(RED "Respuesta incorrecta!" RESET " intenta nuevamente eligiendo únicamente entre %i y %i.\n", MIN_GRITO, MAX_GRITO);
        scanf("%i", grito);
    }

    printf(GREEN "No importa lo que hayas respondido, todos sabemos que incluso %i sigue siendo poco.\n" RESET, MAX_GRITO);
    mostrar_separador();
}

/*
 * Pre: Se debe recibir un alimento previamente validado (BAMBU, PESCADO o FOCAS)
 * Post: Devolverá el puntaje correspondiente al alimento recibido.
 *       (BAMBU = PUNTAJE_BAMBU, PESCADO = PUNTAJE_PESCADO, FOCAS = PUNTAJE_FOCAS)
 */
int static puntaje_alimento(char alimento) {
    int puntaje;

    switch (alimento) {
        case BAMBU:
            puntaje = PUNTAJE_BAMBU;
            break;
        case PESCADO:
            puntaje = PUNTAJE_PESCADO;
            break;
        case FOCAS:
            puntaje = PUNTAJE_FOCAS;
            break;
    }

    return puntaje;
}

/*
 * Pre: Se debe recibir un canal previamente validado (ANIME, MUSICAPOP o LIMPIEZA)
 * Post: Se devuelve el multiplicador correspondiente al canal recibido.
 *       (ANIME = MULTIPLICADOR_ANIME, MUSICAPOP = MULTIPLICADOR_MUSICAPOP, LIMPIEZA = MULTIPLICADOR_LIMPIEZA).
 */
int static multiplicador_canal(char canal) {
    int multiplicador;

    switch (canal) {
        case ANIME:
            multiplicador = MULTIPLICADOR_ANIME;
            break;
        case MUSICAPOP:
            multiplicador = MULTIPLICADOR_MUSICAPOP;
            break;
        case LIMPIEZA:
            multiplicador = MULTIPLICADOR_LIMPIEZA;
            break;
    }

    return multiplicador;
}

/*
 * Pre: El puntajes y multiplicador deben ser válidos:
 *      Alimento debe ser un puntaje válido (PUNTAJE_BAMBU, PUNTAJE_PESCADO, PUNTAJE_FOCAS)
 *      Canal debe ser un multiplicador válido (MULTIPLICADOR_ANIME, MULTIPLICADOR_MUSICAPOP, MULTIPLICADOR_LIMPIEZA)
 *      Piso debe ser un puntaje válido (Entre MIN_PISO y MAX_PISO)
 *      Grito debe ser un puntaje válido (Entre MIN_GRITO y MAX_GRITO)
 * Post: Devolverá el cálculo del puntaje total basándose en la fórmula:
 *       (PUNTAJE DEL ALIMENTO * MULTIPLICADOR CANAL) + PUNTAJE_PISO + PUNTAJE_GRITO
 */
int static puntaje_total(int puntaje_alimento, int multiplicador_canal, int puntaje_piso, int puntaje_grito) {
    return ((puntaje_alimento * multiplicador_canal) + puntaje_piso + puntaje_grito);
}

/*
 * Pre: El puntaje debe ser previamente calculado con la fórmula de puntaje total:
 *      (PUNTAJE DEL ALIMENTO * MULTIPLICADOR CANAL) + PUNTAJE_PISO + PUNTAJE_GRITO
 * Post: Devolverá el escandaloso correspondiente al puntaje enviado (Que debe estar comprendido entre 0 y 0).
 *       Tal que entre 5 y 24 es POLAR, entre 25 y 43 es PANDA y desde 43 es PARDO.
 */
char static escandaloso(int puntaje) {
    char escandaloso;

    if ((puntaje >= MIN_PUNTAJE_POLAR) && (puntaje <= MAX_PUNTAJE_POLAR)) {
        escandaloso = POLAR;
    } else if ((puntaje >= MIN_PUNTAJE_PANDA) && (puntaje <= MAX_PUNTAJE_PANDA)) {
        escandaloso = PANDA;
    } else {
        escandaloso = PARDO;
    }

    return escandaloso;
}

/*
 * Pre: -
 * Post: Imprimirá por pantalla los datos adicionales de PANDA.
*/
void static mostrar_datos_panda() {
    printf(BOLDGREEN "Algunos datos sobre Panda antes de empezar:\n" RESET);
    printf(CYAN "1-" RESET " Inseguro, sensible, se enamora rapido, tierno, vegetariano.\n");
    printf(CYAN "2-" RESET " El celular es su alma en forma rectangular.\n");
    printf(CYAN "3-" RESET " Usa lentes de contacto.\n");
    printf(CYAN "4-" RESET " Tiene una almohada de Miki-Chan.\n");
    printf(CYAN "5-" RESET " Le gusta ir de compras.\n");
    printf(CYAN "6-" RESET " Le gusta el anime y las telenovelas.\n");
}

/*
 * Pre: -
 * Post: Imprimirá por pantalla los datos adicionales de POLAR.
*/
void static mostrar_datos_polar() {
    printf(BOLDGREEN "Algunos datos sobre Polar antes de empezar:\n" RESET);
    printf(CYAN "1-" RESET " Callado, reacciona rápido, habla en tercera persona, demuestra pobremente sus emociones, responsable y maduro, valiente.\n");
    printf(CYAN "2-" RESET " Le encanta la limpieza ya que la mayor parte de su tiempo pasa haciendo eso, y la cocina como podemos ver casi siempre.\n");
    printf(CYAN "3-" RESET " Sabe artes marciales, yoga, bailar, patinar sobre hielo y tejer.\n");
    printf(CYAN "4-" RESET " No puede controlar la cafeína.\n");
    printf(CYAN "5-" RESET " Es el único personaje capaz de guardar objetos dentro de su brazo, por ejemplo, una barra de chocolate.\n");
    printf(CYAN "6-" RESET " Le asustan los pepinos.\n");
}

/*
 * Pre: -
 * Post: Imprimirá por pantalla los datos adicionales de PARDO.
*/
void static mostrar_datos_pardo() {
    printf(BOLDGREEN "Algunos datos sobre Pardo antes de empezar:\n" RESET); 
    printf(CYAN "1-" RESET " Familiarizado con el área forestal.\n");
    printf(CYAN "2-" RESET " Líder, hiperactivo, ruidoso, charlatan, alegre, optimista, extrovertido, quien hace los planes, glotón, sociable, disfruta de la atención, celoso, ingenuo.\n");
    printf(CYAN "3-" RESET " Hace todo lo posible para hacer lo correcto.\n");
    printf(CYAN "4-" RESET " De vez en cuando intentará hacerse famoso en Internet.\n");
    printf(CYAN "5-" RESET " Amor por el cine. Le gustan las películas de acción.\n");
    printf(CYAN "6-" RESET " Le gusta la cultura pop.\n");
}

/*
 * Pre: Se debe recibir un escandaloso válido (PANDA, POLAR o PARDO).
 * Post: Imprimirá por pantalla el resultado recibido y los datos adicionales correspondientes.
 */
void static mostrar_resultado(char escandaloso) {
    printf(BOLDYELLOW "Ahora vamos con los resultados.\n" RESET);
    printf(BOLDYELLOW "**REDOBLE DE TAMBORES**\n" RESET);
    mostrar_separador();

    switch (escandaloso) {
        case PANDA:
            printf(BOLDGREEN "TU RESULTADO ES:" BOLDCYAN " - Panda (%c) - \n" RESET, PANDA);
            mostrar_separador();
            mostrar_datos_panda();
            break;
    
        case POLAR:
            printf(BOLDGREEN "TU RESULTADO ES:" BOLDCYAN " - Polar (%c) - \n" RESET, POLAR);
            mostrar_separador();
            mostrar_datos_polar();
            break;
    
        case PARDO:
            printf(BOLDGREEN "TU RESULTADO ES:" BOLDCYAN " - Pardo (%c) - \n" RESET, PARDO);
            mostrar_separador();
            mostrar_datos_pardo();
            break;
    }

    mostrar_separador();
}

/*
 * Pre: -
 * Post: Realizará el test de personalidad al usuario solicitando sus elecciones en CANAL, ALIMENTO, PISO y Grito
 *       Y actualiza el puntero al personaje resultante en base a las elecciones y fórmulas aplicadas.
 */
void test_de_personalidad(char* personalidad_detectada) {
    char canal;
    char alimento;
    int piso;
    int grito;

    mostrar_bienvenida();

    solicitar_canal(&canal);
    solicitar_alimento(&alimento);
    solicitar_piso(&piso);
    solicitar_grito(&grito);

    int puntaje = puntaje_total(multiplicador_canal(canal), puntaje_alimento(alimento), piso, grito);
    char resultado = escandaloso(puntaje);
    (*personalidad_detectada) = resultado;

    system("clear");
    mostrar_resultado(resultado);
    printf("\n");
}

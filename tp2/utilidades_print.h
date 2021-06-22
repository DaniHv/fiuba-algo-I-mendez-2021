#ifndef __UTILIDADES_PRINT_H__
#define __UTILIDADES_PRINT_H__

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

/*
 * Pre: -
 * Post: Imprimirá por pantalla una línea separadora.
 */
void mostrar_separador();

#endif /* __UTILIDADES_PRINT_H__ */

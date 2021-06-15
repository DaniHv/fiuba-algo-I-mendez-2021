#ifndef __UTILES_H__
#define __UTILES_H__

/*
 * Inicia el cronómetro.
 */
void iniciar_cronometro();

/*
 * Retorna el tiempo actual en segundos desde que se inicio el crónometro.
 * En caso de error, retorna un valor menor a 0.
 */
double tiempo_actual();

/*
 * Detiene el cronómetro, devolviendo el tiempo total desde que se inicio en segundos.
 * En caso de error, retorna un valor menor a 0.
 */
double detener_cronometro();


#endif /* __UTILES_H__ */

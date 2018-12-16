#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

/*! @defgroup temporizador Librería temporizador Grupo 1

*/
/*@{*/
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

/*
 * TIPO             BYTES
   unsigned char    1 
   unsigned int     2 
   unsigned long    4
*/

/**
* @brief Inicializa los flags del contador. Cada vez que se llama se resetea el contador.
* @param factorT Factor de escalado del procesador. Va de 0 a 7. La frecuencia base del reloj (8MHz) se divide por la potencia de dos elevado al factor de escalado. Ejemplo factoT = 7  8MHz/2⁷ = 16μsg
* @return
*/
void init_temporizador(int factor);

/**
* @brief Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @param
* @return 4unsigned chars que representan el valor hexadecimal
*/
unsigned long get_microseconds(void);


/**
* @brief Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @return 4 unsigned chars que representan el valor hexadecimal
*/
unsigned long get_miliseconds(void);

/**
* @brief Espera un tiempo bloqueando la ejecución del programa
* No hay detalles
* @param useg valor decimal en microsegundos que espera la funcion
*/
void delayusg(unsigned long useg);


/**
* @brief Imprime 4 unsigned chars por pantalla en formato hexadecimal
*/
void print4bWord(unsigned long word);

/**
* @brief Ejecuta una función después de un tiempo determiando
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runAfterUsg(void (*f)(void*), void * args, unsigned long useg);

/**
* @brief Ejecuta una función periodicamente
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runEveryUsg(void (*f)(void*), void * args, unsigned long useg);

/*@}*/

#endif

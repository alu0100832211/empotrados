#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

/*! @defgroup temporizador Librería temporizador Grupo 1

*/
/*@{*/
#include <stdlib.h>

#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>


typedef unsigned char byte;
typedef unsigned int bytes2;
typedef unsigned long bytes4;

/**
* @brief Inicializa los flags del contador. Cada vez que se llama se resetea el contador.
* @param factorT Factor de escalado del procesador. Va de 0 a 7. La frecuencia base del reloj (8MHz) se divide por la potencia de dos elevado al factor de escalado. Ejemplo factoT = 7  8MHz/2⁷ = 16μsg
* @return
*/
void init_temporizador(int factor);

/**
* @brief Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @param
* @return 4bytes que representan el valor hexadecimal
*/
bytes4 get_microseconds(void);


/**
* @brief Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @return 4 bytes que representan el valor hexadecimal
*/
bytes4 get_miliseconds(void);

/**
* @brief Espera un tiempo bloqueando la ejecución del programa
* No hay detalles
* @param useg valor decimal en microsegundos que espera la funcion
*/
void delayusg(unsigned long useg);


/**
* @brief Imprime 4 bytes por pantalla en formato hexadecimal
*/
void print4bWord(bytes4 word);

/**
* @brief Ejecuta una función después de un tiempo determiando
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runAfterUsg(void (*f)(void*), void * args, bytes4 useg);

/**
* @brief Ejecuta una función periodicamente
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runEveryUsg(void (*f)(void*), void * args, bytes4 useg);

/*@}*/

#endif

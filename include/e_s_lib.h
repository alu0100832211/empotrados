/**
 * @file doxygen_c.h
 * @author Sergio y Santiago
 * @date 14/12/18
 * @brief Includes necesarios
 *
 * Librería E/S P1 Sistemas Empotrados
 */

#ifndef _E_S_LIB_H
#define _E_S_LIB_H

#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

/*! @defgroup e_s_lib Librería entrada/salida del grupo 4

*/
/*@{*/



//typedef unsigned char unsigned char;
//typedef int int;	//declaración de inteano


/**
 * @brief Método de configuración de puerto como entrada/salida y sus respectivos pines
 *
 * ~~Llamada para configurar los pines 0 1 2 3 4 5 del puerto H como salida
 * configurar('H', 1, "0 1 2 3 4 5")~~
 *
 * ~~Llamada para configurar los pines 0 1 del puerto G como Entrada
 * configurar('G', 0, "0 1")~~
 *
 * Llamada para configurar el pin 3 del puerto H como entrada:
 * `configurar_puerto('H', 0, 3)`.
 *
 * Llamada para configurar el pin 5 del puerto G como salida:
 * `configurar_puerto('G',1, 5)`.
 *
 * @param port_char caracter con el nombre del puerto 'H', 'G', ...
 * @param conf dirección a asignar: 1 salida, 0 entrada
 * @param pin pin a configurar
 * @return 1 si se realizó configuración, 0 si algún fallo.
 */
int configurar_puerto(const char port_char, const int conf, int pin);

/**
 * @brief Método para encender/apagar los PULLUP en los diferentes puertos (A,B,E,H,G)
 *
 * accion = true -> Encender pullup
 * accion = false -> Apagar pullup
 * Encender pullup en puerto A
 * pull_up('A', 1);
 *
 * Apagar pullup en puerto B
 * pull_up('B', 0);
 */
int pull_up(const char port_char, const int accion);

/**
 * @brief Método para encender/apagar los PULLUP en todos los puertos
 *
 * accion = true -> Encender pullup
 * accion = false -> Apagar pullup
 * Encender pullup
 * pull_up(1);
 *
 * Apagar pullup
 * pull_up(0);
 */
int pull_up_all(const int accion);

/**
 * @brief Método para leer un puerto
 *
 * Leer del puerto H
 * unsigned char value;
 * value = leer_puerto('H');
 */
unsigned char leer_puerto(const char port_char);
/**
 * @brief Método para escribir un valor en un puerto
 *
 * Escribir en puerto H el valor 0xff
 *```
 * unsigned char value = 0xff;
 * escribir_puerto('H', value);
 *```
 */
void escribir_puerto(const char port_char, unsigned char value);


/**
 * @brief Método que devuelve la máscara del bit del puerto correspondiente
 *
 * es equivalente a `(1 << bit)`.
 */
unsigned char leer_puerto_bit(const char port_char, int bit);


/*@}*/

#endif

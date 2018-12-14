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



typedef unsigned char byte;
typedef int bool;	//declaración de booleano


/**
 * @brief Método de configuración de puerto como entrada/salida y sus respectivos pines
 *
 * Llamada para configurar los pines 0 1 2 3 4 5 del puerto H como salida
 * configurar('H', 1, "0 1 2 3 4 5")
 *
 * Llamada para configurar los pines 0 1 del puerto G como Entrada
 * configurar('G', 0, "0 1")
 */
bool configurar_puerto(const char port_char, const bool conf, char* pines);

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
bool pull_up(const char port_char, const bool accion);

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
bool pull_up_all(const bool accion);

/**
 * @brief Método para leer un puerto
 *
 * Leer del puerto H
 * byte value;
 * value = leer_puerto('H');
 */
byte leer_puerto(const char port_char);
/**
 * @brief Método para leer un puerto
 *
 * Escribir en puerto H el valor 0xff
 * byte value = 0xff;
 * escribir_puerto('H', value);
 */
void escribir_puerto(const char port_char, byte value);

/*@}*/

#endif

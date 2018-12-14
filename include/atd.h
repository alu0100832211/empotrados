/**
******************************************************
* @file atd.h
* @brief Librería para el manejo del Conversor Analógico-Digital
* @author Yeray Expósito García
* @author Gabriel Alejandro Rodriguez de Abreu
* @version 1.0
* @date Diciembre 2018
*******************************************************/
#ifndef _ATD_H
#define _ATD_H

/*! @defgroup atd Libería A/D del Grupo 3

*/
/*@{*/


#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

#define DIST_AD (M6812_ATD1CTL0-M6812_ATD0CTL0)
#define DIST_RES (M6812_ADR01H-M6812_ADR00H)

#define A_IN_0 0
#define A_IN_1 M6812B_CA
#define A_IN_2 M6812B_CB
#define A_IN_3 (M6812B_CB | M6812B_CA)
#define A_IN_4 M6812B_CC
#define A_IN_5 (M6812B_CC | M6812B_CA)
#define A_IN_6 (M6812B_CC | M6812B_CB)
#define A_IN_7 (M6812B_CC | M6812B_CB | M6812B_CA)

typedef int bool;
#define true 1
#define false 0

typedef char byte;
typedef unsigned short word;

/**
 * @brief Permite establecer la resolución a 10 bits.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
inline void atd_set_resolution10( unsigned short port);

/**
 * @brief Permite establecer la resolución a 8 bits.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
inline void atd_set_resolution8(unsigned short port);

/**
 * @brief Función para configurar el tiempo de muestreo.
 * @param cicles Indica el número de ciclos del proceso de muestreo [2|4|8|16]
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_set_sampling_time(unsigned short cicles, unsigned short port);

/**
 * @brief Permite activar o desactivar la lectura en pines sucesivos
 * @param activate Parámetro para indicar la operación : true=activar   false=desactivar.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
inline void atd_read_multiple_pin(bool activate, unsigned short port);

/**
 * @brief Función para configurar el pin de inicio.
 * @param pin Parámetro para indicar el pin que se desea emplear.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
inline void atd_set_start_pin(unsigned short pin, unsigned short port);

/**
 * @brief Función para configurar el reloj del submódulo.
 * @param val Seleccionador de frecuencia.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_set_submodule_clock(byte val, unsigned short port);

/**
 * @brief Función para activar o desactivar el modo FIFO en el guardado de los resultados.
 * @param on Parámetro para indicar la operación : true=activar false=desactivar.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_set_FIFO(bool on, unsigned short port);

/**
 * @brief Función que espera hasta que se finalice la conversión.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_wait_for_conversor(unsigned short port);

/**
 * @brief Función para la devolución de los valores leídos.
 * @param data  Array en el que se almacenarán los valores leidos.
 * @param n Dirección de la variable que contendrá el número de conversiones realizadas.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_get_data(unsigned short * data, word * n , unsigned short port);

/**
 * @brief Función para determinar el módo de puesta a 0 de los indicadore de conversión completa.
 * @param ror Parámetro para indicar la acción a realizar :
 * true=Basta con acceder al registro de los datos.
 * false=Necesario acceder primero al registro de control y después al de datos.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_reset_on_read(bool ror, unsigned short port);

/**
 * @brief Función configurar el modo de justificación de los datos. (afecta en el momento de leer los registros de resultados)
 * @param left Parámetro con el que se decide la justificación a emplear :
 * true=Se aplicará justificación a la izquierda.
 * false=Se aplicará justificación a la derecha.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_align_left(bool left, unsigned short port);

/**
 * @brief Función para iniciar la conversión.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
bool atd_start_conversion(unsigned short port);

/**
 * @brief Función para configurar el número de conversiones sucesivas.
 * @param conversions Prámetro con el que se indica el número de conversiones sucesivas (1|4|8). En caso de que no se especifique un valor
 * adecuado se usará por defecto 4.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_set_successive_conversions(unsigned short conversions, unsigned short port);

/**
 * @brief Función para activar el módulo.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_activate_module(unsigned short port);

/**
 * @brief Función para desactivar el módulo.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_deactivate_module(unsigned short port);

/**
 * @brief Función para activar el modo de configuración contínua (SCAN).
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_activate_continuous_conversion(unsigned short port);

/**
 * @brief Función para desactivar el modo de configuración contínua (SCAN).
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_deactivate_continuous_conversion(unsigned short port);

/**
 * @brief Función para la habilitación de la interrupción cuando se completa la secuencia.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_activate_interrupt(unsigned short port);

/**
 * @brief Función para la deshabilitación de la interrupción cuando se completa la secuencia.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_deactivate_interrupt(unsigned short port);

/**
 * @brief Función que probee de una configuración completa por defecto. Características: Interrupciones desactivadas, resolución a 8 bits,
 * 8 ciclos de tiempo de muestreo, 8 conversiones sucesivas, justificación a la izquierda, activado el reseteo al leer, lectura del pin 0,
 * seleccionador de frecuencia a 4, modo FIFO desactivado, modo SCAN desactivado y lectura de múltiples pines desactivada.
 * @param port Indica el puerto sobre el que se aplicará la acción.
 */
void atd_default_config(unsigned short port);


/*@}*/

#endif


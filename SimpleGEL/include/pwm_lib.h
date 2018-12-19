

#ifndef _PWD_LIB_H
#define _PWD_LIB_H

/*! @defgroup pwdlib Librería PWD Grupo 2

*/
/*@{*/

#include <sys/ports.h>
#include <sys/param.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <sys/interrupts.h>

/* Herzios del reloj e_clock es de 2Mhz */

/* define PWCLK
#define M6812B_PCKB0 (1 << 0 )
#define M6812B_PCKB1 (1 << 1 )
#define M6812B_PCKB2 (1 << 2 )
#define M6812B_PCKA0 (1 << 3 )
#define M6812B_PCKA1 (1 << 4 )
#define M6812B_PCKA2 (1 << 5 )
#define M6812B_CON01 (1 << 6 )
#define M6812B_CON23 (1 << 7 )
*/

/* define PWPOL
#define M6812B_PPOL0 (1 << 0 )
#define M6812B_PPOL1 (1 << 1 )
#define M6812B_PPOL2 (1 << 2 )
#define M6812B_PPOL3 (1 << 3 )
#define M6812B_PCLK0 (1 << 4 )
#define M6812B_PCLK1 (1 << 5 )
#define M6812B_PCLK2 (1 << 6 typedef unsigned char byte;)
#define M6812B_PCLK3 (1 << 7 )
*/

/* define PWEN
#define M6812B_PWEN0 (1 << 0 )
#define M6812B_PWEN1 (1 << 1 )
#define M6812B_PWEN2 (1 << 2 )
#define M6812B_PWEN3 (1 << 3 )
*/

/* define PWCTL
#define M6812B_CENTR (1 << 3 )
*/

/* define program constants */
#define totalBits 8

//PRECISION
#define PRECISION 32

//M6812_CPU_E_CLOCK=8.000.000hz=8Mhz
#define MAX_FREC_PRESCALER1 62500
#define MAX_PRESCALAR1_VALOR 128
#define MAX_PRESCALAR2_VALOR 256
#define VALOR_TOTAL_DIVISOR 32768


/** returns a divider given a frequency*/
inline void divider_calc (unsigned long frequency);

/** CHANGES ACTUAL CHANNEL: low-high polarity if 0, high-low polarity otherwise */
inline void polarity (unsigned int pol);

/** left-aligned if 0, center-aligned otherwise */
inline void alignment (unsigned int align);

/** CHANGES ACTUAL CHANNEL: set PWPER directly */
void set_PWPER (unsigned int period);

/** CHANGES ACTUAL CHANNEL: set PWDTY directly */
void set_PWDTY (unsigned int period);

/** CHANGES ACTUAL CHANNEL: set duty work by percentage -> 0 1 2 3 4 representating 0 25 50 75 100% */
void set_duty_percentage (unsigned int percent);

/** set current channel */
void set_channel (unsigned int channel);

/** unables all channels */
inline void reset_channel ();

/** a quick method to set up the PWM */
void pwd_generalizado(unsigned int channel, unsigned long frec, unsigned int pol);

/*@}*/

#endif

/* sys/sio.h -- Utility methods to read/write the SIO
   Copyright 2000 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@worldnet.fr)

This file is part of GDB, GAS, and the GNU binutils.

GDB, GAS, and the GNU binutils are free software; you can redistribute
them and/or modify them under the terms of the GNU General Public
License as published by the Free Software Foundation; either version
1, or (at your option) any later version.

GDB, GAS, and the GNU binutils are distributed in the hope that they
will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this file; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _SYS_SIO_H
#define _SYS_SIO_H

#include <sys/param.h>
#include <sys/ports.h>

#ifdef mc6811
# include <asm-m68hc11/sio.h>
#endif

#ifdef mc6812
# include <asm-m68hc12/sio.h>
#endif

extern void serial_init (void);

/* Return != 0 if there is something to read on the serial line.  */
extern unsigned char serial_receive_pending (void);

/* Wait until the SIO has finished to send the character.  */
extern void serial_flush (void);

/* Send the character on the serial line.  */
extern void serial_send (char c);

/* Wait for a character on the serial line and return it.  */
extern unsigned char serial_recv (void);

/** Write the string on the serial line.

    @param msg null terminated string to write.

    @see serial_init, serial_send
*/
extern void serial_print (const char *msg);

/** Wait for a string from serial line.

    @param msg buffer that will hold the string.

    @see serial_init, serial_recv
*/
extern void serial_getline (char *buf);

/** Espera 8 bits (0s o 1s) por la serial para formar un byte
    Permite borrar y termina con \n
*/
extern unsigned char serial_getbinbyte();

/** Saca por la serial la configuración de 8 bits (0s y 1s) del byte pasado

    @param bt byte a sacar
*/
extern void serial_printbinbyte(unsigned char bt);

/** Saca por la serial la configuración de 16 bits (0s y 1s) del doble byte pasado

    @param sa doble byte a sacar
*/
extern void serial_printbinword(unsigned short sa);

/** Espera 2 dígitos exadecimales para formar byte. Permite borrar
    y termina con \n
*/
extern unsigned char serial_gethexbyte();

/** Espera 4 dígitos exadecimales para formar doble byte. Permite borrar
    y termina con \n
*/
extern unsigned short serial_gethexword();

/** Saca por la serial en hexadecimal el byte pasado 
    @param bt byte a sacar
*/
extern void serial_printhexbyte(unsigned char bt);

/** Saca por la serial en hexadecimal el doble byte pasado 
    @param sa doble byte a sacar
*/
extern void serial_printhexword(unsigned short sa);

/** Espera por la serial digitos decimales para formar byte sin signo.
    Permite borrar y termina con \n. Controla desboradmiento.
*/
extern unsigned char serial_getdecbyte();

/** Espera por la serial digitos decimales para formar doble byte sin signo.
    Permite borrar y termina con \n. Controla desboradmiento.
*/
extern unsigned short serial_getdecword();

/** Saca por la serial en decimal doble byte pasado.
    @param sa doble byte sin signo
*/
extern void serial_printdecword(unsigned short sa);

/** Saca por la serial en decimal byte pasado.
    @param ba byte sin signo
*/
extern void serial_printdecbyte(unsigned char ba);


#endif /* _SYS_SIO_H */


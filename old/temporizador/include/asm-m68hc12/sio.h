/* asm-m68hc12/sio.h -- Utility methods to read/write the SIO
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

#ifndef _M68HC12_SIO_H
#define _M68HC12_SIO_H

#if M6812_STDOUT_PORT == 0
# define M6812_SCBD   M6812_SC0BDH
# define M6812_SCCR1  M6812_SC0CR1
# define M6812_SCCR2  M6812_SC0CR2
# define M6812_SCSR1  M6812_SC0SR1
# define M6812_SCDR   M6812_SC0DRL
#elif M6812_STDOUT_PORT == 1
# define M6812_SCBD   M6812_SC1BDH
# define M6812_SCCR1  M6812_SC1CR1
# define M6812_SCCR2  M6812_SC1CR2
# define M6812_SCSR1  M6812_SC1SR1
# define M6812_SCDR   M6812_SC1DRL
#else
# error "M6812_STDOUT_PORT must be defined to either 0 or 1"
#endif

extern inline void
serial_init (void)
{
  ((unsigned volatile short*) &_io_ports[M6812_SCBD])[0] = M6812_DEF_BAUD;

  /* Setup character format 1 start, 8-bits, 1 stop.  */
  _io_ports[M6812_SCCR1] = 0;

  /* Enable reciever and transmitter.  */
  _io_ports[M6812_SCCR2] = 0xc;
}

/* Return != 0 if there is something to read on the serial line.  */
extern inline unsigned char
serial_receive_pending (void)
{
  return _io_ports[M6812_SCSR1] & M6812B_RDRF;
}

/* Wait until the SIO has finished to send the character.  */
extern inline void
serial_flush (void)
{
  while (!(_io_ports[M6812_SCSR1] & M6812B_TDRE))
    cop_optional_reset ();
}

/* Send the character on the serial line.  */
extern inline void
serial_send (char c)
{
  serial_flush ();
  _io_ports[M6812_SCDR] = c;
  _io_ports[M6812_SCCR2] |= M6812B_TE;
}

/* Wait for a character on the serial line and return it.  */
extern inline unsigned char
serial_recv (void)
{
  while (!(_io_ports[M6812_SCSR1] & M6812B_RDRF))
    cop_optional_reset ();

  return _io_ports[M6812_SCDR];
}

#endif /* _M68HC12_SIO_H */


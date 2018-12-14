/* Serial line operations
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@worldnet.fr)	

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include <sys/sio.h>

void
serial_print (const char *msg)
{
  while (*msg != 0)
    serial_send (*msg++);
}

void
serial_getline (char *buf)
{
  char c;

  do
    {
      c = serial_recv ();
      *buf++ = c;
    }
  while (c != '\n' && c != '\r');
  *buf = 0;
}



unsigned char serial_getbinbyte() {
  char c;
  unsigned char numC=0;
  unsigned char tot=0;

  do {
    c=serial_recv();
    if(numC<8 && (c=='0' || c=='1')) {
      tot=(tot<<1)+(c-'0');
      serial_send(c);
      numC++;
    }
    if(numC>0 && c=='\b') {
      /*se pulsó borrar */
      serial_send('\b');
      serial_send(' ');
      serial_send('\b');
      tot=tot>>1; /*quitamos último bit entrado*/
      numC--;
    }
  } while (numC==0 || c!='\r');
  return tot;
}

void serial_printbinbyte(unsigned char bt) {
  unsigned char ba=8;
  while (ba>0) {
    serial_send(bt&0x80?'1':'0');
    bt<<=1;
    ba--;
  }
}

void serial_printbinword(unsigned short sa) {
  serial_printbinbyte((unsigned char)(sa>>8));
  serial_send('.');
  serial_printbinbyte((unsigned char)(sa&0xff));
}



unsigned char serial_gethexbyte() {
  char c,cM;
  unsigned char numC=0;
  unsigned char tot=0;

  do {
    c=serial_recv();

    if(numC<2 && 
       (
        (c>='0' && c<='9') 
        ||((cM=c&(~0x20))>='A' && cM<='F')
        )
       ) {
      tot=(tot<<4)+((c>'9')?cM-'A'+10:c-'0');
      serial_send(c);
      numC++;
    }
    if(numC>0 && c=='\b') {
      /*se pulsó borrar */
      serial_send('\b');
      serial_send(' ');
      serial_send('\b');
      tot=tot>>4; /*quitamos último bit entrado*/
      numC--;
    }
  } while (numC==0 || c!='\r');
  return tot;
}

unsigned short serial_gethexword() {
  char c,cM;
  unsigned char numC=0;
  unsigned short tot=0;

  do {
    c=serial_recv();

    if(numC<4 && 
       (
        (c>='0' && c<='9') 
        ||((cM=c&(~0x20))>='A' && cM<='F')
        )
       ) {
      tot=(tot<<4)+((c>'9')?cM-'A'+10:c-'0');
      serial_send(c);
      numC++;
    }
    if(numC>0 && c=='\b') {
      /*se pulsó borrar */
      serial_send('\b');
      serial_send(' ');
      serial_send('\b');
      tot=tot>>4; /*quitamos último bit entrado*/
      numC--;
    }
  } while (numC==0 || c!='\r');
  return tot;
}

void serial_printhexbyte(unsigned char bt) {
  char DigiHex[]="0123456789ABCDEF";
  serial_send(DigiHex[bt>>4]);
  serial_send(DigiHex[bt&0x0f]);
}

void serial_printhexword(unsigned short sa) {
  serial_printhexbyte((unsigned char)(sa>>8));
  serial_printhexbyte((unsigned char)(sa&0xff));
}


unsigned char serial_getdecbyte()
{
  char c;
  unsigned char numC;
  unsigned short tot,nTot;
  tot=0;
  numC=0;
  do {
    c=serial_recv();
    if(c>='0' && c<='9') {
      nTot=(tot*10)+(c-'0');
      if(nTot<=255) {
        /* Admitimos el caracter*/
        serial_send(c);
        tot=nTot;
        numC++;
      }
    }
    if(numC>0 && c=='\b') {
      /*se pulsó borrar */
      serial_send('\b');
      serial_send(' ');
      serial_send('\b');
      tot=tot/10; /*quitamos último digito entrado*/
      numC--;
    }
    
  } while(numC==0 || c!='\r');
  return tot;
}

unsigned short serial_getdecword()
{
  char c;
  unsigned char numC;
  unsigned long tot,nTot;
  tot=0;
  numC=0;
  do {
    c=serial_recv();
    if(c>='0' && c<='9') {
      nTot=(tot*10)+(c-'0');
      if(nTot<=0xFFFF) {
        /* Admitimos el caracter*/
        serial_send(c);
        tot=nTot;
        numC++;
      }
    }
    if(numC>0 && c=='\b') {
      /*se pulsó borrar */
      serial_send('\b');
      serial_send(' ');
      serial_send('\b');
      tot=tot/10; /*quitamos último digito entrado*/
      numC--;
    }
    
  } while(numC==0 || c!='\r');
  return tot;
}

void serial_printdecword(unsigned short sa) {
  char dg[5];
  unsigned char da=0;
  while(sa!=0) {
    dg[da]=(sa%10)+'0';
    da++;
    sa=sa/10;
  }
  if(da==0) /*vale 0*/
    serial_send('0');
  else
    for(;da>0;da--)
      serial_send(dg[da-1]);
}

void serial_printdecbyte(unsigned char ba) {
  serial_printdecword((unsigned short)ba);
}

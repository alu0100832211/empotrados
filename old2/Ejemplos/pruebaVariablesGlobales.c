/* ****************************************
* Salida onda cuadrada por el OC2
* Se especifica periondo en variable.

   Copyright (C) Alberto F. Hamilton Castro
   Dpto. de Ingeniería Informática de Sistemas
   Universidad de La Laguna

  Licencia: GPLv3

  *************************************** */


#include <sys/interrupts.h>
#include <sys/sio.h>

typedef unsigned char byte;  /*por comodidad*/
typedef unsigned short word;  /*por comodidad*/

word vGlob1 = 0xaaaa;

word vGlob2 = 0x5555;


int main () {


  serial_print("\n\r pruebaVariableGlobales ====");

  while(1) {
  char c;
  c = serial_recv();
  //Hacemos eco del caracter recibido
  serial_send(c);

  serial_print("\n\rLa primera variable vale: ");
  serial_printhexword(vGlob1);

  serial_print("\n\rLa segunda variable vale: ");
  serial_printhexword(vGlob2);

  vGlob2++;


  }
}


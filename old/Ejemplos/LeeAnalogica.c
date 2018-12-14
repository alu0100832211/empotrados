
/* ****************************************
 * Lee una entrada analógica y muestra valor por
 * la serial.
 * Al inicio permite elegir en tiempo de ejecución el puerto
 * y el pin dentro del puerto a utilizar
 * No utiliza interrupciones sino polling

  Copyright (C) Alberto F. Hamilton Castro
  Dpto. de Ingeniería Informática de Sistemas
  Universidad de La Laguna

  Licencia: GPLv3

  *************************************** */


#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

typedef unsigned char byte;  /*por comodidad*/
typedef unsigned short word;  /*por comodidad*/

int main () {
  char c;
  /*Diferencial a usar en los direccionamientos para distinguir puerto 0 y 1 */
  word DirAD;
  byte Pin; /*Pin dentro del puerto del que se va a leer */

  word resultadoAnterior = 0;

  /* Deshabilitamos interrupciones */
  lock ();

  /*Encendemos led*/
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;


  serial_init();
  serial_print("\nLeeAnalogica.c ===========\n");

  /* Elección del puerto */
  serial_print("\nPuerto conversor a utilizar (0 - 1)?:");
  while((c = serial_recv()) != '0' && c != '1');
  serial_send(c); /* a modo de confirmación*/
  DirAD = (c = '0')?0:(M6812_ATD0CTL1 - M6812_ATD0CTL0);

  /* Elección del pin dentro del puerto */
  serial_print("\nPin del puerto a utilizar (0 - 7)?:");
  while((c = serial_recv()) < '0' && c > '7');
  serial_send(c); /* a modo de confirmación*/
  Pin = c - '0';

  /*Pasamos a configurar AD correspondiente*/
  _io_ports[M6812_ATD0CTL2 + DirAD] = M6812B_ADPU; /*Encendemos, justf. izda*/
  _io_ports[M6812_ATD0CTL3 + DirAD] = 0; /*Sin fifo*/

  /* resolución de 10 bits y 16 ciclos de muestreo */
  _io_ports[M6812_ATD0CTL4 + DirAD] = M6812B_RES10 | M6812B_SMP0 | M6812B_SMP1;

  /* Modo scan con 8 resultados sobre el pin seleccionado */
  _io_ports[M6812_ATD0CTL5 + DirAD] = M6812B_SCAN | M6812B_S8C | Pin;


  serial_print("\nTerminada inicialización\n");

  while(1) {
    word resultado;
    byte iguales, i;

    /* Esperamos a que se termine la conversión */
    while(! (_io_ports[M6812_ATD0STAT0 + DirAD] & M6812B_SCF));

    /*Invertimos el led*/
    _io_ports[M6812_PORTG] ^= M6812B_PG7;

    /*Vemos si los 8 resultados son iguales */
    resultado = _IO_PORTS_W(M6812_ADR00H + DirAD);
    iguales = 1;
    for(i = 0; iguales && i < 8; i++)
      iguales = resultado == _IO_PORTS_W(M6812_ADR00H + DirAD + 2 * i);
    if(! iguales)
      continue;
    if (resultado == resultadoAnterior)
      continue;

    /* Los 8 resultados son iguales y distintos a lo que teníamos antes*/
    serial_print("\nNuevo valor = ");
    serial_printdecword(resultado);
    resultadoAnterior = resultado;
  }
}

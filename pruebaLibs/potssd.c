/* Programa que representa el valor analógico del potenciometro
 * en los 4 ssd */
#include <ssd.h>
#include <atd.h>

#define PUERTO_ATD 0

int main (void){
  sieteSeg_init();
  serial_init();
  atd_default_config(PUERTO_ATD);
  atd_activate_module(PUERTO_ATD);
  unsigned short data = 0, last_data = 0;
  
  while(1){
    /* Esperar a que termine la conversión */
    atd_wait_for_conversor(PUERTO_ATD);
    /* Devolver los valores leidos */
    unsigned short nConversiones;
    last_data = data;
    atd_get_data(&data, &nConversiones, PUERTO_ATD);
    data = (last_data + data) / 2;
    delayusg(500000UL); 
    serial_printdecword(data);
    serial_print("\n");
    //serial_recv();
    sieteSeg_valor(data);
  }
}

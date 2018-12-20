/* Libreria para usar siete segmentos (Seven Segment Display) */
#ifndef SSD_H
#define SSD_H

#include <temporizador.h>
#include <e_s_lib.h>
#include <atd.h>

void sieteSeg_init();
void sieteSeg_digitos(unsigned char* digito);
void sieteSeg_valor(unsigned int numero);

#endif

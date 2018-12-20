#ifndef TECLADO_H
#define TECLADO_H
#include <temporizador.h>
#include <e_s_lib.h>

void teclado_init();
char teclado_getch();
char teclado_getch_timeout(unsigned int);

#endif

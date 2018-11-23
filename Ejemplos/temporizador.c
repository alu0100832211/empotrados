/**********************************************

  Librería para realizar un temporizador

***********************************************/



#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>


typedef unsigned char byte;

unsigned long nDesbordamientos = 0UL;

void __attribute__((interrupt)) vi_tov(void) {
	nDesbordamientos++;
	//TFLG2
	//TSCR:TFFCA esta inicializado a 0
	_io_ports[M6812_TFLG2] &= ~M6812B_TOF
}

void init_temporizador(void){ //futuro: en vez de void int factor
	//inicializar flag
	_io_ports[M6812_TSCR] &= ~M6812B_TFFCA;
	//poner contador a 0
	_IO_PORTS_W(M6812_TCNT) = 0;
	//habilitar temporizador por si no lo está
	_io_ports[M6812_TSCR] |= M6812B_TEN;
	/* Desconectamos para no afectar al pin */
	_io_ports[M6812_TCL1] &= ~(M6812B_OM6 | M6812B_OL6);
	//Darle factor al reloj lo mas lento posible 111 = 7
	_io_ports[M6812_TMSK2] = (M6812B_PR2 | M6812B_PR1 | M6812B_PR0);
	//Habilitar las interrupciones de desbordamiento
	_io_ports[M6812_TMSK2] |= M6812B_TOI;
}

//Devuelve los microsegundos quehan pasado
unsigned long get_microsenconds(void){
	//Lo que hay en TCNT son el nº de ciclos que ha pasado
	//Depende del factor el tiempo de ciclo es diferente
	//Multiplicar nº de ciclos totales por tiempo de ciclo
	//nº de ciclos totales será 2¹⁶ * nDesbordamientos
	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	unsigned long frec = M6812_CPU_E_CLOCK/(1<<factorT);		
	unsigned long  numCiclosL = (nDesbordamientos << 16) | _IO_PORTS_W();
}

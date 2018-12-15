
#include <temporizador.h>

void funcionEjemplo(void * args){
	serial_print("a\n");
}

void funcionEjemplo2(void * args){
  unsigned char * byte;
  args = (unsigned char*) args

	serial_print(byte);
}

int main () {
	lock ();

	_io_ports[M6812_DDRG] |= M6812B_PG7;
	_io_ports[M6812_PORTG] |= M6812B_PG7;

	serial_init();
	serial_print("\nDelayUsg.c ============\n");

	init_temporizador(7);

	unlock();

	serial_print("\n\rTerminada inicialización\n");

  unsigned char * byte = (unsigned char*)malloc(sizeof(unsigned char));
	runAfterUsg(funcionEjemplo, NULL, 1000UL*2000UL);
	runEveryUsg(funcionEjemplo2, , 1000UL*3000UL);

	delayusg(1000UL*1000UL);
	print4bWord(get_microseconds());

	while(1){
		_io_ports[M6812_PORTG] ^= M6812B_PG7;
		print4bWord(get_microseconds());
		delayusg(5000UL * 1000UL);
	}
}

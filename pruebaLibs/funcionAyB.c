
#include <temporizador.h>

void funcionEjemplo(void){
	serial_print("a\n");
}

void funcionEjemplo2(void){
	serial_print("b\n");
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

	runAfterUsg(&funcionEjemplo, 20000UL * 1000UL);
	runEveryUsg(&funcionEjemplo2, 1000UL * 1000UL);

	delayusg(1000UL*1000UL);
	serial_printdeclong(get_microseconds());
  serial_send('\n');

	while(1){
		_io_ports[M6812_PORTG] ^= M6812B_PG7;
		serial_printdeclong(get_microseconds());
    serial_send('\n');
		delayusg(5000UL * 1000UL);
	}
}

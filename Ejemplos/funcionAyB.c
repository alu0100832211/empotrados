#include "temporizador.h"

int main () {

	/* Deshabilitamos interrupciones */
	lock ();

	/*Inicialización del Temporizador */
	/* podemos poner cualquier valor de escalado de 0 a 7
	ya que la función se adapta el valor configurad*/
	//_io_ports[M6812_TMSK2] = 7;

	/*Encendemos led*/
	_io_ports[M6812_DDRG] |= M6812B_PG7;
	_io_ports[M6812_PORTG] |= M6812B_PG7;

	serial_init();
	serial_print("\nDelayUsg.c ============\n");

	init_temporizador();

	unlock();  /* habilitamos interrupciones */

	serial_print("\n\rTerminada inicialización\n");


	print4bWord(get_microseconds());
	runAfterUsg(&funcionEjemplo, 1000UL*2000UL);
	runEveryUsg(&funcionEjemplo2, 1000UL*3000UL);
	serial_print("Si la a y la b se imprime abajo (tras 2 segundos) está funcionando\n");
	delayusg(1000UL*1000UL);
	print4bWord(get_microseconds());


	while(1){
		/*Invertimos el led*/
		_io_ports[M6812_PORTG] ^= M6812B_PG7;

		print4bWord(get_microseconds());

		delayusg(5000UL * 1000UL);
	}


	//serial_printdecword(get_microseconds());
	/*Invertimos el led*/
	//_io_ports[M6812_PORTG] ^= M6812B_PG7;

	//delayusg(1000UL * 1000UL);
}

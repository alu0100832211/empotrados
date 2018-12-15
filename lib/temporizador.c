/**********************************************

Librería para realizar un temporizador

***********************************************/

#include <temporizador.h>

bytes4 nDisparosOC0_inicial;
bytes4 nDisparosOC0 = 0;
bytes4 nDisparosOC1 = 0;

bytes4 nDesbordamientos = 0UL;

void * runAfterUsg_args;
void * runEveryUsg_args;
void (*runAfterUsg_f)(void *);
void (*runEveryUsg_f)(void *);

void __attribute__((interrupt)) vi_tov(void) {
	nDesbordamientos++;
	_io_ports[M6812_TFLG2] = M6812B_TOF;
}

void __attribute__((interrupt)) vi_ioc0(void){
	if(nDisparosOC0 == 0){
		runEveryUsg_f(runEveryUsg_args);
		nDisparosOC0 = nDisparosOC0_inicial;
	}
	else{
		nDisparosOC0--;
	}
	_io_ports[M6812_TFLG1] = M6812B_C0F;
}

void __attribute__((interrupt)) vi_ioc1(void) {
	if(nDisparosOC1 == 0){
		runAfterUsg_f(runAfterUsg_args);
		_io_ports[M6812_TMSK1] &= ~M6812B_C1I;
	}
	else{
		nDisparosOC1--;
	}
	_io_ports[M6812_TFLG1] |= M6812B_C1F;
}

/**
* @brief Inicializa los flags del contador. Cada vez que se llama se resetea el contador.
* @param factorT Factor de escalado del procesador. Va de 0 a 7. La frecuencia base del reloj (8MHz) se divide por la potencia de dos elevado al factor de escalado. Ejemplo factoT = 7  8MHz/2⁷ = 16μsg
* IMPORTANTE: De momento, usar un factor distinto de 7 está dando problemas para calcular el tiempo
*/
void init_temporizador(int factorT){
	if (factorT > 7){
		factorT = 7;
	}
	if (factorT < 0){
		factorT = 0;
	}

	_io_ports[M6812_TSCR] &= ~M6812B_TFFCA;
	_io_ports[M6812_TSCR] |= M6812B_TEN;
	_io_ports[M6812_TMSK2] |= factorT;
	_io_ports[M6812_TMSK2] |= M6812B_TOI;

	_IO_PORTS_W(M6812_TCNT) = 0;
}

/**
* @brief Tiempo en microsegundos desde la llamada a init
* Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @return 4bytes que representan el valor hexadecimal
*/
bytes4 get_microseconds(void){
	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	bytes4  numCiclos = (nDesbordamientos << 16) | _IO_PORTS_W(M6812_TCNT);
	int desp = 3 - factorT;
	if (desp > 0){
		return numCiclos >> desp;
	}
	else{
		desp *= -1;
		return numCiclos << desp;
	}
}

/**
* @brief Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @return 4 bytes que representan el valor hexadecimal
*/
bytes4 get_miliseconds(void){
	return get_microseconds() * 1000;
}

/**
* @brief Espera un tiempo bloqueando la ejecución del programa
* No hay detalles
* @param useg valor decimal en microsegundos que espera la funcion
*/
void delayusg(unsigned long useg) {
	unsigned int numCiclos;
	unsigned long numCiclosL;

	_io_ports[M6812_TCTL1] &= ~(M6812B_OM6 | M6812B_OL6);

	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT);
	if(frec/1000000)
	  numCiclosL = frec/1000000 * useg;
	else
	  numCiclosL = frec/100 * useg/10000;

	unsigned int numDisparos = numCiclosL >> 16;
	numCiclos = numCiclosL & 0xffff;

	if(frec/1000000)
	if((numCiclos == 0) && (numDisparos == 0)) numCiclos = 1;

	_io_ports[M6812_TIOS] |= M6812B_IOS6;
	_io_ports[M6812_TFLG1] = M6812B_C6F;
	_IO_PORTS_W(M6812_TC6) = _IO_PORTS_W(M6812_TCNT) + numCiclos ;

	do {
		while (! (_io_ports[M6812_TFLG1] & M6812B_C6F));
		_io_ports[M6812_TFLG1] = M6812B_C6F;
	} while(numDisparos--);
}

/**
* @brief Imprime 4 bytes por pantalla en formato hexadecimal
*/
void print4bWord(bytes4 word){
	bytes2 m1 = word >> 16;
	bytes2 m2 = word;

	serial_print("0x");
	serial_printhexword(m1);
	serial_printhexword(m2);
	serial_print("\n");
}


/**
* @brief Ejecuta una función después de un tiempo determiando
* Ejemplo de uso en 
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runAfterUsg(void (*f)(void *), void * args, bytes4 useg){
	runAfterUsg_f = f;
  runAfterUsg_args = args;

	bytes2 numCiclos;
	bytes4 numCiclosL;
	_io_ports[M6812_TCTL2] &= ~(M6812B_OM1 | M6812B_OL1);
	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT);
	if(frec/1000000)
		numCiclosL = frec/1000000 * useg;
	else
		numCiclosL = frec/100 * useg/10000;
	nDisparosOC1 = numCiclosL >> 16;
	numCiclos = numCiclosL & 0xffff;

	if((numCiclos == 0) && (nDisparosOC1 == 0)) numCiclos = 1;

	_io_ports[M6812_TIOS] |= M6812B_IOS1;
	_io_ports[M6812_TFLG1] = M6812B_C1F;
	_IO_PORTS_W(M6812_TC1) = _IO_PORTS_W(M6812_TCNT) + numCiclos;
	_io_ports[M6812_TMSK1] |= M6812B_C1I;
}

/**
* @brief Ejecuta una función periodicamente
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runEveryUsg(void (*f)(void*), void * args, bytes4 useg){
	runEveryUsg_f = f;
  runEveryUsg_args = args;

	bytes2 numCiclos;
	bytes4 numCiclosL;
	_io_ports[M6812_TCTL2] &= ~(M6812B_OM0 | M6812B_OL0);
	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT);
	if(frec/1000000)
		numCiclosL = frec/1000000 * useg;
	else
		numCiclosL = frec/100 * useg/10000;

	nDisparosOC0 = numCiclosL >> 16;
	nDisparosOC0_inicial = nDisparosOC0;
	numCiclos = numCiclosL & 0xffff;

	if((numCiclos == 0) && (nDisparosOC0 == 0)) numCiclos = 1;

	_io_ports[M6812_TIOS] |= M6812B_IOS0;
	_io_ports[M6812_TFLG1] = M6812B_C0F;
	_IO_PORTS_W(M6812_TC0) = _IO_PORTS_W(M6812_TCNT) + numCiclos ;
	_io_ports[M6812_TMSK1] |= M6812B_C0I;
}

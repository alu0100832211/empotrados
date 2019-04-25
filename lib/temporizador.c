/**********************************************

Librería para realizar un temporizador

***********************************************/

#include <temporizador.h>

bytes4 nDisparosOC0_inicial;
bytes2 numCiclosOC0;
bytes4 nDisparosOC0 = 0;

bytes4 nDisparosOC1 = 0;

bytes4 nDesbordamientos = 0UL;

void (*runAfterUsg_f)(void);
void (*runEveryUsg_f)(void);

void __attribute__((interrupt)) vi_tov(void) {
	nDesbordamientos++;
	_io_ports[M6812_TFLG2] = M6812B_TOF;
}

void __attribute__((interrupt)) vi_ioc0(void){
	_io_ports[M6812_TFLG1] = M6812B_C0F;
	if(nDisparosOC0 == 0){

		nDisparosOC0 = nDisparosOC0_inicial;
    _IO_PORTS_W(M6812_TC0) = _IO_PORTS_W(M6812_TC0) + numCiclosOC0;
		(*runEveryUsg_f)();
	}
	else{
		nDisparosOC0--;
	}
}

void __attribute__((interrupt)) vi_ioc1(void) {
	if(nDisparosOC1 == 0){
		(*runAfterUsg_f)();
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
* @return
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
* @brief Cuenta el numero de desbordamientos del temporizador y calcula los microsegundos en funcion de la frecuencia del reloj
* @param
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
	return get_microseconds() / 1000;
}

/**
* @brief calcula los disparos y ciclos necesarios para cubrir useg dados
*/
void calculaDisparosCiclos(bytes4 useg, bytes4* disparos, bytes2* ciclos) {
  bytes4 numCiclosL;

  byte factorT = _io_ports[M6812_TMSK2] & 0x07;
  bytes4 frec = M6812_CPU_E_CLOCK/(1 << factorT);

  byte numDivisiones = 6;
  bytes4 frecD = frec;
  while ((numDivisiones > 0) && !(frecD % 10)) {
    frecD /= 10;
    numDivisiones--;
  }
  bytes4 usegD = useg;
  while ((numDivisiones > 0) && (usegD > (0xFFFFFFFF/frecD))) {
    usegD /= 10;
    numDivisiones--;
  }
  numCiclosL = frecD * usegD;

  while (numDivisiones > 0) {
    numCiclosL /= 10;
    numDivisiones--;
  }

  *disparos = numCiclosL >> 16;
  *ciclos = numCiclosL & 0xffff;

  if((*ciclos == 0) && (*disparos == 0)) *ciclos = 1;

}



/**
* @brief Espera un tiempo bloqueando la ejecución del programa
* No hay detalles
* @param useg valor decimal en microsegundos que espera la funcion
*/
void delayusg(unsigned long useg) {

	bytes2 numCiclos;
	bytes4 numDisparos;

  calculaDisparosCiclos(useg, &numDisparos, &numCiclos);

	_io_ports[M6812_TIOS] |= M6812B_IOS6;
	_io_ports[M6812_TFLG1] = M6812B_C6F;
	_IO_PORTS_W(M6812_TC6) = _IO_PORTS_W(M6812_TCNT) + numCiclos ;

	do {
		while (! (_io_ports[M6812_TFLG1] & M6812B_C6F));
		_io_ports[M6812_TFLG1] = M6812B_C6F;
	} while(numDisparos--);
}


/**
* @brief Ejecuta una función después de un tiempo determiando
* @param f Función que se repetirá según el tiempo pasado
* @param useg Tiempo que esperará para ejecutar la función pasada. Este parámetro hay que pasarlo en microsegundos
*/
void runAfterUsg(void (*f)(void), bytes4 useg){
	runAfterUsg_f = f;

	bytes2 numCiclos;

  calculaDisparosCiclos(useg, &nDisparosOC1, &numCiclos);

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
void runEveryUsg(void (*f)(void), bytes4 useg){
	runEveryUsg_f = f;

  calculaDisparosCiclos(useg, &nDisparosOC0, &numCiclosOC0);
  nDisparosOC0_inicial = nDisparosOC0;

	_io_ports[M6812_TIOS] |= M6812B_IOS0;
	_io_ports[M6812_TFLG1] = M6812B_C0F;
	_IO_PORTS_W(M6812_TC0) = _IO_PORTS_W(M6812_TCNT) + numCiclosOC0 ;
	_io_ports[M6812_TMSK1] |= M6812B_C0I;

}

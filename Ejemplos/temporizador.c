/**********************************************

Librería para realizar un temporizador

***********************************************/



#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

//Para pensar mejor
typedef unsigned char byte;
typedef unsigned int bytes2;
typedef unsigned long bytes4;

bytes4 nDisparosOC1 = 0;

bytes4 nDisparosOC0_inicial;
bytes4 nDisparosOC0 = 0;

//Nº máximo desbordamientos 2⁸
bytes4 nDesbordamientos = 0UL;



void (*runAfterUsg_f)(void);
void (*runEveryUsg_f)(void);


void __attribute__((interrupt)) vi_tov(void) {
	nDesbordamientos++;
	//TSCR:TFFCA esta inicializado a 0
	_io_ports[M6812_TFLG2] |= M6812B_TOF;
}

void __attribute__((interrupt)) vi_ioc0(void){
	if(nDisparosOC0 == 0){
		(*runEveryUsg_f)();
		nDisparosOC0 = nDisparosOC0_inicial;
	}
	else{
		nDisparosOC0--;
	}
	_io_ports[M6812_TFLG1] = M6812B_C0F; /*Bajamos el banderín  */

}

void __attribute__((interrupt)) vi_ioc1(void) {
	//Ejecutar la funcion periódica y la que se ejecuta tras un tiempo
	//(*runAfterUsg_f)();
	//Dejar de usar comparador de salida
	if(nDisparosOC1 == 0){
		(*runAfterUsg_f)();
		_io_ports[M6812_TMSK1] &= ~M6812B_C1F; //Dejar de usar interrupciones
	}
	else{
		nDisparosOC1--;
	}

	_io_ports[M6812_TFLG1] = M6812B_C1F; /*Bajamos el banderín  */


}


void init_temporizador(void){ //futuro: en vez de void int factor
	//inicializar flag
	_io_ports[M6812_TSCR] &= ~M6812B_TFFCA;
	//habilitar temporizador por si no lo está
	_io_ports[M6812_TSCR] |= M6812B_TEN;
	//El temporizador se para cuando el sistema entra en wait
	//_io_ports[M6812_TSCR] |= M6812B_TWAI;
	/* Desconectamos para no afectar al pin */
	_io_ports[M6812_TCTL1] &= ~(M6812B_OM6 | M6812B_OL6);
	//Darle factor al reloj lo mas lento posible 111 = 7
	_io_ports[M6812_TMSK2] = (M6812B_PR2 | M6812B_PR1 | M6812B_PR0);
	//Desactivar reseteo contador en disparo de OC7
	//_io_ports[M6812_TMSK2] &= ~M6812B_TCRE;
	//Habilitar las interrupciones de desbordamiento
	_io_ports[M6812_TMSK2] |= M6812B_TOI;
	//poner contador a 0
	_IO_PORTS_W(M6812_TCNT) = 0;
}

//Devuelve los microsegundos quehan pasado
bytes4 get_microseconds(void){
	//Obtenemos el factor de frecuencia
	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	//Numero de ciclos total es el valor actual del contador
	//concatenado por la izquierda con el nº desbordamientos
	//bytes4  numCiclos = (nDesbordamientos << 16) | _IO_PORTS_W(M6812_TCTL1);
	bytes4  numCiclos = (nDesbordamientos << 16) | _IO_PORTS_W(M6812_TCNT);
	//Microsegundos depende del factor
	int desp = 3 - factorT;
	//bytes4 microSeconds;
	if (desp > 0){
		return numCiclos >> desp;
	}
	else{
		desp *= -1;
		return numCiclos << desp;
	}
}

//Devuelve los milisegundos que han pasado
bytes4 get_miliseconds(void){
	//Parar el temporizador hasta volver de esta funcion?
	return get_microseconds() * 1000;
}

//Metodo para realizar esperas solicitadas -> el del ejemplo
void delayusg(unsigned long useg) {
	unsigned int numCiclos;
	unsigned long numCiclosL;

	/* Desconectamos para no afectar al pin */
	_io_ports[M6812_TCTL1] &= ~(M6812B_OM6 | M6812B_OL6);

	/* Vemos velocidad del temporizador*/
	byte factorT = _io_ports[M6812_TMSK2] & 0x07; /*Factor de escalado actual*/
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT); /* Frecuencia del temporizador*/
	/* Según la frecuencia elegimos el modo de dividir para evitar desbordamientos */
	if(frec/1000000)
	numCiclosL = frec/1000000 * useg;
	else
	numCiclosL = frec/100 * useg/10000;

	unsigned int numDisparos = numCiclosL >> 16;  /* Numero de disparos necesarios */
	numCiclos = numCiclosL & 0xffff; /* Número restante de ciclos */

	/* Por si escalado muy grande y useg pequeño */
	if((numCiclos == 0) && (numDisparos == 0)) numCiclos = 1;

	_io_ports[M6812_TIOS] |= M6812B_IOS6; /*configuramos canal como comparador salida*/
	_io_ports[M6812_TFLG1] = M6812B_C6F; /*Bajamos el banderín  */
	/*preparamos disparo*/
	_IO_PORTS_W(M6812_TC6) = _IO_PORTS_W(M6812_TCNT) + numCiclos ;

	/*Habilitamos el temporizador, por si no lo está*/
	_io_ports[M6812_TSCR] |= M6812B_TEN;

	/* Esparamos los desboradmientos necesarios */
	do {
		/* Nos quedamos esperando a que se produzca la igualdad*/
		while (! (_io_ports[M6812_TFLG1] & M6812B_C6F));
		_io_ports[M6812_TFLG1] = M6812B_C6F; /* Bajamos el banderín */
	} while(numDisparos--);
}

//Mostrar por pantalla
void print4bWord(bytes4 word){
	bytes2 m1 = word >> 16;
	bytes2 m2 = word;

	serial_print("0x");
	serial_printhexword(m1);
	serial_printhexword(m2);
	serial_print("\n");
}

//Funcion que se ejecuta tras un tiempo determinado
void runAfterUsg(void (*f)(void), bytes4 useg){
	//guardar function en variable global
	runAfterUsg_f = f;

	bytes2 numCiclos;
	bytes4 numCiclosL;
	//Poner delay en el comparador de salida
	_io_ports[M6812_TCTL2] &= ~(M6812B_OM1 | M6812B_OL1);
	/* Vemos velocidad del temporizador*/
	byte factorT = _io_ports[M6812_TMSK2] & 0x07; /*Factor de escalado actual*/
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT); /* Frecuencia del temporizador*/
	/* Según la frecuencia elegimos el modo de dividir para evitar desbordamientos */
	if(frec/1000000)
	numCiclosL = frec/1000000 * useg;
	else
	numCiclosL = frec/100 * useg/10000;

	//NDisparosOC1 = variable global
	nDisparosOC1 = numCiclosL >> 16;  /* Numero de disparos necesarios */
	numCiclos = numCiclosL & 0xffff; /* Número restante de ciclos */

	/* Por si escalado muy grande y useg pequeño */
	if((numCiclos == 0) && (nDisparosOC1 == 0)) numCiclos = 1;

	_io_ports[M6812_TMSK1] |= M6812B_C1I;
	_io_ports[M6812_TIOS] |= M6812B_IOS1; /*configuramos canal como comparador salida*/
	_io_ports[M6812_TFLG1] = M6812B_C1F; /*Bajamos el banderín  */
	/*preparamos disparo*/
	_IO_PORTS_W(M6812_TC1) = _IO_PORTS_W(M6812_TCNT) + numCiclos ;

	//Cuando se dispare

}
//Funcion que se ejecuta periódicamente
void runEveryUsg(void (*f)(void), bytes4 useg){
	//guardar function en variable global
	runEveryUsg_f = f;

	bytes2 numCiclos;
	bytes4 numCiclosL;
	//Poner delay en el comparador de salida
	_io_ports[M6812_TCTL2] &= ~(M6812B_OM0 | M6812B_OL0);
	/* Vemos velocidad del temporizador*/
	byte factorT = _io_ports[M6812_TMSK2] & 0x07; /*Factor de escalado actual*/
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT); /* Frecuencia del temporizador*/
	/* Según la frecuencia elegimos el modo de dividir para evitar desbordamientos */
	if(frec/1000000)
	numCiclosL = frec/1000000 * useg;
	else
	numCiclosL = frec/100 * useg/10000;

	//NDisparosOC0 = variable global
	nDisparosOC0 = numCiclosL >> 16;  /* Numero de disparos necesarios */
	nDisparosOC0_inicial = nDisparosOC0;
	numCiclos = numCiclosL & 0xffff; /* Número restante de ciclos */

	/* Por si escalado muy grande y useg pequeño */
	if((numCiclos == 0) && (nDisparosOC0 == 0)) numCiclos = 1;

	_io_ports[M6812_TMSK1] |= M6812B_C0I;
	_io_ports[M6812_TIOS] |= M6812B_IOS0; /*configuramos canal como comparador salida*/
	_io_ports[M6812_TFLG1] = M6812B_C0F; /*Bajamos el banderín  */
	/*preparamos disparo*/
	_IO_PORTS_W(M6812_TC0) = _IO_PORTS_W(M6812_TCNT) + numCiclos ;

	//Cuando se dispare

}

void funcionEjemplo(void){
	serial_print("a\n");
}

void funcionEjemplo2(void){
	serial_print("b\n");
}

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

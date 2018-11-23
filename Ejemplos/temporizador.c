/**********************************************

  Librería para realizar un temporizador

***********************************************/



#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

//Para pensar mejor
typedef unsigned char byte;
typedef unsigned int 2bytes;
typedef unsigned long 4bytes;

//Nº máximo desbordamientos 2⁸
4bytes nDesbordamientos = 0UL;

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
4bytes get_microseconds(void){
    //Obtenemos el factor de frecuencia
	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
    //Frecuencia es frecuencia total(8MHz)/2^factor
	4bytes frec = M6812_CPU_E_CLOCK/(1<<factorT);		
    //Numero de ciclos total es el valor actual del contador
    //concatenado por la izquierda con el nº desbordamientos
	4bytes  numCiclos = (nDesbordamientos << 16) | _IO_PORTS_W(M6812_TCTL);
    //Microsegundos depende del factor
    int desp = 3 - factorT;
    4bytes microSeconds;
    if (desp > 0){
       return numCiclos >> desp;
    }
    else{
        desp *= -1;
        return numCiclos << desp;
    }
}

//Devuelve los milisegundos que han pasado
4bytes get_miliseconds(void){
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

//Instalar funcion que se ejecute tras tiempo especificado


//Instalar funcion que se realice periódicamente


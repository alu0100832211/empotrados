/**********************************************

Librería para realizar un temporizador

***********************************************/
bytes4 nDisparosOC0_inicial;
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
	if(nDisparosOC0 == 0){
		(*runEveryUsg_f)();
		nDisparosOC0 = nDisparosOC0_inicial;
	}
	else{
		nDisparosOC0--;
	}
	_io_ports[M6812_TFLG1] = M6812B_C0F;
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

bytes4 get_miliseconds(void){
	return get_microseconds() * 1000;
}

void delayusg(unsigned long useg) {
	unsigned int numCiclos;
	unsigned long numCiclosL;

	_io_ports[M6812_TCTL1] &= ~(M6812B_OM6 | M6812B_OL6);

	byte factorT = _io_ports[M6812_TMSK2] & 0x07;
	unsigned long frec = M6812_CPU_E_CLOCK/(1 << factorT);
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

void print4bWord(bytes4 word){
	bytes2 m1 = word >> 16;
	bytes2 m2 = word;

	serial_print("0x");
	serial_printhexword(m1);
	serial_printhexword(m2);
	serial_print("\n");
}


void runAfterUsg(void (*f)(void), bytes4 useg){
	runAfterUsg_f = f;

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

	return 0;
}

void runEveryUsg(void (*f)(void), bytes4 useg){
	runEveryUsg_f = f;

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

	return 0;
}

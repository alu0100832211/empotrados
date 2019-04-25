/**
 * @file e_s_lib.c
 * @author Sergio
 * @author Santiago
 * @date 14/12/18
 * @brief Librería entrada/salida (c)
 *
 * Librería E/S P1 Sistemas Empotrados
 */
#include <e_s_lib.h>


int configurar_puerto(const char port_char, const int conf, int pin){


	unsigned char port;
	serial_init();
	switch(port_char)
	{
		case 'H': port = M6812_DDRH; break;
		case 'G': port = M6812_DDRG; break;
		case 'A': port = M6812_DDRA; break;
		case 'B': port = M6812_DDRB; break;
		default:
    //serial_print("\nPuerto no válido.\n"); break;
    return 0;

	}

	//serial_print("\nPines a asignar: ");
	//serial_printdecbyte(pines);
	//serial_print("\n");

	if (pin >= 0 || pin <= 7)
	{
	  if (conf) // Salida
	  {
		  _io_ports[port] |= 00000001 << pin;
		  return 1;


	  }else{	// Entrada

		  _io_ports[port] &= ~(00000001) << pin;
		  return 1;
	  }
	}

	return 0;

}

int pull_up(const char port_char, const int accion){

	int temp = 1;

	/*
		Si accion = 0 apagar pullup en el puerto port
		Si accion = 1 encender pullup en el puerto port
	*/

	switch(port_char){
		case 'A': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPA;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPA;
		} break;
		case 'B': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPB;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPB;
		 } break;
		case 'E': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPE;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPE;
		 } break;
		case 'H': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPH;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPH;
		 } break;
		case 'G': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPG;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPG;
		 } break;
		default: temp = 0; break;
	}

	return temp;

}

int pull_up_all(const int accion){

	serial_init();

	/*
		Si accion = 1 encendemos pullup en todos los puertos (A,B,E,G,H)
		Si accion = 0 apagamos pullup en todos los puertos (A,B,E,G,H)
	*/

	if (accion){

		_io_ports[M6812_PUCR] = 0x00;
		return 1;
	}else{

		_io_ports[M6812_PUCR] = 0xff;
		return 1;
	}

	return 0;
}


unsigned char leer_puerto(const char port_char){

	unsigned char port;
	switch(port_char)
	{
		case 'H': port = M6812_PORTH; break;
		case 'G': port = M6812_PORTG; break;
		case 'A': port = M6812_PORTA; break;
		case 'B': port = M6812_PORTB; break;

	}

/*
	serial_init();
	serial_print("Leyendo el puerto ");
	serial_printbinunsigned char(port_char);
	serial_print("\nBin -> ");
	serial_printbinunsigned char(_io_ports[port]);
	serial_print("\n");
	serial_print("Hex -> ");
	serial_printhexunsigned char(_io_ports[port]);
	serial_print("\n");
	serial_print("Dec -> ");
	serial_printdecbyte(_io_ports[port]);
	serial_print("\n");
*/

	//return port;
  return _io_ports[port];

}


unsigned char leer_puerto_bit(const char port_char, int bit){

//	serial_init();
	unsigned char port;
	switch(port_char)
	{
		case 'H': {

			if (bit >= 0 || bit <= 7)
			{

				switch(bit)
				{
					case 0:	return M6812B_PH0; break;
					case 1:	return M6812B_PH1; break;
					case 2:	return M6812B_PH2; break;
					case 3:	return M6812B_PH3; break;
					case 4:	return M6812B_PH4; break;
					case 5:	return M6812B_PH5; break;
					case 6:	return M6812B_PH6; break;
					case 7:	return M6812B_PH7; break;


				}

			}

		}; break;

		case 'G': {


			if (bit >= 0 || bit <= 7)
			{

				switch(bit)
				{
					case 0:	return M6812B_PG0; break;
					case 1:	return M6812B_PG1; break;
					case 2:	return M6812B_PG2; break;
					case 3:	return M6812B_PG3; break;
					case 4:	return M6812B_PG4; break;
					case 5:	return M6812B_PG5; break;
					case 6:	return M6812B_PG6; break;
					case 7:	return M6812B_PG7; break;


				}

			}


		}; break;


		case 'A': {

			if (bit >= 0 || bit <= 7)
			{

				switch(bit)
				{
					case 0:	return M6812B_PA0; break;
					case 1:	return M6812B_PA1; break;
					case 2:	return M6812B_PA2; break;
					case 3:	return M6812B_PA3; break;
					case 4:	return M6812B_PA4; break;
					case 5:	return M6812B_PA5; break;
					case 6:	return M6812B_PA6; break;
					case 7:	return M6812B_PA7; break;


				}

			}

		}; break;

		case 'B': {


			if (bit >= 0 || bit <= 7)
			{

				switch(bit)
				{
					case 0:	return M6812B_PB0; break;
					case 1:	return M6812B_PB1; break;
					case 2:	return M6812B_PB2; break;
					case 3:	return M6812B_PB3; break;
					case 4:	return M6812B_PB4; break;
					case 5:	return M6812B_PB5; break;
					case 6:	return M6812B_PB6; break;
					case 7:	return M6812B_PB7; break;


				}

			}


		}; break;

		default:
      //serial_print("\nPuerto no válido.\n"); break;
      return 0;

	}


}


void escribir_puerto(const char port_char, unsigned char value){

	unsigned char port;
	switch(port_char)
	{
		case 'H': port = M6812_PORTH; break;
		case 'G': port = M6812_PORTG; break;
		case 'A': port = M6812_PORTA; break;
		case 'B': port = M6812_PORTB; break;
    default: return;

	}

/*
	serial_init();
	serial_print("Escribiendo en el puerto: ");
	serial_printbinunsigned char(port_char);
	serial_print(" el valor = ");
	serial_printbinunsigned char(value);
	serial_print("\n");
*/
	_io_ports[port] = value;


}

void __attribute__((interrupt))vi_kwgh(void) {
  int irqG = 0;  //pin interrupc puerto G
  int irqH = 0;  //pin interrupc puerto H
  char g = _io_ports[M6812_KWIFG];
  char h = _io_ports[M6812_KWIFH];

  while(_io_ports[M6812_KWIEG] & M6812_KWIFG) { //activada interrup puerto G
	if (g){ //si bit activado...
  	g = 1; //desactiva flag (igualando a '1')
  	serial_print("interrupcion pin KWIFG.\n"); //gestion interrup.
	serial_printdecbyte(irqG);
	serial_print("\n");
	}
	g >> 1; //desplaza bits una posic. a la izqda
	irqG = irqG++ % 8;
  }

  while(_io_ports[M6812_KWIEH] & M6812_KWIFH) { //activada interrup puerto H
	if (h){ //si bit activado...
  	h = 1; //desactiva flag (igualando a '1')
  	serial_print("interrupcion pin KWIFH.\n"); //gestion interrup.
	serial_printdecbyte(irqH);
	serial_print("\n");
	}
	h >> 1; //desplaza bits una posic. a la izqda
	irqH = irqH++ % 8;
  }
}

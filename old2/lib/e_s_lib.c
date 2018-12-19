/**
 * @file doxygen_c.h
 * @author Sergio y Santiago
 * @date 14/12/18
 * @brief Includes necesarios
 *
 * Librería E/S P1 Sistemas Empotrados
 */

#include <e_s_lib.h>

/**
 * @brief Método de configuración de puerto como entrada/salida y sus respectivos pines
 *
 * Llamada para configurar los pines 0 1 2 3 4 5 del puerto H como salida
 * configurar('H', 1, "0 1 2 3 4 5")
 *
 * Llamada para configurar los pines 0 1 del puerto G como Entrada
 * configurar('G', 0, "0 1")
 */
int configurar_puerto(const char port_char, const int conf, char* pines){


	unsigned char port;
	serial_init();
	switch(port_char)
	{
		case 'H': port = M6812_DDRH; break;
		case 'G': port = M6812_DDRG; break;
		default: serial_print("\nPuerto no válido.\n");

	}

	//serial_print("\nPines a asignar: ");
	//serial_printdecbyte(pines);
	//serial_print("\n");

	int i=0;
	int dig =0;
	char pin=0;

	if (conf) // Salida
	{

		while(pin != '\0')
		{
			pin = pines[i];
			if (pin == '0' || pin == '1' || pin == '2' || pin == '3' || pin == '4' || pin == '5' || pin == '6' || pin == '7')
			{
				dig = pin + '0';
				_io_ports[port] |= 00000001 << dig;
			}

			++i;

		}
		return 1;


	}else{	// Entrada

		while(pin != '\0')
		{
			pin = pines[i];
			if (pin == '0' || pin == '1' || pin == '2' || pin == '3' || pin == '4' || pin == '5' || pin == '6' || pin == '7')
			{
				dig = pin + '0';
				_io_ports[port] &= ~(00000001) << dig;
			}
			++i;
		}
		return 1;
	}

	return 0;

}

/**
 * @brief Método para encender/apagar los PULLUP en los diferentes puertos (A,B,E,H,G)
 *
 * accion = true -> Encender pullup
 * accion = false -> Apagar pullup
 * Encender pullup en puerto A
 * pull_up('A', 1);
 *
 * Apagar pullup en puerto B
 * pull_up('B', 0);
 */
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
		}
		case 'B': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPB;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPB;
		 }
		case 'E': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPE;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPE;
		 }
		case 'H': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPH;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPH;
		 }
		case 'G': {
			if (accion)
				_io_ports[M6812_PUCR] |= M6812B_PUPG;
			else
				_io_ports[M6812_PUCR] &= ~M6812B_PUPG;
		 }
		default: temp = 0; break;
	}

	return temp;

}

/**
 * @brief Método para encender/apagar los PULLUP en todos los puertos
 *
 * accion = true -> Encender pullup
 * accion = false -> Apagar pullup
 * Encender pullup
 * pull_up(1);
 *
 * Apagar pullup
 * pull_up(0);
 */
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


/**
 * @brief Método para leer un puerto
 *
 * Leer del puerto H
 * unsigned char value;
 * value = leer_puerto('H');
 */
unsigned char leer_puerto(const char port_char){

	unsigned char port;
	switch(port_char)
	{
		case 'H': port = M6812_PORTH; break;
		case 'G': port = M6812_PORTG; break;

	}

	serial_init();
	serial_print("Leyendo el puerto ");
	serial_printbinbyte(port_char);
	serial_print("\nBin -> ");
	serial_printbinbyte(_io_ports[port]);
	serial_print("\n");
	serial_print("Hex -> ");
	serial_printhexbyte(_io_ports[port]);
	serial_print("\n");
	serial_print("Dec -> ");
	serial_printdecbyte(_io_ports[port]);
	serial_print("\n");

	return port;

}

/**
 * @brief Método para leer un puerto
 *
 * Escribir en puerto H el valor 0xff
 * unsigned char value = 0xff;
 * escribir_puerto('H', value);
 */
void escribir_puerto(const char port_char, unsigned char value){

	unsigned char port;
	switch(port_char)
	{
		case 'H': port = M6812_PORTH; break;
		case 'G': port = M6812_PORTG; break;

	}

	serial_init();
	serial_print("Escribiendo en el puerto: ");
	serial_printbinbyte(port_char);
	serial_print(" el valor = ");
	serial_printbinbyte(value);
	serial_print("\n");

	_io_ports[port] = value;


}


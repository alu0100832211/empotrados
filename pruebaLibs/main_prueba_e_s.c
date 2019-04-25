
#include <e_s_lib.h>

// MAIN PROGRAM

/**
 * @brief MAIN
 *
 * Programa de ejemplo
 */
int main (void)
{
	serial_init();
	serial_print("\nPrueba E/S LEDS a Puertos G y H... \n");


	int opt = 0;

	while(opt != 8)
	{

		serial_print("Seleccione una opción: \n");
		serial_print("1. Configurar un puerto como entrada. \n");
		serial_print("2. Configurar un puerto como salida. \n");
		serial_print("3. Leer un puerto. \n");
		serial_print("4. Escribir un puerto \n");
		serial_print("5. Encender pullup en puerto específico. \n");
		serial_print("6. Apagar pullup en puerto específico. \n");
		serial_print("7. Encender/Apagar pullup en todos los puertos (A,B,E,G,H) \n");
		serial_print("8. Salir.\n");

		opt = serial_getdecbyte();


		switch(opt)
		{

			case 1: {

				int sel = 0;
				bool temp = 1;
				int pin;

				while (sel != 3)
				{

					serial_print("Seleccione un puerto a configurar: \n");
					serial_print("1.Puerto H como entrada.\n");
					serial_print("2. Puerto G como entrada.\n");
					serial_print("3. Cancelar.\n");
					sel = serial_getdecbyte();

					int contador = 0;


					switch (sel)
					{
						case 1: {
						    while (contador <= 7)
						    {
							serial_print("\nIntroduzca el pin a configurar (0-7).\n");
							serial_print("\nPulse 9 para salir.\n");
							pin= serial_getdecbyte();

							if (pin == 9)
							  contador = 9;
							else
							{
							  temp = configurar_puerto('H', 0, pin);
							  ++contador;
							}

						    }

						} break;

						case 2: {
						    while (contador <= 7)
						    {
							serial_print("\nIntroduzca el pin a configurar (0-7).\n");
							serial_print("\nPulse 9 para salir.\n");
							pin= serial_getdecbyte();

							if (pin == 9)
							  contador = 9;
							else
							{
							  temp = configurar_puerto('G', 0, pin);
							  ++contador;
							}

						    }

						} break;

						case 3: sel = 3; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

					if(!temp)
					{
						serial_print("Ha ocurrido algún error configurando el puerto.\n");
						sel = 0;
					}

				}

				opt = 0;

			} break;

			case 2: {

				int sel = 0;
				bool temp = 1;
				int pin;

				while (sel != 3)
				{

					serial_print("Seleccione un puerto a configurar: \n");
					serial_print("1.Puerto H como salida.\n");
					serial_print("2. Puerto G como salida.\n");
					serial_print("3. Cancelar.\n");
					sel = serial_getdecbyte();

					int contador = 0;


					switch (sel)
					{
						case 1: {
						    while (contador <= 7)
						    {
							serial_print("\nIntroduzca el pin a configurar (0-7).\n");
							serial_print("\nPulse 9 para salir.\n");
							pin= serial_getdecbyte();

							if (pin == 9)
							  contador = 9;
							else
							{
							  temp = configurar_puerto('H', 1, pin);
							  ++contador;
							}

						    }

						} break;

						case 2: {
						    while (contador <= 7)
						    {
							serial_print("\nIntroduzca el pin a configurar (0-7).\n");
							serial_print("\nPulse 9 para salir.\n");
							pin= serial_getdecbyte();

							if (pin == 9)
							  contador = 9;
							else
							{
							  temp = configurar_puerto('G', 1, pin);
							  ++contador;
							}

						    }

						} break;

						case 3: sel = 3; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

					if(!temp)
					{
						serial_print("Ha ocurrido algún error configurando el puerto.\n");
						sel = 0;
					}

				}

				opt = 0;


			} break;

			case 3: {

				byte value;
				int sel = 0;

				while (sel != 3)
				{

					serial_print("Seleccione un puerto a leer: \n");
					serial_print("1.Puerto H.\n");
					serial_print("2. Puerto G.\n");
					serial_print("3. Cancelar.\n");

					sel = serial_getdecbyte();

					switch (sel)
					{
						case 1: value = leer_puerto('H'); break;

						case 2: value = leer_puerto('G'); break;

						case 3: sel = 3; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

				}

				opt = 0;


			} break;

			case 4: {

				byte value;
				int sel = 0;

				while (sel != 3)
				{

					serial_print("Seleccione un puerto a escribir: \n");
					serial_print("1.Puerto H.\n");
					serial_print("2. Puerto G.\n");
					serial_print("3. Cancelar.\n");
					sel = serial_getdecbyte();

					if (sel == 1 || sel == 2)
					{
						serial_print("Introduzca el valor a escribir.\n");
						value = serial_getbinbyte();
					}

					switch (sel)
					{

						case 1: escribir_puerto('H', value); break;

						case 2: escribir_puerto('G', value); break;

						case 3: sel = 3; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

				}
				opt = 0;

			} break;

			case 5: {


				int sel = 0;
				bool temp = 1;

				while (sel != 6)
				{

					serial_print("Seleccione un puerto: \n");
					serial_print("1. Encender pullup Puerto A.\n");
					serial_print("2. Encender pullup Puerto B.\n");
					serial_print("3. Encender pullup Puerto E.\n");
					serial_print("4. Encender pullup Puerto G.\n");
					serial_print("5. Encender pullup Puerto H.\n");
					serial_print("6. Cancelar.\n");


					sel = serial_getdecbyte();

					switch (sel)
					{

						case 1: temp = pull_up('A', 1); break;

						case 2: temp = pull_up('B', 1); break;

						case 3: temp = pull_up('E', 1); break;

						case 4: temp = pull_up('H', 1); break;

						case 5: temp = pull_up('G', 1); break;

						case 6: sel = 6; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

					if(!temp)
					{
						serial_print("Ha ocurrido algún error configurando el puerto.\n");
						sel = 0;

					}

				}

				opt = 0;

			} break;

			case 6: {


				int sel = 0;
				bool temp = 1;

				while (sel != 6)
				{

					serial_print("Seleccione un puerto: \n");
					serial_print("1. Apagar pullup Puerto A.\n");
					serial_print("2. Apagar pullup Puerto B.\n");
					serial_print("3. Apagar pullup Puerto E.\n");
					serial_print("4. Apagar pullup Puerto G.\n");
					serial_print("5. Apagar pullup Puerto H.\n");
					serial_print("6. Cancelar.\n");

					sel = serial_getdecbyte();

					switch (sel)
					{

						case 1: temp = pull_up('A', 0); break;

						case 2: temp = pull_up('B', 0); break;

						case 3: temp = pull_up('E', 0); break;

						case 4: temp = pull_up('H', 0); break;

						case 5: temp = pull_up('G', 0); break;

						case 6: sel = 6; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

					if(!temp)
					{
						serial_print("Ha ocurrido algún error configurando el puerto.\n");
						sel = 0;

					}

				}

				opt = 0;

			} break;

			case 7: {

				int sel = 0;
				bool temp = 1;

				while (sel != 3)
				{

					serial_print("Seleccione acción: \n");
					serial_print("1. Encender pullup.\n");
					serial_print("2. Apagar pullup.\n");
					serial_print("3. Cancelar.\n");

					sel = serial_getdecbyte();

					switch (sel)
					{

						case 1: temp = pull_up_all(1); break;

						case 2: temp = pull_up_all(0); break;

						case 3: sel = 3; break;

						default: serial_print("Seleccione una opción válida.\n"); break;
					}

					if(!temp)
					{
						serial_print("Ha ocurrido algún error configurando el puerto.\n");
						sel = 0;

					}

				}
				opt = 0;

			} break;

			case 8: serial_print("Saliendo...\n"); break;

			default: serial_print("Introduzca una opción válida.\n"); break;

		}


	}

}


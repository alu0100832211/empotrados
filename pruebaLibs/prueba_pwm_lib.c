
#include <sys/ports.h>
#include <sys/param.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <sys/interrupts.h>

#include <pwm_lib.h>


int main()
{
  serial_init();
  serial_print("\nInicializándose... ");
  serial_recv();
  pwd_generalizado(0, 3000000, 1);

  serial_print("\nCanal utilizado = ");
  serial_printdecword(actual_channel);

  //_io_ports[M6812_PWDTY0] = 0x10;
  //_io_ports[M6812_PWPER0] = 0x20;

  while(1)
  {
	serial_print("\n##############MENU###############");
	serial_print("\n#	1) Cambiar periodo	#");
	serial_print("\n#	2) Cambiar duty		#");
	serial_print("\n#	3) Cambiar canal	#");
	serial_print("\n#################################");
        option = serial_recv();
        serial_send(option);
	switch(option)
	{
		case ('1'):
			serial_print("\nPonga el periodo a cambiar: ");
                        nb = serial_get_byte();
			serial_printdecbyte(nb);
			set_PWPER(nb);
			break;

		case ('2'):
			serial_print("\nPonga el duty a cambiar: ");
			nb = serial_get_byte();
                        serial_printdecbyte(nb);
			set_PWDTY(nb);
			break;

		case ('3'):
			serial_print("\nDeshabilitando canal anterior....");
			reset_channel();
			serial_print("\nPonga que canal va a utilizar: ");
                        actual_channel = serial_recv();
			serial_printdecword(actual_channel);
			serial_print("\nPonga el valor de la frecuencia que va a utilizar (0<Hz<2000000): ");
                        frec_v = serial_recv();
                        serial_printdecword(frec_v);
			serial_print("\nPonga que polarizacion va a utilizar: ");
			serial_printdecword(pol_v);
			pwd_generalizado(actual_channel,frec_v,pol_v);
			break;
		default:
			serial_print("\nOpcion incorrecta");
			break;
	}
  }

}

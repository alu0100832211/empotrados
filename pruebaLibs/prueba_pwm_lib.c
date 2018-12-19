
#include <sys/ports.h>
#include <sys/param.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <sys/interrupts.h>

#include <pwm_lib.h>

char option;
int pol_v = 0;
int nb = 0;
unsigned long int frec_v= 0;
int channel = 0;


int main()
{
  serial_init();
  serial_print("\nInicializándose... ");
  serial_recv();
  pwd_generalizado(channel, 50000UL, 1);

  serial_print("\nCanal utilizado = ");
  serial_printdecbyte(channel);

  while(1)
  {
    serial_print("\n##############MENU###############");
    serial_print("\n#    1) Cambiar periodo    #");
    serial_print("\n#    2) Cambiar duty        #");
    serial_print("\n#    3) Cambiar canal    #");
    serial_print("\n#################################");
    option = serial_recv();
    serial_send(option);
    switch(option)
    {
      case ('1'):
        serial_print("\nPonga el periodo a cambiar: ");
        nb = serial_getdecbyte();
        serial_printdecbyte(nb);
        set_PWPER(nb);
        break;

      case ('2'):
        serial_print("\nPonga el duty a cambiar: ");
        nb = serial_getdecbyte();
        serial_printdecbyte(nb);
        set_PWDTY(nb);
        break;

      case ('3'):
        serial_print("\nDeshabilitando canal anterior....");
        reset_channel();
        serial_print("\nPonga que canal va a utilizar: ");
       channel = serial_recv();
        serial_printdecbyte(channel);
        //serial_print("\nPonga el valor de la frecuencia que va a utilizar (0<Hz<2000000): ");
  //                  frec_v = serial_recv();
  //                  serial_printdecbyte(frec_v);
        //serial_print("\nPonga que polarizacion va a utilizar: ");
        //serial_printdecbyte(pol_v);
        pwd_generalizado(channel,40000,pol_v);
        break;
      default:
        serial_print("\nOpcion incorrecta");
        break;
    }
  }

}

#include <pwm_lib.h>

int main(void){
<<<<<<< HEAD
=======
  pwd_generalizado(0, 0, 1);
>>>>>>> funciona
  while(1){
    serial_init();
    serial_print("Introducir valor (0-5000):\n");

    unsigned long frec = serial_recv();
    if (frec > 100) frec = 100;
    else if (frec < 0) frec = 0;
//void pwd_generalizado(unsigned int channel, unsigned long frec, unsigned int pol);
    //pin 40 PP0/PW0
<<<<<<< HEAD
    pwd_generalizado(0, frec, 1);
=======
    set_PWPER(frec);
>>>>>>> funciona
  }
}

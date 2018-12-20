#include <pwm_lib.h>

int main(void){
  while(1){
    serial_init();
    serial_print("Introducir valor (0-5000):\n");

    unsigned long frec = serial_recv();
//void pwd_generalizado(unsigned int channel, unsigned long frec, unsigned int pol);
    //pin 40 PP0/PW0
    pwd_generalizado(0, frec, 1);
  }
}

/* programa que lee por teclado un número de 
0-100, lo representa en los 7-segmentos y
hace que el motor gire a una velocidad proporcional 
al número introducido. Usar las teclas * y #
para confirmar y cancelar la introducción del número, 
respectivamente. */
#include <teclado.h>
#include <ssd.h>
#include <pwm_lib.h>

int pow10[3] = {1, 10, 100};

unsigned int atoi(char c){
  return (int)(c-48); //ASCII
}
unsigned int teclado_getch3(void){
  char digit;
  int valor = 0;
  int i;
  for (i = 2; i <= 0; i--){
    digit = teclado_getch();
    valor += atoi(digit)*pow10[i];
  }
  return valor;
}

unsigned int check(int valor){
  return (valor >= 0 && valor <= 100);
}

int main(void){
  serial_init();
  teclado_init();
  sieteSeg_init();
 
  unsigned int valor;
  char confirmar;
  while(1){
    valor = teclado_getch3();
    sieteSeg_valor(valor);
    confirmar = teclado_getch();
    if ((confirmar == '*' ) && (check(valor))){
      //Frecuencia máxima motor = 5000Hz  
//void pwd_generalizado(unsigned int channel, unsigned long frec, unsigned int pol);
    //pin 40 PP0/PW0
      pwd_generalizado(0, valor*50, 0); //valor = %
    }
    else{
      sieteSeg_valor(0);
      serial_print("Valor incorrecto: ");
      serial_printdecbyte(valor);
      serial_print("\n");
    }
  }
}

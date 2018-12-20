/* programa que lee por teclado un número de 
0-100, lo representa en los 7-segmentos y
hace que el motor gire a una velocidad proporcional 
al número introducido. Usar las teclas * y #
para confirmar y cancelar la introducción del número, 
respectivamente. */
#include <teclado.h>
#include <ssd.h>

void pwm_init(void){
}
unsigned int teclado_getch3(void){
}
unsigned int check(int valor){
}
int main(void){
  serial_init();
  teclado_init();
  sieteSeg_init();
  pwm_init();
 
  unsigned int valor;
  char confirmar;
  while(1){
    valor = teclado_getch3();
    sieteSeg_valor(valor)
    confirmar = teclado_getch();
    if (confirmar == '*' & check(valor)){
      //Hacer girar motor a ese valor
    }
    else{
      sieteSeg_valor(0);
      serial_print("Valor incorrecto: ");
      serial_printdecbyte(valor);
      serial_print("\n");
    }
  }
}

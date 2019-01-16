#include <teclado.h>

int main(void){
  serial_init();
  teclado_init();
  char letra;
  while(1){
  /* Bits de columnas estan a 1 ??*/
    letra = teclado_getch();
    serial_print("teclado_getch()\nLetra recibida: ");
    serial_print(&letra);
    serial_print("teclado_getch_timeout(2000ms)\nLetra recibida: ");
    letra = teclado_getch_timeout(2000);
    serial_print(&letra);
  }
}

/* Ejemplo que ilustra el paso de argumentos a la funcion 
 * runEveryUsg y runAfterUsg. En este caso se pasa un
 * struct, pero puede ser cualquier otro tipo.
 * El argumento tiene que ser un puntero.
 */
#include <stdio.h>
#include <stdlib.h>


//Función definida en la librería
void runEveryUsg(void (*f)(void*), void * args, unsigned long microseconds){
  /* ... */
  f(args);
}

// Función que define el programador
void foo(void * ptr){
  unsigned char* byte;
  byte = (unsigned char*)ptr;
  printf("Recibido byte %c\n", *byte);
}

int main(void){
  unsigned char* byte;
  byte = (unsigned char*)malloc(sizeof(unsigned char));
  *byte = 0b01100001;

  runEveryUsg(foo, byte, 10000UL);

  free(byte);
}

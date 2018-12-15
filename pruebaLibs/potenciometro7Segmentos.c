#include <temporizador.h>
#include <e_s_lib.h>
#include <atd.h>
#include <cmath>
/********************** POSIBLES PROBLEMAS *************************************
 * Probar llamar runEveryUseg runAfterUseg varias veces
 * Probar llamar runEveryUseg runAfterUseg con argumentos
 * Probar que la funcion pasada a runEverUseg use una variable global
 ******************************************************************************/

/********************** VARIABLES GLOBALES ************************************/
int n7S = 0; //número 7 segmento (7-segmento que se enciende)
/******************************************************************************/

void sieteSeg_init(){
  configurar_puerto('G', 1, "0 1 2 3 4 5 6 7");
  init_temporizador(7);
}
/**
 * @brief Escribe digitos en siete-segmentos
 * recibirá puntero a array de al menos
 * 4 bytes. A partir de ese momento se mostrará en cada 7-segmentos valor correspondiente a
 * las primeras 4 posiciones del array pasado.
 */
void refrescar7Seg(void * array){
  array = (unsigned char*) array;

  _io_ports[M6812_PORTG] = array[n7S];
  (n7S == 3) ? n7S = 0 : n7S++;
}

void sieteSeg_digitos(unsigned char* array){
  runEveryUsg(refrescar7Seg, array, 1000UL);
}

/**
 * @brief Recibe entero y lo muestra en los 7 segmentos
 * Como sólo hay 4 
 */
void sieteSeg_valor(unsigned int numero){
  unsigned char * digitos = new unsigned char [4]; // 4 7-segmentos
  for (int i = 0; i < 4; i++){
    digitos[i] = numero % 10;
    numero /= 10;
  }
  
  sieteSeg_digitos(digitos);
}


int main (void){
  /* Parte 2
   * Hacer un programa que lea el valor de uno de 
   * los potenciómetros y represente en los 7-segmentos 
   * el valor de la conversión con una resolución de 10 bits.
   */

  // Usar libreria conversor analogico digital

}

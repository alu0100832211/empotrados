#include <temporizador.h>
#include <e_s_lib.h>
#include <atd.h>
#include <cmath>
/********************** POSIBLES PROBLEMAS *************************************
 * No probamos llamar runEveryUseg runAfterUseg varias veces
 * No probamos llamar runEveryUseg runAfterUseg con argumentos
 ******************************************************************************/

/********************** VARIABLES GLOBALES ************************************/
int iR7S = 0; //i Refrescar 7 Segmentos
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
void refrescar7Seg(unsigned char* array){
  _io_ports[M6812_PORTG] = array[iR7S];
  (iR7S == 4) ? iR7S = 0 : iR7S++;
}

void sieteSeg_digitos(unsigned char* array){
  runEveryUsg(refrescar7Seg(array), 1000UL);
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

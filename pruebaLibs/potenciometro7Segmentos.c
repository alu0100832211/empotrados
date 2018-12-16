#include <temporizador.h>
#include <e_s_lib.h>
#include <atd.h>
/***************** POSIBLES PROBLEMAS ***************************
 * Probar llamar runEveryUseg runAfterUseg varias veces
 * Probar llamar runEveryUseg runAfterUseg con argumentos
 * Probar que la funcion pasada a runEverUseg use una variable global
 * Atd devuelve el tanto por 1 del voltaje máximo representado en 10 bits,
 * ¿qué significa esto?
 * ¿Los puertos atd usables son 0 y 1?
 * ¿Qué puertos están configurados para qué cosa? Por ejemplo:
 * el temporizador usa puerto T. (E/S dice que todos los puertos
 * se pueden configurar como E/S si no están configurados para 
 * otra cosa.)
 * Nuestra librería no devuelve valores en hexadecimal porque un 
 * valor en sí no es hexadecimal. Lo que pasa es que al llamarlo
 * con serial_hexprint se representaba en hexadecimal.
 ***************************************************************/
#define FACTOR_T 7
#define PUERTO_ATD 1

/***************** VARIABLES GLOBALES **************************/
int n7S = 0; //número 7 segmento (7-segmento que se enciende)
unsigned char digits_refreshed[4];
/***************************************************************/

void sieteSeg_init(){
  init_temporizador(FACTOR_T);
  configurar_puerto('G', 1, "0 1 2 3 4 5 6 7");
  atd_default_config(PUERTO_ATD);
  atd_activate_module(PUERTO_ATD);
}
/**
 * @brief Escribe digitos en siete-segmentos
 * recibirá puntero a array de al menos
 * 4 bytes. A partir de ese momento se mostrará en cada 7-segmentos valor correspondiente a
 * las primeras 4 posiciones del array pasado.
 */
void refrescar7Seg(void * ptr){
  unsigned char * digito;
  digito = (unsigned char*)ptr;
  _io_ports[M6812_PORTG] = digito[n7S];
  (n7S == 3) ? n7S = 0 : n7S++;
}

void sieteSeg_digitos(unsigned char* digito){
  /** 1000 = [1][0][0][0] 
   *          0  1  2  3  **/
  unsigned char digitMask;
  int digitPos;
  for (digitPos = 0; digitPos < 4; digitPos++){
    digitMask = 1 << (4 + digitPos);      //00010000 << digitPos
    digitMask |= 0x0f;                    //00010000 |= 00011111
    digito[digitPos] |= 0xf0;             //xxxxyyyy |= 11110000 = 1111yyyy
    digito[digitPos] &= digitMask;        //1111yyyy &= 00011111 = 0001yyyy
  }
  runEveryUsg(refrescar7Seg, (void*) digito, 1000UL);
}

/**
 * @brief Recibe entero y lo muestra en los 7 segmentos
 */
void sieteSeg_valor(unsigned int numero){
  int i;
  for (i = 0; i < 4; i++){
    digits_refreshed[i] = numero % 10;
    numero /= 10;
  }

  sieteSeg_digitos(digits_refreshed);
}


int main (void){
  /* Parte 2
   * Hacer un programa que lea el valor de uno de 
   * los potenciómetros y represente en los 7-segmentos 
   * el valor de la conversión con una resolución de 10 bits.
   */
  /* Iniciar la conversión */
  atd_start_conversion(PUERTO_ATD);
  /* Esperar a que termine la conversión */
  atd_wait_for_conversor(PUERTO_ATD);
  /* Devolver los valores leidos */
  unsigned short data;
  unsigned short nConversiones;

  atd_get_data(&data, &nConversiones, PUERTO_ATD);
  sieteSeg_valor(data);
}

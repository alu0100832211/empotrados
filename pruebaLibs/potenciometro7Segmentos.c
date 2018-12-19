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
 * Si refrescar7Seg no va, probar sustituir variable static por
 * variable global
 ***************************************************************/
#define FACTOR_T 7
#define PUERTO_ATD 1

/***************** VARIABLES GLOBALES **************************/
unsigned char digits_refreshed[4];
/***************************************************************/

void sieteSeg_init(){
  init_temporizador(FACTOR_T);
  configurar_puerto('G', 1, 0);
  configurar_puerto('G', 1, 1);
  configurar_puerto('G', 1, 2);
  configurar_puerto('G', 1, 3);
  configurar_puerto('G', 1, 4);
  configurar_puerto('G', 1, 5);
  configurar_puerto('G', 1, 6);
  configurar_puerto('G', 1, 7);
  atd_default_config(PUERTO_ATD);
  atd_activate_module(PUERTO_ATD);
}
/**
 * @brief Escribe digitos en siete-segmentos
 * recibirá puntero a array de al menos
 * 4 bytes. A partir de ese momento se mostrará en cada 7-segmentos valor correspondiente a
 * las primeras 4 posiciones del array pasado.
 */
void refrescar7Seg(void){
  static int i = 0; //7-segmento que se enciende
  _io_ports[M6812_PORTG] = digits_refreshed[i];
  (i == 3) ? i = 0 : i++;
}

void sieteSeg_digitos(unsigned char* digito){
  /** 1000 = [1][0][0][0] 
   *          0  1  2  3  **/
  unsigned char activate7S;
  int digitPos;
  for (digitPos = 0; digitPos < 4; digitPos++){ //Configurar el 7-segmento que se enciende
    activate7S = 0x80 >> digitPos;      //10000000 >> digitPos
    digito[digitPos] |= activate7S;
    digito[digitPos] &= activate7S;
  }
  digits_refreshed[0] = digito[0]; // No se puede digits_refreshed = digito
  digits_refreshed[1] = digito[1]; // ni &digits_refreshed = digito
  digits_refreshed[2] = digito[2];
  digits_refreshed[3] = digito[3];
  runEveryUsg(refrescar7Seg, 1000UL);
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
  while(1){
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
}

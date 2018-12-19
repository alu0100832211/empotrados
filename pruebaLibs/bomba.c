#include <temporizador>
#include <e_s_lib.h>
#include <pwm_lib.h>

#define FACTOR_T 7
#define PUERTO_ATD 1

/***************** VARIABLES GLOBALES **************************/
unsigned char digits_refreshed[4];

char teclado[4][3] = {
// C1  C2  C3
  {'1','2','3' }, // F1
  {'4','5','6' }, // F2
  {'7','8','9' }, // F2
  {'*','0','#' }, // F2
};
/***************************************************************/
void teclado_init(){
  /* Conexionado
   * C2 F1 C1 F4 C3 F3 F2 Pines de Dixen
   * H1 G0 H0 G3 H2 G2 G1 Pin puerto ADAPT */

  /* Problema: no permite subconjunto pines en pull-up 
   * con lo cual hay que usar distintos puertos para
   * entrada y salida */

  /* G = FILAS = SALIDA */
  configurar_puerto('G', 1, 0);
  configurar_puerto('G', 1, 1);
  configurar_puerto('G', 1, 2);
  configurar_puerto('G', 1, 3);

  /* Todas las filas a 0 */
  escribir_puerto('G', 0);

  /* H = COLUMNAS = ENTRADA */
  configurar_puerto('H', 0, 0);
  configurar_puerto('H', 0, 1);
  configurar_puerto('H', 0, 2);

  /* COLUMNAS = resistencias pull-up */
  pull_up('H', 1);
}
char teclado_getch(){
  /* Salir si columnas no están a 1 */
  if ((_io_ports[M6812_PORTH] & COL_M ) != COL_M)
    return 'E';
  /* Esperar a que alguna columna llegue a 0 */
  while((_io_ports[M6812_PORTH] & COL_M ) == COL_M);
  /* Estabilización del valor 20 msg = 20000 useg*/
  delayusg(20000UL);
  /* Guardar la columna detectada */
  unsigned char bitPulsacion = !(_io_ports[M6812_PORTH] & COL_M); /*Dejar un 1 en la posicion de la columna*/
  int i = 0;
  unsigned char bitPulsacion_aux = bitPulsacion;
  while(bitPulsacion_aux > 1){
    bitPulsacion_aux = bitPulsacion_aux >> 1;
    i++;
  }
  int colPulsacion = i;
  /* Escribir 1 en todas las filas */
  _io_ports[M6812_PORTG] |= FIL_M; 
  
  int filaPulsacion = 0;
  /* Poner cada fila a 0 */
  for (i = 0; i < 4; i++){
    _io_ports[M6812_PORTG] &= !(1 << i);
    if (!(_io_ports[M6812_PORTH] & bitPulsacion)){ /* Si la columna pulsada baja a 0 */
      filaPulsacion = i;
      break;
    }
    _io_ports[M6812_PORTG] |= (1 << i);
  }
  /* Esperar a soltar la tecla */
  _io_ports[M6812_PORTG] &= !FIL_M; 
  while((_io_ports[M6812_PORTH] & COL_M ) != COL_M);
  delayusg(20000UL);

  return teclado[filaPulsacion][colPulsacion];
}

void sieteSeg_init(){
  configurar_puerto('I', 1, 0);
  configurar_puerto('I', 1, 1);
  configurar_puerto('I', 1, 2);
  configurar_puerto('I', 1, 3);
  configurar_puerto('I', 1, 4);
  configurar_puerto('I', 1, 5);
  configurar_puerto('I', 1, 6);
  configurar_puerto('I', 1, 7);
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

void all_init(){
  serial_init();
  init_temporizador(FACTOR_T);
  teclado_init();
  sieteSeg_init();
}

int atoi(const char charNum){
  return (int)(charNum-48); /*ASCII*/
}

int main (void){
  all_init();

  int digitoNumero[3];
  int i = 0;
  int num = 0;
  int pow10[3] = {1, 10, 100};

  while(1){
    serial_print("Esperando digitoNumero\n");
    num = 0;
    for(i = 0; i < 3; i++){
      digitoNumero[i] = teclado_getch();
      if(digitoNumero[i] == 'E'){
        serial_print("Error en teclado getch\n");
      }
      num += atoi(digitoNumero[i])*pow10[3-i];
    }
    if (num >= 0)
      if(num <= 0){
    // Hacer girar el motor
    }
   //Usar las teclas * y #
   //para confirmar y cancelar la introducción del número, respectivamente.
  }
}

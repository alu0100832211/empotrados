#include <temporizador.h>
/* 1 en la posicion del pin conectado */
#define COL_M 0b00000111
#define FIL_M 0b00001111
#define FACTOR_T 7
/******************** POSIBLES PROBLEMAS ********************
 * Libreria e/s no permite poner sólo un conjunto de pines como pull up
 * Solucion: usar puertos distintos
 * Se traga la definicion de teclado?
 ************************************************************/

/******************* VARIABLES GLOBALES *********************/
char teclado[4][3] = {
  {'1','2','3' },
  {'4','5','6' },
  {'7','8','9' },
  {'*','0','#' },
}
/************************************************************/
void teclado_init(){
  init_temporizador(FACTOR_T);
  /* Conexionado
   * C2 F1 C1 F4 C3 F3 F2 Pines de Dixen
   * H1 G0 H0 G3 H2 G2 G1 Pin puerto ADAPT */

  /* Problema: no permite subconjunto pines en pull-up 
   * con lo cual hay que usar distintos puertos para
   * entrada y salida */

  /* G = FILAS = SALIDA */
  configurar_puerto('G', 1, "0 1 2 3");

  /* Todas las filas a 0 */
  escribir_puerto('G', 0);

  /* H = COLUMNAS = ENTRADA */
  configurar_puerto('H', 0, "0 1 2");

  /* COLUMNAS = resistencias pull-up */
  pull_up('H', 1);
}

char teclado_getch(int mode){
  /* Salir si columnas no están a 1 */
  if (_io_ports[M6812_PORTH] & COL_M != COL_M)
    return -1;
  /* Esperar a que alguna columna llegue a 0 */
  while(_io_ports[M6812_PORTH] & COL_M == COL_M);
  /* Estabilización del valor 20 msg = 20000 useg*/
  delayusg(20000UL);
  /* Guardar la columna detectada */
  unsigned char colPulsacion = (unsigned char)log2(_io_ports[M6812_PORTH] & COL_M); // A ver si se lo traga
  /* Escribir 1 en todas las filas */
  _io_ports[M6812_PORTG] |= FIL_M; 
  
  unsigned char filaPulsacion;
  /* Poner cada fila a 0 */
  for (int i = 0; i < 4; i++){
    _io_ports[M6812_PORTG] &= !(1 << i);
    if (_io_ports[M6812_PORTH] & colPulsacion){
      filaPulsacion = i;
      break;
    }
    _io_ports[M6812_PORTG] |= (1 << i);
  }

  return teclado[filaPulsacion][colPulsacion];
}

char teclado_getch_timeout(unsigned int milis){
 unsigned long int tInicial = get_miliseconds(); 
 unsigned long int tActual = get_miliseconds();
 while((tActual-tInicial) < milis){
   // Comprobar si hay pulsacion
   tActual = get_miliseconds();
 }
}

int main(void){
  serial_init();
  teclado_init();
  /* Bits de columnas estan a 1 ??*/
  if (teclado_getch() == -1)
    exit(EXIT_FAILURE);
  else
    serial_print("bits de columnas están a 1 •ᴗ•\n");

}

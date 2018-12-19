#include <temporizador.h>
#include <e_s_lib.h>

/* Los bits a 1 son los que el puerto está usando */
#define COL_M 0x07 //  00000111
#define FIL_M 0x0f //  00001111
#define FACTOR_T 7
/******************** POSIBLES PROBLEMAS ********************
 * Libreria e/s no permite poner sólo un conjunto de pines como pull up
 * Solucion: usar puertos distintos
 * Se traga la definicion de teclado?
 ************************************************************/

/******************* VARIABLES GLOBALES *********************/
char teclado[4][3] = {
// C1  C2  C3
  {'1','2','3' }, // F1
  {'4','5','6' }, // F2
  {'7','8','9' }, // F2
  {'*','0','#' }, // F2
};
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

char teclado_getch(){
  /* Salir si columnas no están a 1 */
  if ((_io_ports[M6812_PORTH] & COL_M ) != COL_M)
    return 'E';
  /* Esperar a que alguna columna llegue a 0 */
  while((_io_ports[M6812_PORTH] & COL_M ) == COL_M);
  /* Estabilización del valor 20 msg = 20000 useg*/
  delayusg(20000UL);
  /* Guardar la columna detectada */
  unsigned char colPulsacion = !(_io_ports[M6812_PORTH] | 0xf0);
  int i = 0;
  while(colPulsacion > 1){
    colPulsacion >> 1;
    i++;
  }
  colPulsacion = i;
  /* Escribir 1 en todas las filas */
  _io_ports[M6812_PORTG] |= FIL_M; 
  
  unsigned char filaPulsacion = 0;
  /* Poner cada fila a 0 */
  for (i = 0; i < 4; i++){
    _io_ports[M6812_PORTG] &= !(1 << i);
    if (_io_ports[M6812_PORTH] & colPulsacion){
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

char teclado_getch_timeout(unsigned int milis){
 unsigned long int tInicial = get_miliseconds(); 
 unsigned long int tActual = get_miliseconds();
 while((tActual-tInicial) < milis){
   tActual = get_miliseconds();
   if ((_io_ports[M6812_PORTH] & COL_M )!=COL_M)
     return teclado_getch();
 }

 return 'T';
}

int main(void){
  serial_init();
  teclado_init();
  while(1){
  /* Bits de columnas estan a 1 ??*/
  if (teclado_getch() == 'E')
    serial_print("bits de columnas no están a 1 ERROR!\n");
  else
    serial_print("bits de columnas están a 1\n");
  }
  while(1);
}

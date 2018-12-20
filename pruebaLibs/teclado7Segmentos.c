#include <temporizador.h>
#include <e_s_lib.h>

/* Los bits a 1 son los que el puerto está usando */
#define COL_M 0x54 //  01010100
#define FIL_M 0x2B //  00101011
#define C1 // 00010000
#define C2 // 00000000
#define C3 // 00010000
#define F1 // 00010000
#define F2 // 00010000
#define F3 // 00010000
#define F4 // 00010000
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
   *  0  1  0  1  0  1  1 */
  configurar_puerto('H', 0, 0);
  configurar_puerto('H', 1, 1);
  configurar_puerto('H', 0, 2);
  configurar_puerto('H', 1, 3);
  configurar_puerto('H', 0, 4);
  configurar_puerto('H', 1, 5);
  configurar_puerto('H', 1, 6);

  /* Todas las filas a 0 */
  escribir_puerto('H', 0);
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
  /* Calcular la columna          */
  unsigned char bitPulsacion = !((_io_ports[M6812_PORTH] | !COL_M)& COL_M); 
  int i = 0;
  unsigned char aux = bitPulsacion;
  while(aux > 1){
    if(aux & 1) i++;
    aux >>= 1;
  }
  int colPulsacion = i;
  /* Calcular la fila */
  /* Escribir 1 en todas las filas */
  _io_ports[M6812_PORTH] |= FIL_M; 
  int filaPulsacion = 0;
  /* Poner cada fila a 0 */
  for (i = 0; i < 4; i++){
    _io_ports[M6812_PORTH] &= !(1 << i);
    if (!(_io_ports[M6812_PORTH] & bitPulsacion)){ /* Si la columna pulsada baja a 0 */
      filaPulsacion = i;
      break;
    }
    _io_ports[M6812_PORTH] |= (1 << i);
  }

  /* Esperar a soltar la tecla */
  _io_ports[M6812_PORTH] &= !FIL_M; 
  while((_io_ports[M6812_PORTH] & COL_M ) != COL_M);
  delayusg(20000UL);

  return teclado[filaPulsacion][colPulsacion];
}

char teclado_getch_timeout(unsigned int milis){
 unsigned long int tInicial = get_miliseconds(); 
 unsigned long int tActual = tInicial;
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

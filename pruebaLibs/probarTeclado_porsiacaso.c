#include <temporizador.h>
#include <e_s_lib.h>

/* Máscaras para conexionado loco */
#define C1 0x10     // 00010000
#define C2 0x40     // 01000000
#define C3 0x04     // 00000100
#define F1 0x20     // 00100000
#define F2 0x01     // 00000001
#define F3 0x02     // 00000010
#define F4 0x08     // 00001000
#define COL_M (C1 | C2 | C3)        // 01010100
#define FIL_M (F1 | F2 | F3 | F4)   // 00101011
#define FACTOR_T 7

/**********Conexionado******************** 
* C2 F1 C1 F4 C3 F3 F2 Pines de Dixen    *
*  0  1  0  1  0  1  1 Entrada/Salida    * 
* ***************************************/


/******************* VARIABLES GLOBALES *********************/
char teclado[4][3] = {
// C1  C2  C3
  {'1','2','3' }, // F1
  {'4','5','6' }, // F2
  {'7','8','9' }, // F2
  {'*','0','#' }, // F2
};

char colBit[3] = {C1 , C2, C3};
char filBit[4] = {F1, F2, F3, F4};
/************************************************************/
void teclado_init(){
  init_temporizador(FACTOR_T);
/**********Conexionado******************** 
* C2 F1 C1 F4 C3 F3 F2 Pines de Dixen    *
*  0  1  0  1  0  1  1 Entrada/Salida    * 
* ***************************************/
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

int getColPulsacion(unsigned char portBits){
  portBits &= COL_M;
  int i;
  for(i = 0; i < 3; i++){
    if(portBits & colBit[i])
      break;
  }
  return i;
}

int getFilPulsacion(int colPulsacion){
  /* Escribir 1 en todas las filas */
  _io_ports[M6812_PORTH] |= FIL_M; 
  int i;
  /* Poner cada fila a 0 */
  for (i = 0; i < 4; i++){
    _io_ports[M6812_PORTH] &= !filBit[i];
    if (!(_io_ports[M6812_PORTH] & colBit[colPulsacion]))
      break;
    _io_ports[M6812_PORTH] |= filBit[i];
  }
  return i;
}

char teclado_getch(){
  /* Salir si columnas no están a 1 */
  if ((_io_ports[M6812_PORTH] & COL_M ) != COL_M)
    return 'E';
  /* Esperar a que alguna columna llegue a 0 */
  while((_io_ports[M6812_PORTH] & COL_M ) == COL_M);
  /* Estabilización del valor 20 msg = 20000 useg*/
  delayusg(20000UL);
  int colPulsacion = getColPulsacion(_io_ports[M6812_PORTH]);
  int filaPulsacion = getFilPulsacion(colPulsacion);
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

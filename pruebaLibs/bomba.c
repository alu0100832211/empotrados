////////////////////////////////////////////////////////////////////////////////
//////////////////////CODIGO DEL TECLADO////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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
* H6 H5 H4 H3 H2 H1 H0                   *
*  0 Entrada 1 Salida                    * 
*  ***************************************/ 
  configurar_puerto('H', 0, 0);
  configurar_puerto('H', 0, 2);
  configurar_puerto('H', 0, 4);
  configurar_puerto('H', 0, 3);
  configurar_puerto('H', 0, 1);
  configurar_puerto('H', 0, 5);
  configurar_puerto('H', 0, 6);
  configurar_puerto('H', 1, 0);
  configurar_puerto('H', 1, 1);
  configurar_puerto('H', 1, 3);
  configurar_puerto('H', 1, 5);

  /* Todas las filas a 0 */
  escribir_puerto('H', 0);
  pull_up('H', 1);
}

int getColPulsacion(unsigned char portBits){
  portBits &= COL_M;
  //serial_print("\n");
  //serial_print("Col pulsada: portBits ");
  //serial_printbinbyte(~portBits);
  //serial_print("\n");
  int i;
  for(i = 0; i < 3; i++){
  //  serial_printbinbyte(colBit[i]);
  //  serial_print(" & ");
  //  serial_printbinbyte(!portBits);
  //  serial_print("\n");
    if(~portBits & colBit[i])
      break;
  }
  //serial_printdecbyte(i);
  //serial_print("\n");
  return i;
}

int getFilPulsacion(int colPulsacion){
  /* Escribir 1 en todas las filas */
  _io_ports[M6812_PORTH] |= FIL_M; 
  int i;
  /* Poner cada fila a 0 */
  for (i = 0; i < 4; i++){
    _io_ports[M6812_PORTH] &= ~filBit[i];
    if (!(_io_ports[M6812_PORTH] & colBit[colPulsacion]))
      break;
    _io_ports[M6812_PORTH] |= filBit[i];
  }
  //serial_printdecbyte(i);
  //serial_print("\n");
  return i;
}

char teclado_getch(){
  /* Salir si columnas no están a 1 */
//  if ((_io_ports[M6812_PORTH] & COL_M ) != COL_M){
//    serial_print("columnas no están a 1\n");
//    return 'E';
//  }
  /* Esperar a que alguna columna llegue a 0 */
  while((_io_ports[M6812_PORTH] & COL_M ) == COL_M);
  /* Estabilización del valor 20 msg = 20000 useg*/
  delayusg(20000UL);
  int colPulsacion = getColPulsacion(_io_ports[M6812_PORTH]);
  int filaPulsacion = getFilPulsacion(colPulsacion);
  /* Esperar a soltar la tecla */
  _io_ports[M6812_PORTH] &= ~FIL_M; 
  while((_io_ports[M6812_PORTH] & COL_M ) != COL_M);
  delayusg(20000UL);
  
  //serial_print("Fila: ");
  //serial_print(filaPulsacion);
  //serial_print("\n");
  //serial_print("Columna: ");
  //serial_print(colPulsacion);
  //serial_print("\n");

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
////////////////////////////////////////////////////////////////////////////////
//////////////////////CODIGO DEL SSD ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <atd.h>
#define FACTOR_T 7
#define PUERTO_ATD 0

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
  serial_init();

  for (digitPos = 0; digitPos < 4; digitPos++){ //Configurar el 7-segmento que se enciende
    activate7S = 0x80 >> digitPos;      //10000000 >> digitPos
    digito[digitPos] |= activate7S;
    //digito[digitPos] &= activate7S;
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
  for (i = 3; i >= 0; i--){
    digits_refreshed[i] = numero % 10;
    numero /= 10;
  }

  sieteSeg_digitos(digits_refreshed);
}

/* programa que lee por teclado un número de 
0-100, lo representa en los 7-segmentos y
hace que el motor gire a una velocidad proporcional 
al número introducido. Usar las teclas * y #
para confirmar y cancelar la introducción del número, 
respectivamente. */
#include <pwm_lib.h>

int pow10[3] = {1, 10, 100};

unsigned int atoi(char c){
  return (int)(c-48); //ASCII
}
unsigned int teclado_getch3(void){
  char digit;
  int valor = 0;
  int i;
  for (i = 2; i <= 0; i--){
    digit = teclado_getch();
    valor += atoi(digit)*pow10[i];
  }
  return valor;
}

unsigned int check(int valor){
  return (valor >= 0 && valor <= 100);
}

int main(void){
  serial_init();
  teclado_init();
  sieteSeg_init();
 
  unsigned int valor;
  char confirmar;
  while(1){
    valor = teclado_getch3();
    sieteSeg_valor(valor);
    confirmar = teclado_getch();
    if ((confirmar == '*' ) && (check(valor))){
      //Frecuencia máxima motor = 5000Hz  
//void pwd_generalizado(unsigned int channel, unsigned long frec, unsigned int pol);
    //pin 40 PP0/PW0
      pwd_generalizado(0, valor*50, 0); //valor = %
    }
    else{
      sieteSeg_valor(0);
      serial_print("Valor incorrecto: ");
      serial_printdecbyte(valor);
      serial_print("\n");
    }
  }
}

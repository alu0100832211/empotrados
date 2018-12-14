#include <temporizador.h>
#include <e_s_lib.h>

void sieteSeg_init(){
  //Inicializacion
  configurar_puerto('G', 1, "0 1 2 3 4 5 6 7");
}

void sieteSeg_digitos(unsigned char* array){
  // recibirá puntero a array de al menos
  //4 bytes. A partir de ese momento se mostrará en cada 7-segmentos valor correspondiente a
  //las primeras 4 posiciones del array pasado.

  //1000-0001 0100-0000 0010-0000 0001-0000   1000
  char led3 = array[3];
  char led2 = array[2];
  char led1 = array[1];
  char led0 = array[0];

  // char array[4];
  // array[0] = 1;
  // array[1] = 0;
  // array[2] = 0;
  // array[3] = 0;

  int i = 0;
  while(true){  
    delayusg(10000 UL);

    //PG7
    // PG6
    //  PG5
    //   PG4
    //    PG3
    //     PG2
    //      PG1
    //       PG0
    //        PORTG
    //

    if (i == 0){
      _io_ports[M6812_PORT_G] = led0;
    }
    if (i == 1){
      _io_ports[M6812_PORT_G] = led1;
    }
    if (i == 2){
      _io_ports[M6812_PORT_G] = led2;
    }
    if (i == 3){
      _io_ports[M6812_PORT_G] = led3;
    }

    i++;
    if (i==4){
      i = 0;    
    }
  }


}
int main (void){
  char array[4];
  array[0] = 0b00010000;
  array[1] = 0b00010001;
  array[2] = 0b00010010;
  array[3] = 0b00010011;

}

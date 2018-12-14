#include <atd.h>

typedef unsigned short res;

#define res10 1
#define res8  0

void simple_test(unsigned short port){
  word resultado[8];
  word n;
  unsigned short i;
  if(atd_start_conversion(port)){
      serial_print("\nConversión iniciada");
      atd_wait_for_conversor(port);
      serial_print("\nResultados:\n");
      atd_get_data(&resultado[0], &n, port);
      for(i = 0; i < n; i++){
          serial_printdecword(resultado[i]);
          serial_print("\n");
      }
  }
}


void prueba_resolucion(res resolucion,unsigned short port){
    serial_print("\nProbando las resoluciones");
    atd_default_config(port);
    if(resolucion == res8)
        atd_set_resolution10(port);
    simple_test(port);
    serial_print("\n");

}

void prueba_tiempos_de_muestreo(unsigned short port){
    serial_print("\nProbando con tiempos de muestreo");
    atd_default_config(port);

    serial_print("\nCon 2 ciclos\n");
    atd_set_sampling_time(2,0);
    simple_test(port);
    serial_print("\n");

    serial_print("\nCon 4 ciclos\n");
    atd_set_sampling_time(4,0);
    simple_test(port);
    serial_print("\n");

    serial_print("\nCon 8 ciclos\n");
    atd_set_sampling_time(8,0);
    simple_test(port);
    serial_print("\n");

    serial_print("\nCon 16 ciclos\n");
    atd_set_sampling_time(16,0);
    simple_test(port);
    serial_print("\n");
}

void prueba_conversion_continua(unsigned short port){
    serial_print("\nProbando el modo de conversión continua");
    atd_default_config(port);
    atd_activate_continuous_conversion(port);

    word resultado[8];
    word n;
    unsigned short i,j;


    if(atd_start_conversion(port)){
        for(i = 0; i < 10; i++){
            serial_print("\nRealizando conversión número: ");
            serial_printdecword(i);
            atd_wait_for_conversor(port);

            atd_get_data(&resultado[0], &n, port);
            serial_print("\nResultados:\n");
            for(j=0; j<n; j++){
                serial_printdecword(resultado[i]);
                serial_print("\n");
            }

        }

    }
}

void prueba_multiples_conversiones(unsigned short port){
    serial_print("\nProbando con distinta cantidad de conversiones");
    atd_default_config(port);

    serial_print("\n1 conversión:\n");
    atd_set_successive_conversions(1,port);
    simple_test(port);
    serial_print("\n");

    serial_print("\n4 conversiones:\n");
    atd_set_successive_conversions(4,port);
    simple_test(port);
    serial_print("\n");

    serial_print("\n8 conversones:\n");
    atd_set_successive_conversions(8,port);
    simple_test(port);
    serial_print("\n");


}

void prueba_justificacion(unsigned short port){
    serial_print("\nProbando las dos formas de justificación");
    atd_default_config(port);

    serial_print("\nJustificacion a la izquierda:\n");
    atd_align_left(true,port);
    simple_test(port);
    serial_print("\n");

    serial_print("\nJustificacion a la derecha:\n");
    atd_align_left(false,port);
    simple_test(port);
    serial_print("\n");
}


int main(){
    serial_init();
    unsigned short puerto = 0;
    serial_print("\nSTART");
    atd_default_config(puerto);
    atd_activate_module(puerto);
    /*Poner aquí las pruebas, ya sea con un menú interactivo o una detrás de otra*/

    return 0;
}

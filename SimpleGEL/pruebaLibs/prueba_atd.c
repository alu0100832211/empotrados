#include <atd.h>

typedef unsigned short res;

#define res10 1
#define res8  0

void simple_test(unsigned short port){
  word resultado[8];
  word n;
  unsigned short i;

  if(atd_start_conversion(port)){
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

    serial_print("\nCon 2 ciclos");
    atd_set_sampling_time(2,0);
    simple_test(port);
    serial_print("\n");

    serial_print("\nCon 4 ciclos");
    atd_set_sampling_time(4,0);
    simple_test(port);
    serial_print("\n");

    serial_print("\nCon 8 ciclos");
    atd_set_sampling_time(8,0);
    simple_test(port);
    serial_print("\n");

    serial_print("\nCon 16 ciclos");
    atd_set_sampling_time(16,0);
    simple_test(port);
    serial_print("\n");
}

void prueba_conversion_continua(unsigned short port){
    serial_print("\nProbando el modo de conversión continua");
    atd_default_config(port);
    atd_activate_continuous_conversion(port);
    atd_set_FIFO(true,port);

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
                serial_printdecword(resultado[j]);
                serial_print("\n");
            }

        }

    }
}


void prueba_multiples_conversiones(unsigned short port){
    serial_print("\nProbando con distinta cantidad de conversiones");
    atd_default_config(port);

    serial_print("\n1 conversión:");
    atd_set_successive_conversions(1,port);
    simple_test(port);
    serial_print("\n");

    serial_print("\n4 conversiones:");
    atd_set_successive_conversions(4,port);
    simple_test(port);
    serial_print("\n");

    serial_print("\n8 conversones:");
    atd_set_successive_conversions(8,port);
    simple_test(port);
    serial_print("\n");


}

void prueba_justificacion(unsigned short port){
    serial_print("\nProbando las dos formas de justificación");
    atd_default_config(port);

    serial_print("\nJustificacion a la izquierda:");
    atd_align_left(true,port);
    simple_test(port);
    serial_print("\n");

    serial_print("\nJustificacion a la derecha:");
    atd_align_left(false,port);
    simple_test(port);
    serial_print("\n");
}

void print_last_result(word * result, word * n){
    serial_print("\nPrinting last result");
    atd_get_last_result(&result[0], &n);
    unsigned short i = 0;
    for(i = 0; i < n; i++){
	serial_printdecword(result[i]);
	serial_print("\n");
    }

}


int main(){
    word result[8];
    word n;
    serial_init();
    unsigned short puerto = 0;
    serial_print("\nSTART");
    while(1) {
      atd_default_config(puerto);
      atd_activate_module(puerto);


      prueba_resolucion(res8,puerto);
      prueba_resolucion(res10,puerto);
      prueba_conversion_continua(puerto);
      prueba_multiples_conversiones(puerto);
      prueba_justificacion(puerto);
      prueba_tiempos_de_muestreo(puerto);

      // Interrupción no manejada hace que se resete el micro.
      // atd_default_config(puerto);
      // atd_activate_interrupt(puerto);


      serial_print("\nTerminado. Pulsa para nueva iteración");
      serial_recv();
    }

}

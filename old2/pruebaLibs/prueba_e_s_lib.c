/**
 * @file doxygen_c.h
 * @author Sergio y Santiago
 * @date 14/12/18
 * @brief Includes necesarios
 *
 * Librería E/S P1 Sistemas Empotrados
 */

#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>
#include <e_s_lib.h>


// MAIN PROGRAM

/**
 * @brief MAIN
 *
 * Programa de ejemplo
 */
int main (void)
{
  serial_init();
  serial_print("\nPrueba E/S LEDS a Puertos G y H... \n");


  int opt = 0;

  while(opt != 8)
  {

    serial_print("Seleccione una opción: \n");
    serial_print("1. Configurar un puerto como entrada. \n");
    serial_print("2. Configurar un puerto como salida. \n");
    serial_print("3. Leer un puerto. \n");
    serial_print("4. Escribir un puerto \n");
    serial_print("5. Encender pullup en puerto específico. \n");
    serial_print("6. Apagar pullup en puerto específico. \n");
    serial_print("7. Encender/Apagar pullup en todos los puertos (A,B,E,G,H) \n");
    serial_print("8. Salir.\n");

    opt = serial_getdecbyte();


    switch(opt)
    {

      case 1: {

        int sel = 0;
        bool temp = 1;
        int pines;

        while (sel != 3)
        {

          serial_print("Seleccione un puerto a configurar: \n");
          serial_print("1.Puerto H como entrada.\n");
          serial_print("2. Puerto G como entrada.\n");
          serial_print("3. Cancelar.\n");
          sel = serial_getdecbyte();

          if (sel == 1 || sel == 2)
          {
            serial_print("\nIntroduzca los pines a configurar (0 1 2 5 7).\n");
            pines= serial_getdecbyte();
          }

          switch (sel)
          {
            case 1: temp = configurar_puerto('H', 0, pines); break;

            case 2: temp = configurar_puerto('G', 0, pines); break;

            case 3: sel = 3; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

          if(!temp)
          {
            serial_print("Ha ocurrido algún error configurando el puerto.\n");
            sel = 0;
          }

        }

        opt = 0;

      } break;

      case 2: {

        int sel = 0;
        bool temp = 1;
        int pines;

        while (sel != 3)
        {

          serial_print("Seleccione un puerto a configurar: \n");
          serial_print("1.Puerto H como salida.\n");
          serial_print("2. Puerto G como salida.\n");
          serial_print("3. Cancelar.\n");
          sel = serial_getdecbyte();

          if (sel == 1 || sel == 2)
          {
            serial_print("\nIntroduzca los pines a configurar (0 1 2 5 7).\n");
            pines= serial_getdecbyte();
          }

          switch (sel)
          {

            case 1: temp = configurar_puerto('H', 1, pines); break;

            case 2: temp = configurar_puerto('G', 1, pines); break;

            case 3: sel = 3; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

          if(!temp)
          {
            serial_print("Ha ocurrido algún error configurando el puerto.\n");
            sel = 0;
          }

        }

        opt = 0;


      } break;

      case 3: {

        byte value;
        int sel = 0;

        while (sel != 3)
        {

          serial_print("Seleccione un puerto a leer: \n");
          serial_print("1.Puerto H.\n");
          serial_print("2. Puerto G.\n");
          serial_print("3. Cancelar.\n");

          sel = serial_getdecbyte();

          switch (sel)
          {
            case 1: value = leer_puerto('H'); break;

            case 2: value = leer_puerto('G'); break;

            case 3: sel = 3; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

        }

        opt = 0;


      } break;

      case 4: {

        byte value;
        int sel = 0;

        while (sel != 3)
        {

          serial_print("Seleccione un puerto a escribir: \n");
          serial_print("1.Puerto H.\n");
          serial_print("2. Puerto G.\n");
          serial_print("3. Cancelar.\n");
          sel = serial_getdecbyte();

          if (sel == 1 || sel == 2)
          {
            serial_print("Introduzca el valor a escribir.\n");
            value = serial_getbinbyte();
          }

          switch (sel)
          {

            case 1: escribir_puerto('H', value); break;

            case 2: escribir_puerto('G', value); break;

            case 3: sel = 3; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

        }
        opt = 0;

      } break;

      case 5: {


        int sel = 0;
        bool temp = 1;

        while (sel != 6)
        {

          serial_print("Seleccione un puerto: \n");
          serial_print("1. Encender pullup Puerto A.\n");
          serial_print("2. Encender pullup Puerto B.\n");
          serial_print("3. Encender pullup Puerto E.\n");
          serial_print("4. Encender pullup Puerto G.\n");
          serial_print("5. Encender pullup Puerto H.\n");
          serial_print("6. Cancelar.\n");


          sel = serial_getdecbyte();

          switch (sel)
          {

            case 1: temp = pull_up('A', 1); break;

            case 2: temp = pull_up('B', 1); break;

            case 3: temp = pull_up('E', 1); break;

            case 4: temp = pull_up('H', 1); break;

            case 5: temp = pull_up('G', 1); break;

            case 6: sel = 6; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

          if(!temp)
          {
            serial_print("Ha ocurrido algún error configurando el puerto.\n");
            sel = 0;

          }

        }

        opt = 0;

      } break;

      case 6: {


        int sel = 0;
        bool temp = 1;

        while (sel != 6)
        {

          serial_print("Seleccione un puerto: \n");
          serial_print("1. Apagar pullup Puerto A.\n");
          serial_print("2. Apagar pullup Puerto B.\n");
          serial_print("3. Apagar pullup Puerto E.\n");
          serial_print("4. Apagar pullup Puerto G.\n");
          serial_print("5. Apagar pullup Puerto H.\n");
          serial_print("6. Cancelar.\n");

          sel = serial_getdecbyte();

          switch (sel)
          {

            case 1: temp = pull_up('A', 0); break;

            case 2: temp = pull_up('B', 0); break;

            case 3: temp = pull_up('E', 0); break;

            case 4: temp = pull_up('H', 0); break;

            case 5: temp = pull_up('G', 0); break;

            case 6: sel = 6; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

          if(!temp)
          {
            serial_print("Ha ocurrido algún error configurando el puerto.\n");
            sel = 0;

          }

        }

        opt = 0;

      } break;

      case 7: {

        int sel = 0;
        bool temp = 1;

        while (sel != 3)
        {

          serial_print("Seleccione acción: \n");
          serial_print("1. Encender pullup.\n");
          serial_print("2. Apagar pullup.\n");
          serial_print("3. Cancelar.\n");

          sel = serial_getdecbyte();

          switch (sel)
          {

            case 1: temp = pull_up_all(1); break;

            case 2: temp = pull_up_all(0); break;

            case 3: sel = 3; break;

            default: serial_print("Seleccione una opción válida.\n"); break;
          }

          if(!temp)
          {
            serial_print("Ha ocurrido algún error configurando el puerto.\n");
            sel = 0;

          }

        }
        opt = 0;

      } break;

      case 8: serial_print("Saliendo...\n"); break;

      default: serial_print("Introduzca una opción válida.\n"); break;

    }


  }

}

/* Manejador interrupciones de puertos G y H (vi_kwgh)  */


/**
 * @brief Método para administrar interrupciones
 *
 *
 */
void __attribute__((interrupt))vi_kwgh(void) {
  int irqG = 0;  //pin interrupc puerto G
  int irqH = 0;  //pin interrupc puerto H
  char g = _io_ports[M6812_KWIFG];
  char h = _io_ports[M6812_KWIFH];

  while(_io_ports[M6812_KWIEG] & M6812_KWIFG) { //activada interrup puerto G
    if (g){ //si bit activado...
      g = 1; //desactiva flag (igualando a '1')
      serial_print("interrupcion pin KWIFG.\n"); //gestion interrup.
      serial_printdecbyte(irqG);
      serial_print("\n");
    }
    g >> 1; //desplaza bits una posic. a la izqda
    irqG = irqG++ % 8;
  }

  while(_io_ports[M6812_KWIEH] & M6812_KWIFH) { //activada interrup puerto H
    if (h){ //si bit activado...
      h = 1; //desactiva flag (igualando a '1')
      serial_print("interrupcion pin KWIFH.\n"); //gestion interrup.
      serial_printdecbyte(irqH);
      serial_print("\n");
    }
    h >> 1; //desplaza bits una posic. a la izqda
    irqH = irqH++ % 8;
  }
}

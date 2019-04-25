#include <atd.h>

typedef unsigned short res;

#define res10 1
#define res8  0


void print_last_result(word * result, word * n){
    serial_print("\nPrinting last result");
    atd_get_last_result(result, n);
    unsigned short i = 0;
    for(i = 0; i < *n; i++){
      serial_printdecword(result[i]);
      serial_print("\n");
    }

}


int main(){

    lock();

    serial_init();
    unsigned short puerto = 0;
    word result[8];
    word n;

    serial_print("\nSTART");
    atd_default_config(puerto);
    atd_activate_continuous_conversion(puerto);
    atd_activate_interrupt(puerto);
    atd_activate_module(puerto);
    atd_interruptAction(&print_last_result,&result, &n);

    unlock();

    atd_start_conversion(puerto);

    while(1);

    return 0;
}

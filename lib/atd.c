#include <atd.h>

unsigned short lastResults[8];
unsigned short lastN;

char start_pin[8] = {A_IN_0, A_IN_1, A_IN_2, A_IN_3, A_IN_4, A_IN_5, A_IN_6, A_IN_7};

unsigned short * resultDest;
unsigned short * nDest;

void (*interruptAction_f)(unsigned short*, unsigned short*);

void atd_set_resolution10( unsigned short port){
    if( port <= 1)
        _io_ports[M6812_ATD0CTL4 + port*DIST_AD] |= M6812B_RES10;
}

void atd_set_resolution8(unsigned short port){
    if( port <= 1)
        _io_ports[M6812_ATD0CTL4 + port*DIST_AD] &= ~(M6812B_RES10);
}

void atd_set_sampling_time(unsigned short cicles, unsigned short port){
    char mask  = 0;
    char reset = ~(M6812B_SMP1 | M6812B_SMP0);
    switch( cicles){
        case 2:
            mask = 0;
        break;

	      case 4:
            mask |= M6812B_SMP0;
	      break;

	      case 8:
            mask |= M6812B_SMP1;
	      break;

	      case 16:
            mask |= M6812B_SMP1 | M6812B_SMP0;
	      break;

	      default:
            mask = 255;
    }

    if(mask != 255 && port <= 1){
        _io_ports[M6812_ATD0CTL4 + port*DIST_AD] &= reset;
	      _io_ports[M6812_ATD0CTL4 + port*DIST_AD] |= mask;
    }

}

void atd_read_multiple_pin(int activate, unsigned short port){
    if(port <= 1){
	if(activate)
	    _io_ports[M6812_ATD0CTL5 + port*DIST_AD] |= M6812B_MULT;
	else
	    _io_ports[M6812_ATD0CTL5 + port*DIST_AD] &= ~M6812B_MULT;
    }
}

void atd_set_start_pin(unsigned short pin, unsigned short port){
    if(port <= 1 && pin <= 7){
        _io_ports[M6812_ATD0CTL5 + port*DIST_AD] &= ~(A_IN_7);
        _io_ports[M6812_ATD0CTL5 + port*DIST_AD] |= start_pin[pin];
    }
}

void atd_set_submodule_clock(char val, unsigned short port){
    if (port <= 1 && (val > 0 && val < 8) ){
        // Ponemos a 0 PRS4:PRS0 para poder realizar la OR sin quedarnos con el resto.
        _io_ports[M6812_ATD0CTL4 + port*DIST_AD] &= ~(M6812B_PRS4 | M6812B_PRS4 | M6812B_PRS2 | M6812B_PRS1 | M6812B_PRS0 );

        // En este caso sabemos que PRS4:PRS0 son los 5 bits menos significativos
        // así que podemos operar con el valor directamente
        _io_ports[M6812_ATD0CTL4 + port*DIST_AD] |= val;
    }
}

void atd_set_FIFO(int on, unsigned short port){
    if(port <= 1){
      if (on == true)
        _io_ports[M6812_ATD0CTL3 + port*DIST_AD] |= M6812B_FIFO;
      else
        _io_ports[M6812_ATD0CTL3 + port*DIST_AD] &= ~(M6812B_FIFO);
    }
}

void atd_wait_for_conversor(unsigned short port){
    if(port <= 1){
        while(!(_io_ports[M6812_ATD0STAT0 + port*DIST_AD] & M6812B_SCF));
    }
}

void atd_get_data(unsigned short * data, unsigned short * n , unsigned short port){
    if(port <= 1 && (_io_ports[M6812_ATD0STAT0 + port*DIST_AD] & M6812B_SCF)){
        char n_conversiones;
        if(_io_ports[M6812_ATD0CTL5 + port*DIST_AD] & M6812B_S8C)
            n_conversiones = 8;
        else{
            if(_io_ports[M6812_ATD0CTL3 + port*DIST_AD] & M6812B_S1C)
                n_conversiones = 1;
            else
		            n_conversiones = 4;
	      }

	      unsigned short last = n_conversiones-1;
	      if(_io_ports[M6812_ATD0CTL3 + port*DIST_AD] & M6812B_FIFO)
	          last  = (M6812B_CC2 | M6812B_CC1 | M6812B_CC0 ) & _io_ports[M6812_ATD0STAT0 + port*DIST_AD];

        unsigned short first = (last + 9 - n_conversiones) % 8;
        unsigned short inx = first;
	      unsigned short i;
        for(i = 0; i < n_conversiones; i++ ){
            data[inx%8] =  0;
            data[inx%8] |= _io_ports[(M6812_ADR00H + port*DIST_AD) + (inx%8)*DIST_RES];
            data[inx%8] = data[inx%8] << 8;
            data[inx%8] |= _io_ports[(M6812_ADR00L + port*DIST_AD) + (inx%8)*DIST_RES];
            inx++;
        }
        (*n) = n_conversiones;
    }
}

void atd_reset_on_read(int ror, unsigned short port){
    if(port <= 1){
        if(ror)
            _io_ports[M6812_ATD0CTL2 + port*DIST_AD] |= M6812B_AFFC;
        else
            _io_ports[M6812_ATD0CTL2 + port*DIST_AD] &= ~(M6812B_AFFC);
    }
}

void atd_align_left(int left, unsigned short port){
    if(port <= 1){
        if(left)
            _io_ports[M6812_ATD0CTL2 + port*DIST_AD] &= ~(M6812B_DJM);
        else
	    _io_ports[M6812_ATD0CTL2 + port*DIST_AD] |=   M6812B_DJM ;
    }
}



int atd_start_conversion(unsigned short port){
    int res = false;
    if(port <= 1){
	char prevState = _io_ports[M6812_ATD0CTL5 + port*DIST_AD];
	_io_ports[M6812_ATD0CTL5 + port*DIST_AD] = 0;
	_io_ports[M6812_ATD0CTL5 + port*DIST_AD] = prevState;
        /*if(!(_io_ports[M6812_ATD0STAT0 + port*DIST_AD] & M6812B_SCF)){

            res = true;
	}*/
    }
    return true;
}


void atd_set_successive_conversions(unsigned short conversions, unsigned short port) {

   _io_ports[M6812_ATD0CTL5 + port*DIST_AD] &= ~(M6812B_S8C);
   _io_ports[M6812_ATD0CTL3 + port*DIST_AD] &= ~(M6812B_S1C);

   switch(conversions) {

      case 1:
         _io_ports[M6812_ATD0CTL3 + port*DIST_AD] |= M6812B_S1C;
      break;

      case 8:
         _io_ports[M6812_ATD0CTL5 + port*DIST_AD] |= M6812B_S8C;
      break;

   }

}

void atd_activate_module(unsigned short port){

   if(port<=1)
      _io_ports[M6812_ATD0CTL2 + port*DIST_AD] |= M6812B_ADPU;
}

void atd_deactivate_module(unsigned short port){

   if(port<=1)
      _io_ports[M6812_ATD0CTL2 + port*DIST_AD] &= ~(M6812B_ADPU);
}

void atd_activate_continuous_conversion(unsigned short port) {

   if(port<=1)
      _io_ports[M6812_ATD0CTL5 + port*DIST_AD] |= M6812B_SCAN;
}

void atd_deactivate_continuous_conversion(unsigned short port) {

   if(port<=1)
      _io_ports[M6812_ATD0CTL5 + port*DIST_AD] &= ~(M6812B_SCAN);
}

void atd_activate_interrupt(unsigned short port){
    if(port <=1){
        _io_ports[M6812_ATD0CTL2 + port*DIST_AD] |= M6812B_ASCIE;
    }
}

void atd_deactivate_interrupt(unsigned short port){
    if(port <= 1){
        _io_ports[M6812_ATD0CTL2 + port*DIST_AD] &= ~(M6812B_ASCIE);
    }

}

void atd_default_config(unsigned short port){

    atd_deactivate_interrupt(port);
    // Resolución a 8 bits
    atd_set_resolution8(port);

    _io_ports[M6812_ATD0CTL2 + port*DIST_AD] &= ~(M6812B_ASWAI);
    // Numero de ciclos = 8
    atd_set_sampling_time(2,port);

    atd_set_successive_conversions(1, port);

    // Justificacion a la izquierda
    atd_align_left(false,port);

    // Resetear al Leer
    atd_reset_on_read(true, port);

    // Leer desde el pin 0
    atd_set_start_pin(0,port);

    // Poner reloj por defecto
    atd_set_submodule_clock(4,port);

    // Modo no FIFO
    atd_set_FIFO(false,port);

    atd_deactivate_continuous_conversion(port);

    atd_read_multiple_pin(false, port);

    // Encender puerto 0
}

void __attribute__((interrupt)) vi_atd(void){
    char port = ((_io_ports[M6812_ATD0STAT0] && M6812B_SCF)? 0: 1);
    _io_ports[M6812_ATD0CTL2 + port*DIST_AD] &= ~(M6812B_ASCIF);

    atd_get_data(&lastResults[0], &lastN, port);



    (*interruptAction_f)(resultDest,nDest);


}

void atd_get_last_result(unsigned short * result, unsigned short * n){
    unsigned short i=0;
    for(i=0; i < lastN; i++){
	result[i] = lastResults[i];
    }
    (*n) = lastN;
}

void atd_interruptAction(void (*f)(unsigned short*, unsigned short*), unsigned short * result, unsigned short * n){
    interruptAction_f = f;
    resultDest = result;
    nDest = n;
}

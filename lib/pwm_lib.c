
#include <sys/ports.h>
#include <sys/param.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <sys/interrupts.h>

#include <pwm_lib.h>


//variable canal, variable periodo y variable frecuencia
unsigned int actual_channel = 0;

/** returns a divider given a frequency, this divider it's the first divider*/
inline void divider_calc (unsigned long frequency)
{
  unsigned char periodo = _io_ports[M6812_PWPER0 + actual_channel];
		unsigned long x = M6812_CPU_E_CLOCK / frequency / periodo;
		if (x <= MAX_PRESCALAR1_VALOR)
		{
			//divisor es 111
			if (x == MAX_PRESCALAR1_VALOR)
			{
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA0);

				_io_ports[M6812_PWCLK] |= (M6812B_PCKB2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB0);
			}
			//divisor es 110
			if ((x < MAX_PRESCALAR1_VALOR) && (x >= 64))
			{
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA0);

				_io_ports[M6812_PWCLK] |= (M6812B_PCKB2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB0);
			}
			//divisor es 101
			if ((x < 64) && (x >= 32))
			{
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA0);

				_io_ports[M6812_PWCLK] |= (M6812B_PCKB2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB0);
			}
			//divisor es 100
			if ((x < 32) && (x >= 16))
			{
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA0);

				_io_ports[M6812_PWCLK] |= (M6812B_PCKB2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB0);
			}
			//divisor es 011
			if ((x < 16) && (x >= 8))
			{
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA0);

				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB0);
			}
			//divisor es 010
			if ((x < 8) && (x >= 4))
			{
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA0);

				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB2);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB0);
			}
			//divisor es 001
			if ((x < 4) && (x >= 2))
			{
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKA0);

				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB1);
				_io_ports[M6812_PWCLK] |= (M6812B_PCKB0);
			}
			//divisor es 000
			if (x == 1)
			{
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKA0);

				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB2);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB1);
				_io_ports[M6812_PWCLK] &= (~M6812B_PCKB0);
			}
		}
		else
		{
			//divisor es 111
			_io_ports[M6812_PWCLK] |= (M6812B_PCKA2);
			_io_ports[M6812_PWCLK] |= (M6812B_PCKA1);
			_io_ports[M6812_PWCLK] |= (M6812B_PCKA0);

			_io_ports[M6812_PWCLK] |= (M6812B_PCKB2);
			_io_ports[M6812_PWCLK] |= (M6812B_PCKB1);
			_io_ports[M6812_PWCLK] |= (M6812B_PCKB0);

			//x = x / MAX_PRESCALAR1_VALOR;
			//Cambio de pwdscal para dividir aun mas la frecuencia
		}
}


/** CHANGES ACTUAL CHANNEL: low-high polarity if 0, high-low polarity otherwise */
inline void polarity (unsigned int pol)
{
    if (pol != 0) _io_ports[M6812_PWPOL] |= (M6812B_PPOL0 << actual_channel);
    else _io_ports[M6812_PWPOL] &=  ~(M6812B_PPOL0  << actual_channel);
}

/** left-aligned if 0, center-aligned otherwise */
inline void alignment (unsigned int align)
{
    if(align != 0) _io_ports[M6812_PWCTL] |= (M6812B_CENTR);
    else _io_ports[M6812_PWCTL] &= (~M6812B_CENTR);
}

/** CHANGES ACTUAL CHANNEL: set PWPER directly */
void set_PWPER (unsigned int period)
{
   _io_ports[M6812_PWPER0 + actual_channel] = period;
}

/** CHANGES ACTUAL CHANNEL: set PWDTY directly */
void set_PWDTY (unsigned int period)
{
  _io_ports[M6812_PWDTY0 + actual_channel] = period;
}

/** CHANGES ACTUAL CHANNEL: set duty work by percentage -> 0 1 2 3 4 representating 0 25 50 75 100% */
void set_duty_percentage (unsigned int percent)
{
    int pwper = _io_ports[M6812_PWPER0 + actual_channel];
		int pwdty = pwper * percent / 100;

		_io_ports[M6812_PWDTY0 + actual_channel] = pwdty;

    /*if(actual_channel == 0)
    {
        switch (percent)
        {
            case (0): _io_ports[M6812_PWDTY0] = 0;
            case (1): _io_ports[M6812_PWDTY0] = ( _io_ports[M6812_PWPER0] >> 2); break;
            case (2): _io_ports[M6812_PWDTY0] = ( _io_ports[M6812_PWPER0] >> 1); break;
            case (3): _io_ports[M6812_PWDTY0] = ( _io_ports[M6812_PWPER0] >> 2) | ( _io_ports[M6812_PWPER0] >> 1); break;
            default: _io_ports[M6812_PWDTY0] =  _io_ports[M6812_PWPER0]; break;

        }
    }
    if(actual_channel == 1)
    {
        switch (percent)
        {
            case (0): _io_ports[M6812_PWDTY1] = 0;
            case (1): _io_ports[M6812_PWDTY1] = ( _io_ports[M6812_PWPER1] >> 2); break;
            case (2): _io_ports[M6812_PWDTY1] = ( _io_ports[M6812_PWPER1] >> 1); break;
            case (3): _io_ports[M6812_PWDTY1] = ( _io_ports[M6812_PWPER1] >> 2) | ( _io_ports[M6812_PWPER1] >> 1); break;
            default: _io_ports[M6812_PWDTY1] =  _io_ports[M6812_PWPER1]; break;

        }
    }
    if(actual_channel == 2)
    {
        switch (percent)
        {
            case (0): _io_ports[M6812_PWDTY2] = 0;
            case (1): _io_ports[M6812_PWDTY2] = ( _io_ports[M6812_PWPER2] >> 2); break;
            case (2): _io_ports[M6812_PWDTY2] = ( _io_ports[M6812_PWPER2] >> 1); break;
            case (3): _io_ports[M6812_PWDTY2] = ( _io_ports[M6812_PWPER2] >> 2) | ( _io_ports[M6812_PWPER2] >> 1); break;
            default: _io_ports[M6812_PWDTY2] =  _io_ports[M6812_PWPER2]; break;

        }
    }
    else
    {
        switch (percent)
        {
            case (0): _io_ports[M6812_PWDTY3] = 0;
            case (1): _io_ports[M6812_PWDTY3] = ( _io_ports[M6812_PWPER3] >> 2); break;
            case (2): _io_ports[M6812_PWDTY3] = ( _io_ports[M6812_PWPER3] >> 1); break;
            case (3): _io_ports[M6812_PWDTY3] = ( _io_ports[M6812_PWPER3] >> 2) | ( _io_ports[M6812_PWPER3] >> 1); break;
            default: _io_ports[M6812_PWDTY3] =  _io_ports[M6812_PWPER3]; break;
        }
    }*/
}

/** set current channel */
void set_channel (unsigned int channel)
{
     actual_channel = channel;
     switch (channel)
        {
            case (0):
                _io_ports[M6812_PWCLK] &= (~M6812B_CON01);
                _io_ports[M6812_PWEN] |= M6812B_PWEN0; break;
            case (1):
                _io_ports[M6812_PWCLK] &= (~M6812B_CON01);
                _io_ports[M6812_PWEN] |= M6812B_PWEN1; break;
            case (2):
                _io_ports[M6812_PWCLK] &= (~M6812B_CON23);
                _io_ports[M6812_PWEN] |= M6812B_PWEN2; break;
            default:
                actual_channel = 3;
                _io_ports[M6812_PWCLK] &= (~M6812B_CON23);
                _io_ports[M6812_PWEN] |= M6812B_PWEN3; break;
        }
     _io_ports[M6812_DDRP] |= (M6812B_DDP0 << actual_channel);
}

/** unables all channels */
inline void reset_channel ()
{
_io_ports[M6812_PWEN] &= (~M6812B_PWEN0);
_io_ports[M6812_PWEN] &= (~M6812B_PWEN1);
_io_ports[M6812_PWEN] &= (~M6812B_PWEN2);
_io_ports[M6812_PWEN] &= (~M6812B_PWEN3);
}

//IGUAL QUE ABAJO
/** set PWSCAL for S0 & S1 channels */
/*inline void set_PWSCAL (unsigned int value)
{
 _io_ports[M6812_PWSCAL0 + (actual_channel/2) ] = value;
}*/

//ESTO LO QUITE PORQUE NO SE LO QUE HACE Y LA VERDAD EL DIVISOR LO TENEMOS QUE CREAR CON LA FRECUENCIA QUE NOS VENGA
/** set PWCLK for A & B channels */
/*inline void set_AB_clock( unsigned int value)
{
 if(value < 8) //overflow control
 { if(actual_channel < 2) _io_ports[M6812_PWCLK] = (value << 3);
   else _io_ports[M6812_PWCLK] = value;

 }
}*/

/** a quick method to set up the PWM */
void pwd_generalizado(unsigned int channel, unsigned long frec, unsigned int pol)
{
	//if ((frec > M6812_CPU_E_CLOCK) ||  Warning: comparison is always false due to limited range of data type

	if(frec == 0)
	{
		serial_print("\nERROR NO SE PUEDE OBTENER ESA FRECUENCIA");
	}
	else
	{
		set_PWPER(100);
		set_duty_percentage(15);


		set_channel(channel);
		alignment(0);

		//if(M6812_CPU_E_CLOCK < frec) return;
		divider_calc(frec);
		//if(frec_final > PRECISION) _io_ports[M6812_PWPOL] &= ~(M6812B_PCLK0 << actual_channel); //desplaza el 0
		//else _io_ports[M6812_PWPOL] |= (M6812B_PCLK0 << actual_channel);
		polarity(pol);

		//_io_ports[M6812_PWSCAL0] = 0x20; //00100000 div=32*2
		//_io_ports[M6812_PWSCAL1] = 0x07; //00000111 div=7*2

		// serial_print("\nPWPER = ");
		// serial_printdecword(_io_ports[M6812_PWPER0]);
    //
		// serial_print("\nPWDTY = ");
		// serial_printdecword(_io_ports[M6812_PWDTY0]);
    //
		// serial_print("\nPWCLK = ");
		// serial_printdecword(_io_ports[M6812_PWCLK]);
    //
		// serial_print("\nPWPOL = ");
		// serial_printdecword(_io_ports[M6812_PWPOL]);
	}
}

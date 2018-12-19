
#include <sys/ports.h>
#include <sys/param.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <sys/interrupts.h>

#include <pwm_lib.h>


int actual_channel = 0;
char option;
int pol_v = 0, nb = 0;
unsigned long int frec_v= 0;

/** returns a divider given a frequency*/
inline int fre_calc (unsigned long int frequency)
{
    return (M6812_CPU_E_CLOCK/frequency);
}

/** CHANGES ACTUAL CHANNEL: low-high polarity if 0, high-low polarity otherwise */
inline void polarity (int pol)
{
    if (pol != 0) _io_ports[M6812_PWPOL] |= (M6812B_PPOL0 << actual_channel);
    else _io_ports[M6812_PWPOL] &=  ~(M6812B_PPOL0  << actual_channel);
}

/** left-aligned if 0, center-aligned otherwise */
inline void alignment (int align)
{
    if(align != 0) _io_ports[M6812_PWCTL] |= (M6812B_CENTR);
    else _io_ports[M6812_PWCTL] &= (~M6812B_CENTR);
}

/** CHANGES ACTUAL CHANNEL: set PWPER directly */
void set_PWPER (unsigned char period)
{
   _io_ports[M6812_PWPER0 + actual_channel] = period;
}

/** CHANGES ACTUAL CHANNEL: set PWDTY directly */
void set_PWDTY (unsigned char period)
{
  _io_ports[M6812_PWDTY0 + actual_channel] = period;
}

/** CHANGES ACTUAL CHANNEL: set duty work by percentage -> 0 1 2 3 4 representating 0 25 50 75 100% */
void set_duty_percentage (int percent)
{
    if(actual_channel == 0)
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
    }
}

/** set current channel */
void set_channel (int channel)
{
     actual_channel = channel;
     switch (channel)
        {
            case (0):
                _io_ports[M6812_PWCLK] &= (~M6812B_CON01);
                _io_ports[M6812_PWEN] |= M6812B_PWEN0; break;
            case (1):
                _io_ports[M6812_PWCLK] &= M6812B_CON01;
                _io_ports[M6812_PWEN] |= M6812B_PWEN1; break;
            case (2):
                _io_ports[M6812_PWCLK] &= (~M6812B_CON23);
                _io_ports[M6812_PWEN] |= M6812B_PWEN2; break;
            default:
                actual_channel = 3;
                _io_ports[M6812_PWCLK] &= M6812B_CON23;
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

/** set PWSCAL for S0 & S1 channels */
inline void set_PWSCAL (int value)
{
 _io_ports[M6812_PWSCAL0 + (actual_channel/2) ] = value;
}

/** set PWCLK for A & B channels */
inline void set_AB_clock(int value)
{
 if(value < 8) //overflow control
 { if(actual_channel < 2) _io_ports[M6812_PWCLK] = (value << 3);
   else _io_ports[M6812_PWCLK] = value;

 }
}

/** a quick method to set up the PWM */
void pwd_generalizado(int channel, unsigned long frec, int pol)
{
   set_PWPER(100);
  //set_PWDTY(75);
  set_duty_percentage(1);


  set_channel(channel);
  //if ((M6812_CPU_E_CLOCK)  / 100000) numciclos = M6812_CPU_E_CLOCK/1000000 * useg;
  //else n
  alignment(0);
  if(M6812_CPU_E_CLOCK < frec) return;
  unsigned long frec_final=fre_calc(frec);
  if(frec_final > PRECISION) _io_ports[M6812_PWPOL] &= ~(M6812B_PCLK0 << actual_channel); //desplaza el 0
  else _io_ports[M6812_PWPOL] |= (M6812B_PCLK0 << actual_channel);
  polarity(pol);
  //if ((frec)  / 100000) numciclos = fre/1000000 * useg;
   // si el clock es mayor que la frecuencia obtenida    _io_ports[]
  //else

  _io_ports[M6812_PWSCAL0] = 0x20; //00100000 div=32*2
  _io_ports[M6812_PWSCAL1] = 0x07; //00000111 div=7*2

    serial_print("\nPWPER = ");
    serial_printdecword(_io_ports[M6812_PWPER0]);

    serial_print("\nPWDTY = ");
    serial_printdecword(_io_ports[M6812_PWDTY0]);

    serial_print("\nPWCLK = ");
    serial_printdecword(_io_ports[M6812_PWCLK]);

    serial_print("\nPWPOL = ");
    serial_printdecword(_io_ports[M6812_PWPOL]);
}


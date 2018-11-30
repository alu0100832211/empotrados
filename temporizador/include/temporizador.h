#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>


typedef unsigned char byte;
typedef unsigned int bytes2;
typedef unsigned long bytes4;

void init_temporizador(int factor);
bytes4 get_microseconds(void);
bytes4 get_miliseconds(void);
void delayusg(unsigned long useg);
void runAfterUsg(void (*f)(void), bytes4 useg);
void runEveryUsg(void (*f)(void), bytes4 useg);

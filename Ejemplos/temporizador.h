//extern unsigned long nDesbordamientos; 
//no hace falta acceder a esta variable 
//fuera de la libreria, solo se usa para
//obtener el tiempo en useg, mseg, etc.


void __attribute__((interrupt)) vi_tov(void);
void init_temporizador(void);
unsigned long get_microseconds(void);
unsigned long get_miliseconds(void);
void delayusg(unsigned long useg) ;

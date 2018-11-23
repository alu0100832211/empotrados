//extern unsigned long nDesbordamientos; 
//no hace falta acceder a esta variable 
//fuera de la libreria, solo se usa para
//obtener el tiempo en useg, mseg, etc.


void __attribute__((interrupt)) vi_tov(void);
void init_temporizador(void);
4bytes get_microseconds(void);
4bytes get_miliseconds(void);
void delayusg(unsigned long useg) ;

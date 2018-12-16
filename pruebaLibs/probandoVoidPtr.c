#include <stdio.h>

void call(void (*f)(void*), void* args){
  f(args);
}
void function(void * ptr){
  static int i = 0;
  
  i++;

  printf("%i\n", i);
}
int main(void){
  for (int i = 0; i< 10; i++){
    call(function, 0);
  }

}

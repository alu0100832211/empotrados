#include <stdio.h>

void call(void (*f)(void*), void* args){
  f(args);
}
void function(void * ptr){
  static int i = 0;
  
  i++;

  printf("%i\n", i);
}

char ** funciton2(int n){
  static char array[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'#', '0', '*'},
  };
  return &array;

}
int main(void){
  for (int i = 0; i< 10; i++){
    call(function, 0);
  }

  char ** array =  funciton2(0);

  for(int i = 0; i < 4; i++)
    for (int j = 0; j < 3; j++)
      printf("%c ", array[i][j]);
  printf("\n");

}

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int pinBMas = 8; 
int pinBMenos = 7;
int pwmPin = 6;
int tPin = A0;
float t;
float tInput = 20;

void setup(){
  Serial.begin(9600);
  lcd.begin(8,2);
  
  pinMode(pinBMenos, INPUT);
  digitalWrite(pinBMenos, HIGH);
  pinMode(pinBMas, INPUT);
  digitalWrite(pinBMas, HIGH);
  //boton pulsado = LOW
  //boton sin pulsar = HIGH
}

//void serialEvent(){
  //tInput = Serial.parseFloat();
//  if (tInputTmp = Serial.parseFloat()){
//    tInput = tInputTmp;
//  }
//}
void loop(){
  t = get_temp();
  //Poner velocidad del ventilador 
//  int fanSpeed = map_float(t, tInput, 50, 255, 0);
//  fanSpeed = constrain(fanSpeed, 0, 255);
  int incrementar = digitalRead(pinBMas);
  if(incrementar == LOW){
    Serial.println("incrementar");
    tInput+=0.5;
  }
  int decrementar = digitalRead(pinBMenos);
  if(decrementar == LOW){
    Serial.println("decrementar");
    tInput-=0.5;
  }
  
  float error = t - tInput;
  int fanSpeed = 50*error;
  fanSpeed = constrain(fanSpeed, 0, 255);
  fanSpeed = map(fanSpeed, 0, 255, 255, 0);
  
//  Serial.print(fanSpeed);Serial.print(" ");

  analogWrite(pwmPin, fanSpeed);
  

  lcd.setCursor(0,0);
  lcd.print(t);lcd.print("C ");
  lcd.setCursor(0,1);
  lcd.print(tInput);lcd.print("C ");
//  lcd.print(fanSpeed);lcd.print("V");
  lcd.display();  
  delay(200);
  lcd.noDisplay();
  
//  Serial.print(t);Serial.print("C ");
//  Serial.print(map(fanSpeed, 255, 0, 0, 100));Serial.print("% ");
//  Serial.print(tInput);Serial.print("C temperatura deseada\n");
}
void stopControl(){
  analogWrite(A0, 255);
}

void adjustTemp(){
  //Si se aprieta boton+
  //tInput += 0.1
  //Si esta apretado boton -
  //tInput += 0.1
}
//map con decimales https://www.arduino.cc/reference/en/language/functions/math/map/
long map_float(float x, long in_min, long in_max,  long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float get_temp (void){
  return (analogRead(tPin) * 0.046996015);
}

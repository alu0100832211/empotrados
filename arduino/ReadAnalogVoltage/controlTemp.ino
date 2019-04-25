void setup(){
  Serial.begin(9600);
}

int pwmPin = 1;
float t;
//int tInput;
void serialEvent(){

}
void loop(){
  Serial.println("loop");
  if (float tInput = Serial.parseFloat()){
    //if (tInput == 0 ) return;
    while(true){
      t = get_temp();
      //Poner velocidad del ventilador 
      int fanSpeed = map_float(t, tInput-5, 50, 255, 0);
      fanSpeed = constrain(fanSpeed, 0, 255);
      analogWrite(pwmPin, fanSpeed);
      Serial.print(t);Serial.print("ºC ");
      Serial.print(map(fanSpeed, 255, 0, 0, 100));Serial.print("%\n");
    }
  }
}

//map con decimales https://www.arduino.cc/reference/en/language/functions/math/map/
long map_float(float x, long in_min, long in_max,  long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float get_temp (void){
  //int sensorValue = analogRead(A0);
  //float voltage = sensorValue * (5.0 / 1023.0);
  //float temp = (voltage / 10.4) * 100;
  return (analogRead(A0) * 0.046996015);
}

#include "dht.h"
#define dataPin A0

dht DHT;

int temp;
int hum;

void setup() {
 
 Serial.begin(9600); 
 
 Serial.println("Ready");
}

void loop(){
  int readData = DHT.read11(dataPin);

  hum = DHT.humidity;
  temp = DHT.temperature;

Serial.print(temp); //send distance to MIT App
Serial.print(";");
Serial.print(hum); //send distance to MIT App 
Serial.println(";");
  
  delay(1000);
}
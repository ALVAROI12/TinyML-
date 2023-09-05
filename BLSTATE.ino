#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// Connect the HC-05 STATE pin to Arduino pin 4.
//
 
char c = ' ';
 
// BTconnected will = false when not connected and true when connected
boolean BTconnected = false;
 
// connect the STATE pin to Arduino pin D4
const byte BTpin = 4;
 
 
void setup() 
{
    // set the BTpin for input
    pinMode(BTpin, INPUT);   
 
    // start serial communication with the serial monitor on the host computer
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Connect the HC-05 to an Android device to continue");
 
    // wait until the HC-05 has made a connection
    
 
    // Start serial communication with the bluetooth module
    // HC-02 default serial speed for communication mode is 9600 but can be different
    BTserial.begin(9600);  
}
 
void loop()
{

      if ( digitalRead(BTpin)==HIGH)  { BTconnected = true; Serial.println("HC-02 is connected");
    Serial.println("");
    delay(2000);
      }
    else {
Serial.println("HC-02 is disconnected");
Serial.println("");
delay(2000);}
    

}
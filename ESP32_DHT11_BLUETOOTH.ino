/*********
  all rights to Alvaro Ibarra
*********/

// Load libraries
#include "BluetoothSerial.h"
#include "DHT.h"
// Define PIN and Type of sensor
#define DHTPIN 13
#define DHTTYPE DHT11
// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// declare variables for Temperature and Humidity
int temp;
int hum;

// Bluetooth Serial object
BluetoothSerial SerialBT;
//Create DHT as an Object
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

void setup() {
   
  Serial.begin(115200);
  dht.begin();
  // Bluetooth device name
SerialBT.begin("ESP32");
Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {

float h = dht.readHumidity();
float t = dht.readTemperature();

  
  SerialBT.println(t); 
  SerialBT.println(";");
  SerialBT.println(h); 
  SerialBT.println(";");

  
  delay(1000);
}
// import libraries
#include "ThingSpeak.h"
#include "WiFi.h"
#include "DHT.h"
#include "BluetoothSerial.h"

// Define PIN and Type of sensor
#define pin1 13       //DHT11 PIN.

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


const char* ssid = "uiw-resnet";                        //SSID de vuestro router.
const char* password = "";                //Contraseña de vuestro router.

unsigned long ID = 2050217;                //ID de vuestro canal.
const char* APIKey = "H62ZCHN3C0703GUT";     //Write API Key de vuestro canal.

WiFiClient cliente;
BluetoothSerial SerialBT;

DHT dht1(pin1, DHT11);    //DHT11 BLUE


void setup() {

  // start serial monitor
  Serial.begin(115200);
  Serial.println("sensors test:");
  SerialBT.begin("ESP32");
  Serial.println("BLE ON");

  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conect!");

  ThingSpeak.begin(cliente);

dht1.begin();
}

void loop() {

  delay(2000);
  leerdht1();

  ThingSpeak.writeFields(ID,APIKey);
  Serial.println("Data sent");
  delay(14000);
}

void leerdht1() {
  
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();

  while (isnan(t1) || isnan(h1)){
    Serial.println("Failed to read");
    delay(2000);
    t1 = dht1.readTemperature();
    h1 = dht1.readHumidity();
  }

  Serial.print("Temp: ");
  Serial.print(t1);
  Serial.println(" ºC.");

  Serial.print("Hum: ");
  Serial.print(h1);
  Serial.println(" %."); 

  Serial.println("-----------------------");

// Bluetooth data send
  SerialBT.println(t1); 
  SerialBT.println(";");
  SerialBT.println(h1); 
  SerialBT.println(";");

  // WIFI DATA SEND
  ThingSpeak.setField (1,t1);
  ThingSpeak.setField (2,h1);
}

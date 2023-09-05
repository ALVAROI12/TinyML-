#include "ThingSpeak.h"
#include "WiFi.h"

#include "DHT.h"



#define pin1 13       //DHT11 PIN.


const char* ssid = "uiw-resnet";                        //SSID de vuestro router.
const char* password = "";                //Contraseña de vuestro router.

unsigned long channelID = 2050217;                //ID de vuestro canal.
const char* WriteAPIKey = "H62ZCHN3C0703GUT";     //Write API Key de vuestro canal.

WiFiClient cliente;

DHT dht1(pin1, DHT11);    //El azul.


void setup() {
  Serial.begin(115200);
  Serial.println("sensors test:");

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

  ThingSpeak.writeFields(channelID,WriteAPIKey);
  Serial.println("Data sent to ThingSpeak!");
  delay(14000);
}

void leerdht1() {
  
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();

  while (isnan(t1) || isnan(h1)){
    Serial.println("Failed to read sensor/ reading again...");
    delay(2000);
    t1 = dht1.readTemperature();
    h1 = dht1.readHumidity();
  }

  Serial.print("Temperature DHT11: ");
  Serial.print(t1);
  Serial.println(" ºC.");

  Serial.print("Humidity DHT11: ");
  Serial.print(h1);
  Serial.println(" %."); 

  Serial.println("-----------------------");

  ThingSpeak.setField (1,t1);
  ThingSpeak.setField (2,h1);
}


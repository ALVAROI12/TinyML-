#include <Arduino_HS300x.h>
#include <SoftwareSerial.h> 
                                  

String agAdi = "uiw-resnet";                 // We write the name of our network here.    
String agSifresi = "";           // We write the password of our network here.

int rxPin = 3;                                               //ESP8266 RX pini
int txPin = 2;                                               //ESP8266 TX pini

String ip = "184.106.153.149";                                //Thingspeak ip adresi
float sicaklik, nem;

SoftwareSerial esp(rxPin, txPin);                             // We make serial communication pin settings.

void setup() {  
  while (!Serial);

  if (!HS300x.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);

  Serial.begin(9600);  // We are starting our communication with the serial port.
  Serial.println("Started");
  esp.begin(115200);                                          // We are starting serial communication with ESP8266.
  esp.println("AT");                                         // We do the module control with the AT command.
  Serial.println("AT  sent ");
  while(!esp.find("OK")){                                     // We wait until the module is ready.
    esp.println("AT");
    Serial.println("ESP8266 Not Find.");
  }
  Serial.println("OK Command Received");
  esp.println("AT+CWMODE=1");                                 // We set the ESP8266 module as a client.
  while(!esp.find("OK")){                                     // We wait until the setting is done.
    esp.println("AT+CWMODE=1");
    Serial.println("Setting is ....");
  }
  Serial.println("Set as client");
  Serial.println("Connecting to the Network ...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    // We are connecting to our network.
  while(!esp.find("OK"));                                     // We wait until it is connected to the network.
  Serial.println("connected to the network.");
  delay(1000);
}
}
void loop() {
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           // We connect to Thingspeak.
  if(esp.find("Error")){                                      // We check the connection error.
    Serial.println("AT+CIPSTART Error");
  }
  float temperature = HS300x.readTemperature(FAHRENHEIT);
  float humidity    = HS300x.readHumidity();
  String veri = "GET https://api.thingspeak.com/update?api_key=H62ZCHN3C0703GUT";   // Thingspeak command. We write our own api key in the key part.                                   
  veri += "&field1=";
  veri += String(temperature);// The temperature variable we will send
  veri += "&field2=";
  veri += String(humidity);// The moisture variable we will send
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   // We give the length of data that we will send to ESP.
  esp.println(veri.length()+2);
  delay(2000);
  if(esp.find(">")){                                          // The commands in it are running when ESP8266 is ready..
    esp.print(veri);                                          // We send the data.
    Serial.println(veri);
    Serial.println("Data sent.");
    delay(1000);
  }
                                                
}
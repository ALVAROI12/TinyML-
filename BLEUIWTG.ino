#include <ArduinoBLE.h>
#include <Arduino_HS300x.h>
#include "Arduino_BMI270_BMM150.h"

String h, t, m;

BLEService customService("180C");

BLEStringCharacteristic ble_temperature("2A57", BLERead | BLENotify, 13);
BLEStringCharacteristic ble_humidity("2A59", BLERead | BLENotify, 13);
BLEStringCharacteristic ble_magnetic("2A58", BLERead | BLENotify, 20);


void readValues();

void setup(){

   IMU.begin();
   HS300x.begin();
   Serial.begin(9600);
    while (!Serial);
    if (!BLE.begin())

    {
        Serial.println("BLE failed to Initiate");
        delay(500);
        while (1);
    }
// Setting BLE Name
    BLE.setLocalName("UIW BLE");
    
    // Setting BLE Service Advertisment
    BLE.setAdvertisedService(customService);
    
    // Adding characteristics to BLE Service Advertisment
    customService.addCharacteristic(ble_humidity);
    customService.addCharacteristic(ble_magnetic);
    customService.addCharacteristic(ble_temperature);
    // Adding the service to the BLE stack
    BLE.addService(customService);

    // Start advertising
    BLE.advertise();
    Serial.println("Bluetooth device is now active, waiting for connections...");
}

void loop()
{
    // Variable to check if cetral device is connected
    BLEDevice central = BLE.central();
    if (central)
    {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        while (central.connected())
        {
            delay(200);
            
            // Read values from sensors
            readValues();

            // Writing sensor values to the characteristic
      
            ble_humidity.writeValue(h);
            ble_temperature.writeValue(t);
            ble_magnetic.writeValue(m);

            // Displaying the sensor values on the Serial Monitor
            Serial.println("Reading Sensors");
          
            Serial.println(h);
            Serial.println(t);
            Serial.println(m);
            Serial.println("\n");
            delay(1000);
        }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
}

void readValues()
{
    // Reading raw sensor values from three sensors
    float x, y, z;
  float humidity    = HS300x.readHumidity();  
  float temperature = HS300x.readTemperature();
    if (IMU.magneticFieldAvailable()) {
      IMU.readMagneticField(x, y, z);

    // Saving sensor values into a user presentable way with units
    h = String(humidity) + "%";
    t = String(temperature) + " C";
    m = "X:" + String(x) + ", Y:" + String(y);
    }
}






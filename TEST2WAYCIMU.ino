#include <ArduinoBLE.h>

// Define the UUIDs for the ESP32 BLE service and characteristics
#define SERVICE_UUID        "1234"
#define SENSOR_DATA_UUID    "5678"

// Define the Bluetooth characteristics
BLEService sensorService(SERVICE_UUID);
BLECharacteristic sensorDataCharacteristic(SENSOR_DATA_UUID, BLENotify);

void setup() {
  Serial.begin(9600);

  // Initialize the BLE library
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }

  // Scan for the ESP32 BLE peripheral device
  BLE.scanForUuid(SERVICE_UUID);

  // Connect to the ESP32
  BLEDevice peripheral = BLE.available();
  if (!peripheral) {
    Serial.println("Failed to connect to ESP32");
    while (1);
  }

  // Discover the ESP32 BLE service and characteristics
  BLEClient client = peripheral.connect();
  if (!client.discoverService(SERVICE_UUID)) {
    Serial.println("Failed to discover ESP32 BLE service");
    while (1);
  }
  BLECharacteristic sensorDataCharacteristic = client.characteristic(SENSOR_DATA_UUID);
  if (!sensorDataCharacteristic) {
    Serial.println("Failed to discover ESP32 BLE characteristic");
    while (1);
  }

  // Subscribe to the ESP32 BLE characteristic notifications
  sensorDataCharacteristic.subscribe();

  // Start the BLE service
  BLE.setLocalName("Arduino BLE Sense 33");
  BLE.setAdvertisedService(sensorService);
  sensorService.addCharacteristic(sensorDataCharacteristic);
  BLE.addService(sensorService);
  BLE.advertise();
}

void loop() {
  // Read the sensor data from the ESP32 BLE characteristic
  if (sensorDataCharacteristic.valueUpdated()) {
    String sensorData = sensorDataCharacteristic.readValue();
    Serial.println(sensorData);
  }
}

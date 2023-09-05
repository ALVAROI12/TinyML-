//ESP32 TWO WAY COMUNICATION WITH ARDUINO BLE 
//AND THINKSPEAK INTEGRATION
//CREDITS TO
//Alvaro Ibarra & Arduino & cattledog 



#include "ThingSpeak.h"
#include "WiFi.h"
#include "BLEDevice.h"
#include "DHT.h"

#define DHTPIN 12
#define DHTTYPE DHT11

const char* ssid = "uiw-resnet";  
const char* password = "";        

unsigned long ID = 2050217;  
const char* APIKey = "H62ZCHN3C0703GUT";

WiFiClient cliente;

int rele = 13;

DHT dht(DHTPIN, DHTTYPE);

float t1;
float ble_data;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("180F");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("2A19");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)
{

  Serial.println(" ");
  Serial.println("Temperature: ");
  Serial.println((char*)pData);
  ThingSpeak.setField(1, (char*)pData);
  ble_data = atof((char*)pData);
}

void read()
{
  t1 = dht.readTemperature();
  Serial.println("Temperature2: ");
  Serial.print(t1);
  Serial.println(" ");
  ThingSpeak.setField(2, t1);
}


void compare()
{
 
 Serial.print("Test t1");
 Serial.println(" ");
 Serial.print(t1);
 Serial.println(" ");
 Serial.print("Test ble_data");
 Serial.println(" ");
 Serial.print(ble_data);
 Serial.println(" ");

  if (ble_data > t1)
  {
    digitalWrite(rele, HIGH);
 }
  else
  {
    digitalWrite(rele, LOW);
  }
}

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient* pclient)
  {
  }

  void onDisconnect(BLEClient* pclient)
  {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer()
{
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient* pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }

  if (pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  connected = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
    {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    }  // Found our server
  }    // onResult
};     // MyAdvertisedDeviceCallbacks


void setup()
{
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  dht.begin();
  //////////////////////////////////////////////////////////////////////
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conect!");

  ThingSpeak.begin(cliente);

  pinMode(rele, OUTPUT);


  /////////////////////////////////////////////////////////////////////////////////////

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  ////////////////////////////////////////////////////////////////////////////////////
}


// This is the Arduino main loop function.
void loop()
{

  read();
  compare();
  delay(1000);

  ThingSpeak.writeFields(ID, APIKey);
  Serial.println("Data sent");
  Serial.println(" ");
  

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true)
  {
    if (connectToServer())
    {
      Serial.println("We are now connected to the BLE Server.");
    }
    else
    {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected)
  {
    String newValue = "Time since boot: " + String(millis() / 1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  }
  else if (doScan)
  {
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }

  delay(1000);  // Delay a second between loops.
}  // End of loop
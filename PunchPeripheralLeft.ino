#include <Arduino.h>
#include <ArduinoBLE.h>

const char* uuidOfTxService = "00002A3D-0000-1000-8000-00805f9b34fb";
const char* uuidOfTxChar = "00012A3D-0000-1000-8000-00805f9b34fb";

//const char* uuidOfTxService = "aece76e9-3ebd-493a-a8ab-f7e07af1f03f";
//const char* uuidOfTxChar = "6d8aab39-256a-4443-b61e-7d7b96f7b3df";


const char* nameOfPeripheral = "LeftPeripheral";
const char* testData = "012345678911234567892123456789312345678941234567895";
const char* testData2 = "2.01, 2.02, 2.03, 300, 400, 500";
const int TX_BUFFER_SIZE = 256;
bool TX_BUFFER_FIXED_LENGTH = false;

const int sendInterval = 1000;

BLEService sensorService(uuidOfTxService); 
BLECharacteristic sensorCharacteristic(uuidOfTxChar,BLENotify, TX_BUFFER_SIZE, TX_BUFFER_FIXED_LENGTH);

void setup() {
  Serial.begin(115200);
  while (!Serial);  
  delay(100);
  initBLE();
  configureBLE();

}


void loop() {

  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
      writeSensorData();
    }
    Serial.println("* Disconnected to central device!");
  }
}

void initBLE(void){
  if (!BLE.begin()) {
    Serial.println("Starting BLE module failed!");
    while (1);
  }

}


void configureBLE(void){
  BLE.setLocalName(nameOfPeripheral);
  BLE.setAdvertisedService(sensorService);
  sensorService.addCharacteristic(sensorCharacteristic);
  BLE.addService(sensorService);
  sensorCharacteristic.writeValue("apa");
  BLE.advertise();
  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}


void writeSensorData(void) {
  delay(sendInterval);
  sensorCharacteristic.writeValue(testData);
  delay(sendInterval);
  sensorCharacteristic.writeValue(testData2);
}
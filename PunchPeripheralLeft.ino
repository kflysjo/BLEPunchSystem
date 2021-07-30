#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* uuidOfTxServiceLH = "00002A3D-0000-1000-8000-00805f9b34fb";
const char* uuidOfTxCharacteristicLH = "00012A3D-0000-1000-8000-00805f9b34fb";
const char* nameOfPeripheral = "LeftPeripheral";

char testData[256];
String dataPayload = "";
char catBuffer[64];
const int TX_BUFFER_SIZE = 256;
bool TX_BUFFER_FIXED_LENGTH = false;

const int sendInterval = 1000;

BLEService sensorService(uuidOfTxServiceLH); 
BLECharacteristic sensorCharacteristic(uuidOfTxCharacteristicLH,BLENotify, TX_BUFFER_SIZE, TX_BUFFER_FIXED_LENGTH);

void initBLE(void);
void configureBLE(void);
void writeSensorData(void);


void setup() {
  Serial.begin(115200);
  while (!Serial);  
  delay(1000);
  initBLE();
  configureBLE();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
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
float accX, accY, accZ;
float gyroX, gyroY, gyroZ;

  while (!IMU.accelerationAvailable() && !IMU.gyroscopeAvailable())  
  {
    ;// Future error protection?
  }
  strcpy(testData, "");
  IMU.readAcceleration(accX, accY, accZ);
  IMU.readGyroscope(gyroX, gyroY, gyroZ);

  sprintf(catBuffer, "%.2f, ", accX);
  strcat(testData, catBuffer);
  sprintf(catBuffer, "%.2f, ", accY);
  strcat(testData, catBuffer);
  sprintf(catBuffer, "%.2f, ", accZ);
  strcat(testData, catBuffer);
  sprintf(catBuffer, "%.2f, ", gyroX);
  strcat(testData, catBuffer);
  sprintf(catBuffer, "%.2f, ", gyroY);
  strcat(testData, catBuffer);
  sprintf(catBuffer, "%.2f\n", gyroZ);
  strcat(testData, catBuffer);

  Serial.print(testData);

  sensorCharacteristic.writeValue(testData);
  delay(sendInterval);



}

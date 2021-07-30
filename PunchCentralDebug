#include <Arduino.h>
#include <ArduinoBLE.h>

const char* uuidOfTxService = "00002A3D-0000-1000-8000-00805f9b34fb";
const char* uuidOfTxChar = "00012A3D-0000-1000-8000-00805f9b34fb";




const int TX_BUFFER_SIZE = 512;
bool TX_BUFFER_FIXED_LENGTH = false;

BLEService sensorService(uuidOfTxService); 
BLECharacteristic sensorCharacteristic(uuidOfTxChar,BLENotify, TX_BUFFER_SIZE, TX_BUFFER_FIXED_LENGTH);

char RX_BUFFER[TX_BUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("* Starting BLE module failed!");
    while (1);

  }

  BLE.setLocalName("PunchCentral"); 
  BLE.advertise();
  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
}

void loop() {
  connectToPeripheral();
}

void connectToPeripheral(){
  BLEDevice peripheral;

  Serial.println("- Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(uuidOfTxService);
    peripheral = BLE.available();
  } while (!peripheral);

  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }

}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");
  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } 
    else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }
  Serial.println("- Discovering peripheral device attributes...");


  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");

  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;

  }

  BLECharacteristic simpleKeyCharacteristic = peripheral.characteristic(uuidOfTxChar);

  // subscribe to the simple key characteristic
  Serial.println("Subscribing to simple key characteristic ...");
  if (!simpleKeyCharacteristic) {
    Serial.println("no simple key characteristic found!");
    peripheral.disconnect();
    return;
  } 
  else if (!simpleKeyCharacteristic.canSubscribe()) {
    Serial.println("simple key characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } 
  else if (!simpleKeyCharacteristic.subscribe()) {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  }
 

  while (peripheral.connected()) {
    if (simpleKeyCharacteristic.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      simpleKeyCharacteristic.readValue(RX_BUFFER, TX_BUFFER_SIZE);
      Serial.println(RX_BUFFER);
      delay(5);

  }
  
}
}

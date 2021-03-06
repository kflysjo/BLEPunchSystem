#include <Arduino.h>
#include <ArduinoBLE.h>

const char* uuidOfRxServiceLH = "00002A3D-0000-1000-8000-00805f9b34fb";
const char* uuidOfRxCharacteristicLH = "00012A3D-0000-1000-8000-00805f9b34fb";

const int RX_BUFFER_SIZE = 200;
char RX_BUFFER[RX_BUFFER_SIZE];
bool RX_BUFFER_FIXED_LENGTH = true;
int succesful_rx_counter = 0;

void connectToPeripheral();
void controlPeripheral(BLEDevice peripheralLH);

BLEService sensorService(uuidOfRxServiceLH); 
BLECharacteristic sensorCharacteristic(uuidOfRxCharacteristicLH,BLENotify, RX_BUFFER_SIZE, RX_BUFFER_FIXED_LENGTH);


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
  BLEDevice peripheralLH;

  Serial.println("- Discovering peripheralLH device...");

  do
  {
    BLE.scanForUuid(uuidOfRxServiceLH);
    peripheralLH = BLE.available();
  } while (!peripheralLH);

  if (peripheralLH) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheralLH.address());
    Serial.print("* Device name: ");
    Serial.println(peripheralLH.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheralLH.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheralLH);
  }

}

void controlPeripheral(BLEDevice peripheralLH) {
  Serial.println("- Connecting to peripheralLH device...");
  if (peripheralLH.connect()) {
    Serial.println("* Connected to peripheralLH device!");
    Serial.println(" ");
  } 
    else {
    Serial.println("* Connection to peripheralLH device failed!");
    Serial.println(" ");
    return;
  }
  Serial.println("- Discovering peripheralLH device attributes...");


  if (peripheralLH.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");

  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheralLH.disconnect();
    return;

  }

  BLECharacteristic simpleKeyCharacteristicLH = peripheralLH.characteristic(uuidOfRxCharacteristicLH);

  Serial.println("Subscribing to simple key characteristic ...");
  if (!simpleKeyCharacteristicLH) {
    Serial.println("no simple key characteristic found!");
    peripheralLH.disconnect();
    return;
  } 
  else if (!simpleKeyCharacteristicLH.canSubscribe()) {
    Serial.println("simple key characteristic is not subscribable!");
    peripheralLH.disconnect();
    return;
  } 
  else if (!simpleKeyCharacteristicLH.subscribe()) {
    Serial.println("subscription failed!");
    peripheralLH.disconnect();
    return;
  }
 

  while (peripheralLH.connected()) {
    if (simpleKeyCharacteristicLH.valueUpdated()) {
      simpleKeyCharacteristicLH.readValue(RX_BUFFER, RX_BUFFER_SIZE);
      succesful_rx_counter++;
      delayMicroseconds(50);

    if (succesful_rx_counter % 1000 == 0)
    {
      Serial.println(millis());
      Serial.println(succesful_rx_counter);
    }
    

  }
  
}
}

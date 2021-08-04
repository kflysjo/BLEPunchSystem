#include <Arduino.h>
#include <ArduinoBLE.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>


File myFile;
long random_number;
char filename_buffer[16];
char rng_buffer[8];
char individual_log_appendage[] = ".csv";

const char* uuidOfRxServiceLH = "00002A3D-0000-1000-8000-00805f9b34fb";
const char* uuidOfRxCharacteristicLH = "00012A3D-0000-1000-8000-00805f9b34fb";

const int RX_BUFFER_SIZE = 200;
const int RX_TIMEOUT = 200;
char RX_BUFFER[RX_BUFFER_SIZE];
bool RX_BUFFER_FIXED_LENGTH = true;
int succesful_rx_counter = 0;
long int current_millis, last_succesful_rx_millis;

void connectToPeripheral();
void controlPeripheral(BLEDevice peripheralLH);

BLEService sensorService(uuidOfRxServiceLH); 
BLECharacteristic sensorCharacteristic(uuidOfRxCharacteristicLH,BLENotify, RX_BUFFER_SIZE, RX_BUFFER_FIXED_LENGTH);


void setup() {
  Serial.begin(115200);
  while (!Serial);

  randomSeed(analogRead(A0));


  Serial.print("Initializing SD card...");
  pinMode(9, OUTPUT);
  delay(1000);

  while (!SD.begin(9)) 
  {
    Serial.println("SD-card initialization failed!");
    delay(1000);
  }
  Serial.println("SD-card initialization done.");

    for (int i = 0; i < 3; i++)
  {

    random_number = random(10,100);
    Serial.print("random number: ");
    Serial.println(random_number);

    sprintf(rng_buffer, "%ld", random_number);
    Serial.print("strlen of rng buffer: ");
    Serial.println(strlen(rng_buffer));

    strcat(filename_buffer, rng_buffer);
    Serial.print("strlen of filenamebuffer: ");
    Serial.println(strlen(filename_buffer));

  }

  strcat(filename_buffer, individual_log_appendage);

  Serial.print("Full filename: ");
  Serial.println(filename_buffer);

  Serial.print("strlen of filename: ");
  Serial.println(strlen(filename_buffer));


  
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
 
  
  delay(100);
  myFile = SD.open(filename_buffer, FILE_WRITE);
  delay(100);
  myFile.println("SD-card object has been created");

  myFile.println("File is open, glhf");

  last_succesful_rx_millis = millis();
  current_millis = millis();
  
  while (peripheralLH.connected()/* && (current_millis - last_succesful_rx_millis) > RX_TIMEOUT*/) {
    current_millis = millis();
    if (simpleKeyCharacteristicLH.valueUpdated()) {
      
      last_succesful_rx_millis = millis();
      simpleKeyCharacteristicLH.readValue(RX_BUFFER, RX_BUFFER_SIZE);
      delayMicroseconds(500);
      
      //myFile.print(millis());
      //myFile.print(", ");
      //myFile.print(RX_BUFFER);
      
      myFile.println("apa");
      myFile.flush();
      
      delayMicroseconds(500);
      //Serial.println(RX_BUFFER);
  }

  
    if((current_millis - last_succesful_rx_millis) > RX_TIMEOUT){
      Serial.println("RX timeout");      
      break;
    }
  
  }

  Serial.println("Disconnected from LH");
  myFile.close();

}

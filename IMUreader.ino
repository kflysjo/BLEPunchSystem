#include <Arduino.h>
/*
  Arduino LSM9DS1 - Simple Accelerometer
  This example reads the acceleration values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.
  The circuit:
  - Arduino Nano 33 BLE Sense
  created 10 Jul 2019
  by Riccardo Rizzo
  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

float accx, accy, accz;
float gyrox, gyroy, gyroz;
long int millis_at_start = 0, millis_now = 0; 
int i, count = 1000, combobreaker = 1, millis_diff;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

}

void loop() {

  while (combobreaker==1)
  {
    
    millis_at_start = millis();

    for (i = 0; i < count;) 
    {
      while (!IMU.accelerationAvailable())
      {
        delayMicroseconds(1);
      }
      
        IMU.readAcceleration(accx, accy, accz);

        i++;

  }

    millis_now = millis();
    millis_diff = millis_now - millis_at_start;  
    
    
    
    Serial.print("Start time: ");
    Serial.println(millis_at_start);
    Serial.print("Stop time: ");
    Serial.println(millis_now);
    Serial.print("Diff time: ");
    Serial.println(millis_diff);
    


  }
  

  
  
    combobreaker = 0;




}

void serialPrintData(void)
{
  Serial.print(accx);
  Serial.print(", ");
  Serial.print(accy);
  Serial.print(", ");
  Serial.print(accz);
  Serial.print(", ");
  Serial.print(gyrox);
  Serial.print(", ");
  Serial.print(gyroy);
  Serial.print(", ");
  Serial.println(gyroz);
}

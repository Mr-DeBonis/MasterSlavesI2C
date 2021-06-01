// Master
// by Ignacio De Bonis
//
// Reads data from two I2C/TWI slave devices
// Devices send potentiometer values (int), humidity and temperature (float)
// Reads timestamp fron DS3231 module, which uses I2C communication.

// Created 25 May 2021

#include <Wire.h>

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// DS3231 slave address: 0x68
#include "RTClib.h"

RTC_DS1307 rtc;

int slave_address[2] = {8, 9};

// Unions allows to access each byte of the float (or any other type) variable.

typedef union {
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t hum, temp;
int pot;

const int requestbytes = 2 * sizeof(temp.number) + sizeof(pot);

void printValue(int temp)
{
  if (temp < 10) {
    Serial.print("0");
  }
  Serial.print(temp);
}

void setup() {
  Wire.begin();        // join i2c
  Serial.begin(9600);

  // rtc module
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // Added TimeSpan(0,3,0,0) to adjust to UTC time, since the timezone is GMT-4
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(0, 4, 0, 0));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

}

void loop() {
  DateTime now = rtc.now();
  printTime(now);

  //////////////////////////////////////////////////
  requestData(slave_address[0]);
  requestData(slave_address[1]);
  Serial.println();
  //////////////////////////////////////////////////

  delay(1000);
}

void printData(int pot, float temp, float hum, int slave_address) {
  // Show values in screen
  Serial.print("Esclavo ");
  Serial.print(slave_address);
  Serial.print(": Potenciometro: ");
  Serial.print(pot);
  Serial.print("\tTemperatura: ");
  Serial.print(temp);
  Serial.print(" *C Humedad: ");
  Serial.print(hum);
  Serial.println(" %\t");

}

void requestData(int slaveAddress) {
  // Request potentiometer, temperature and humidity values from slave

  Wire.requestFrom(slaveAddress, requestbytes);

  while (Wire.available()) { // slave may send less than requested
    pot = (Wire.read() | Wire.read() << 8);  // read two bytes and merge them into an int
    for (int i = 0; i < 4; i++) {
      temp.bytes[i] = Wire.read();          // read 4 bytes and merge them into a float
    }
    for (int i = 0; i < 4; i++) {
      hum.bytes[i] = Wire.read();           // Same.
    }
  }

  // Show values in screen
  printData(pot, temp.number, hum.number, slaveAddress);
}


void printTime(DateTime now) {
  // Print timestamp
  // Unixtime:
  Serial.println(now.unixtime());

  // Date and time in UTC
  printValue(now.day());
  Serial.print('/');
  printValue(now.month());
  Serial.print('/');
  printValue(now.year());

  Serial.print(", ");

  printValue(now.hour());
  Serial.print(':');
  printValue(now.minute());
  Serial.print(':');
  printValue(now.second());
  Serial.println();
}

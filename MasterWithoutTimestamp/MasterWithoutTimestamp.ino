// Master
// by Ignacio De Bonis
//
// Reads data from two I2C/TWI slave devices
// Devices send potentiometer values (int), humidity and temperature (float)

// Created 31 May 2021

#include <Wire.h>

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
}

void loop() {
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

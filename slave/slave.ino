// Slave
// by Ignacio De Bonis
//
// It reads data from DHT11 and potentiometer, and sends to master on request.
// It sends potentiometer values (int), humidity and temperature (float)

// Created 9 May 2021

// Slave adresses 7 and under are reserved.
// I2C protocol can support up to 127 devices.
#define SLAVE_ADDRESS 8

#include <Wire.h>
#include <DHT.h>

// Define the digital pin that reads the sensor
#define DHTPIN 7
// Define the sensor type
#define DHTTYPE DHT11

// Define DHT11
DHT dht(DHTPIN, DHTTYPE);

// Define Pot value
int sensorValue;

typedef union {
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t hum, temp;

void setup() {
  // Initialize variables
  hum.number = 0;
  temp.number = 0;

  // Initialize DHT11
  dht.begin();
  Wire.begin(SLAVE_ADDRESS);    // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);

}

void loop() {
  sensorValue = analogRead(A0);

  // Read relative humidity
  hum.number = dht.readHumidity();
  // Read temperature in Celsius
  temp.number = dht.readTemperature();

  Serial.println(hum.number);
  Serial.println(temp.number);
  delay(1000);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  // Send Analog values
  Wire.write(sensorValue); // send the lower 8 bits
  Wire.write((sensorValue >> 8)); // send the upper 8 bits

  // Send DHT values
  for (int i = 0; i < 4; i++) {
    Wire.write(temp.bytes[i]);
  }
  for (int i = 0; i < 4; i++) {
    Wire.write(hum.bytes[i]);
  }
  // as expected by master
}

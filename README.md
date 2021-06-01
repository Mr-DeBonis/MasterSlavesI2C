# Master Slave

Control several devices through I2C.

Upload code to slaves with *slave.ino*. Make sure to change the address (SLAVE_ADDRESS). The slave will read values from 2 sensors: Temperature, humidity, voltage and send them to master every second.

To upload code to master, there are 2 options:
* **Without timestamp:** Use the *MasterWithoutTimestamp.ino* file to upload to master. DS3231 can be skipped.
* **With timestamp:** Use the *MasterTimestamp.ino* file to upload to master. Add timestamp to each measurement.

Both codes request data from slaves each second.

The breadboard  planes and schematics are made with [Fritzing](https://fritzing.org/).

## Components:

* 3 Arduinos (UNO or Nano)
* 2 DHT11 (1 for each slave)
* 2 Potentiometers (1 for each slave)
* 1 DS3231 (RTC module)

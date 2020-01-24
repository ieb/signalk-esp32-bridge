# Signalk Bluetooth LE Sensor Bridge

This project contains the code for a SignalK to BLE Bridge which queries sensors advertising over BLE. It then queries those sensors periodically and injectes the data into the SignalK server.

Any BLE device would do, however the code is targetting ESP32 chips which have built in BLE and will
run on batteries if required. The chips are available on development boards on eBay for 5 GBP. The chips have i2c, SDI and 1 wire interfaces as well as having 8 channels of 12bit ADC converters that will operate at high speed.  The code for the sensors is under the sensors folder. It uses the Ardiono IDE to program the ESP32 devices.

Applications include
* Engine room termperature and voltage sensing.
* Battery sensing
* Pressure temperature humidity
* Strain guage/load cell.

Other instruments may be possible.

Sensors include
* BMP280 and BME280 environment sensors
* 1Wire DS18B20 temperature probes
* Voltage measurements 12bit 0-3.3v



# notes

    rsync -v -r --exclude node_modules --delete -e ssh * 192.168.1.188:signalk-ble-bridge 


    rsync -v -r --exclude node_modules --delete -e ssh 192.168.1.188:signalk-ble-bridge .

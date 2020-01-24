# Sensors

Under here is the sensor code to be loaded into the ESP32s using an Arduino IDE


# 1-Wire



The standard Aruino 1Wire library doesnt work for ESP32s because of the dual core nature of the ESP32 and the strict timing demands of the 1Wire protocol.  The OneWire library from https://github.com/stickbreaker/OneWire.git is fixed for the ESP32


    git clone https://github.com/stickbreaker/OneWire.git
    git co v2.3.3-ESP32
    ln -s $(pwd)/OneWire ~/Documents/Arduino/libraries

The library should be ok for Ardiono boards as well.
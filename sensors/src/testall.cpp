#ifndef ARDUINO
#define TEST 1
#endif

#ifdef TEST
#import "testmocks.h"
#import "UptimeSensor.h"
#import "OneWireSensor.h"
#import "BMP280Sensor.h"
#import "BLESensorServer.h"


#define ONEWIREBUS 15  // ESP32 OneWire pin.

int main() {

  SkSensor * sensors[] = {
     new SKUptimeSensor(),
     new BMP280Sensor(),
     new OneWireSensor(ONEWIREBUS),
     NULL
  };
  SkBLEService*  service = new SkBLEService("4fafc201-1fb5-459e-8fcc-c5c9c331914b",&sensors[0]);
  service->start();

}
#endif





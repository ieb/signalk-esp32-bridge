
#import <UptimeSensor.h>
#import <OneWireSensor.h>
#import <BMP280Sensor.h>
#import <VoltageSensor.h>
#import <BLESensorServer.h>

#define ONEWIREBUS 15  // ESP32 OneWire pin.

OneWireSensor * oneWire;

void setup() {
  Serial.begin(115200);
  oneWire = new OneWireSensor(ONEWIREBUS);
  SkSensor * sensors[] = {
     new SKUptimeSensor(),
     new BMP280Sensor(),
     oneWire,
     new VoltageSensor((uint8_t *)VoltageSensor_ADC1_PINS),
     NULL
  };
  SkBLEService*  service = new SkBLEService("4fafc201-1fb5-459e-8fcc-c5c9c331914b",&sensors[0]);
  service->start();
}


void loop() {
  oneWire->execute();
  delay(oneWire->getDelay());
}

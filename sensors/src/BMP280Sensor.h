#ifndef __BMP280SENSOR_H__
#define __BMP280SENSOR_H__

#include "common.h"

#ifndef TEST
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define log(x) Serial.println(x)

#endif



class BMP280Sensor: public SkSensor, public BLECharacteristicCallbacks   {
public:
  typedef struct {
      float temperature;
      float pressure;
      float humidity;
  } pt_sensor_t;

  BMP280Sensor() {
      bmp_temp = bmp.getTemperatureSensor();
      bmp_pressure = bmp.getPressureSensor();
      // Library seems to think this is at 0x77 which all the BMP280s I have had are not.
      if (!bmp.begin(0x76, 0x58)) {
        log(F("Could not find a valid BMP280 sensor, check wiring!"));
        enabled = false;
        return;
      }
      enabled = true;
    
      /* Default settings from datasheet. */
      bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    
      bmp_temp->printSensorDetails();
      log("BMP280 Sensor setup Complete");

  }
  const char * getName() {
    return "BPM280 at beb5483e-36e1-4688-b7f5-ea07361b26a8";
  }
  const char * getCharacteristicUUID() {
    return "beb5483e-36e1-4688-b7f5-ea07361b26a8";
  }
  BLECharacteristicCallbacks * getCallbacks() {
    return this;
  }
  void read(pt_sensor_t * readings) {
    sensors_event_t temp_event, pressure_event;
    this->bmp_temp->getEvent(&temp_event);
    this->bmp_pressure->getEvent(&pressure_event);
    readings->pressure = pressure_event.pressure;
    readings->temperature = pressure_event.temperature;

  }
  void onRead(BLECharacteristic* pcharac) {
      pt_sensor_t readings;
      read(&readings);
      pcharac->setValue((uint8_t *)&readings, sizeof(readings));
      log("BMP280 Read done");
  }
  void onWrite(BLECharacteristic* pCharacteristic) {
  }
private:
  Adafruit_BMP280 bmp; // use I2C interface
  Adafruit_Sensor * bmp_temp;
  Adafruit_Sensor * bmp_pressure;
};





#endif
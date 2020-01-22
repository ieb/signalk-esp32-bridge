#ifndef __ONEWIRESENSOR_H__
#define __ONEWIRESENSOR_H__


#include "common.h"

#ifndef TEST
#define REQUIRESALARMS false
#include <OneWire.h>
#include <DallasTemperature.h>

#define log(x) Serial.println(x)
#define logHex(x)  Serial.print(x, HEX)
#else 
#define logHex(x)  std::cout << x 
#endif


#define MAX_SENSORS 5


class OneWireSensor: public SkSensor, public BLECharacteristicCallbacks   {
public:
  typedef struct {
      float temperature;
      float pressure;
      float humidity;
  } pt_sensor_t;

  OneWireSensor(uint8_t oneWireBus) {
    oneWire = new OneWire(oneWireBus);
    sensors = new DallasTemperature(oneWire);
    sensors->begin();
    oneWire->reset_search();
    for( maxSensorN = 0; maxSensorN < MAX_SENSORS; maxSensorN++ ) {
      // allocate space for the sensor address, its a unit8_t[] 
      sensorAddress[maxSensorN] = (DeviceAddress *)malloc(sizeof(DeviceAddress));
      memset(sensorAddress[maxSensorN],0,sizeof(DeviceAddress));
      if ( !oneWire->search(*sensorAddress[maxSensorN]) ) {
        break;
      }
      logb("Found Sensor at :");
      printAddress(*sensorAddress[maxSensorN]);
    }
    if ( maxSensorN == 0 ) {
      enabled = false;
      log("No 1wire devices attached.");  
      return;    
    }
    enabled = true;
    logb("Number Found :");
    log(maxSensorN);
    for (int i = 0; i < maxSensorN; i++) {
        sensors->setResolution(*sensorAddress[i], 12); // 12 bit precision 0.0625 C
    }
    log("OneWireSensort setup complete ");

  }
  ~OneWireSensor() {
    for (int i = 0; i < maxSensorN; i++) {
      free(sensorAddress[i]);
    }
  }

  void printAddress(DeviceAddress deviceAddress) {
    for (uint8_t i = 0; i < 8; i++) {
      // zero pad the address if necessary
      if (deviceAddress[i] < 16) logb("0");
      logHex(deviceAddress[i]);
    }
    log("");
  }

  const char * getName() {
    return "OneWire at f3f7dd8f-6fee-47fd-a1bf-8785cf19b622";
  }

  const char * getCharacteristicUUID() {
    return "f3f7dd8f-6fee-47fd-a1bf-8785cf19b622";
  }

  BLECharacteristicCallbacks * getCallbacks() {
    return this;
  }
  void onRead(BLECharacteristic* pcharac) {
    sensors->requestTemperatures();
    for (int i = 0; i < maxSensorN; i++) {
          temperature[i] = sensors->getTempC(*sensorAddress[i])*10;
    }
    pcharac->setValue((uint8_t *)&temperature[0], sizeof(float)*maxSensorN);
    log("One wire Read done");
  }
  void onWrite(BLECharacteristic* pCharacteristic) {
  }
private:
  DallasTemperature * sensors;
  OneWire * oneWire;
  uint8_t maxSensorN;
  DeviceAddress * sensorAddress[MAX_SENSORS];
  uint16_t temperature[MAX_SENSORS];
};





#endif
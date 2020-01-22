#ifndef __BLESENSOR_NULL_H__
#define __BLESENSOR_NULL_H__

#include "common.h"

#ifndef TEST
#include <BLECharacteristic.h>

#define log(x) Serial.println(x)

#endif




class SKUptimeSensor: public SkSensor, public BLECharacteristicCallbacks   {
public:

  SKUptimeSensor() {  
    enabled = true;
    log("Uptime sensor setup complete");
  }
  const char * getName() {
    return "Uptime at 9122a4c3-ac92-47c4-8d3d-fb0f5f8e137a";
  }
  const char * getCharacteristicUUID() {
    return "9122a4c3-ac92-47c4-8d3d-fb0f5f8e137a";
  }
  BLECharacteristicCallbacks * getCallbacks() {
    return this;
  }
  void onRead(BLECharacteristic* pcharac) {
    unsigned long t = millis();
    pcharac->setValue((uint8_t *)&t, sizeof(unsigned long));
    log("Uptime Read done");
  }
  void onWrite(BLECharacteristic* pCharacteristic) {
  }
private:
};





#endif
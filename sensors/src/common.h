#ifndef __BLESENSOR_COMMON_H__
#define __BLESENSOR_COMMON_H__


#ifndef TEST

#include <BLECharacteristic.h>

#define logb(x) Serial.print(x)
#define log(x) Serial.println(x)

#endif


class SkSensor {
public:
  // note to self, if not = 0 clang doesnt construct the typeinfo for the class of vtable.
  SkSensor() {
    enabled = false;
  }
  virtual BLEUUID getCharacteristicUUID(uint8_t i) {
    return *uuid;
  }
  virtual BLECharacteristicCallbacks * getCallbacks(uint8_t i) = 0;
  virtual const char * getName() = 0;
  virtual uint8_t getNCharacteristics() = 0;
  virtual uint8_t getAccess(uint8_t i) {
    return BLECharacteristic::PROPERTY_READ;
  }
protected:
  bool enabled;
  BLEUUID *uuid;
};







#endif
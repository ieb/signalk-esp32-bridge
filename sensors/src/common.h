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
  virtual const char * getCharacteristicUUID()  = 0;
  virtual BLECharacteristicCallbacks * getCallbacks() = 0;
  virtual const char * getName() = 0;
  virtual bool isEnabled() {
    return enabled;
  };
protected:
  bool enabled;
};







#endif
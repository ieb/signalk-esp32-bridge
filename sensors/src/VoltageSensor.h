#ifndef __VOLTAGESENSOR_H__
#define __VOLTAGESENSOR_H__

#include "common.h"

#ifndef TEST

#define log(x) Serial.println(x)

#else 

#define analogRead(x) x*5; std::cout << "Reading " << x << std::endl;

#endif



class VoltageSensor: public SkSensor, public BLECharacteristicCallbacks   {
public:

  VoltageSensor(uint8_t * newPins) {
    pins = NULL;
    voltages = NULL;
    savePins(newPins);
    log("Voltages Sensor setup Complete");

  }
  const char * getName() {
    return "Voltages at 2bfa8db4-868a-4e78-a841-e11c70083fc1";
  }
  const char * getCharacteristicUUID() {
    return "2bfa8db4-868a-4e78-a841-e11c70083fc1";
  }
  BLECharacteristicCallbacks * getCallbacks() {
    return this;
  }
  uint8_t getAccess() {
    return BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
  }

  void onRead(BLECharacteristic* pcharac) {
      for (int i = 0; i < nvoltages; ++i) {
        voltages[i] = analogRead(pins[i]); 
      }
      pcharac->setValue((uint8_t *)voltages, sizeof(uint16_t)*nvoltages);
      log("Voltages Read done");
  }
  void onWrite(BLECharacteristic* pcharac) {
    savePins(pcharac->getData());
  }

  void savePins(uint8_t * newPins) {
    if ( pins != NULL ) {
      free(pins);
      pins = NULL;
    }
    if ( voltages != NULL ) {
      free(voltages);
      voltages = NULL;
    }
    nvoltages = 0;
    while(newPins[nvoltages] != 0 && nvoltages < 15) nvoltages++;
    pins = (uint8_t *)malloc( sizeof(uint8_t)*nvoltages);
    voltages = (uint16_t *)malloc( sizeof(uint16_t)*nvoltages);
    logb("New Pins are ");
    for (int i = 0; i < nvoltages; ++i) {
      logb(newPins[i]);
      logb(" ");
      pins[i] = newPins[i];
      voltages[i] = 0;
    }
    log("");
    enabled = true;
  }
private:
  uint16_t *voltages;
  uint8_t *pins;
  int nvoltages;
};

  // see https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/ for numbers
  // 36 == ADC1 CH0
  // 39 == ADC1 CH3
  // 32 == ADC1 CH4
  // 33 == ADC1 CH5
  // 34 == ADC1 CH6
  // 35 == ADC1 CH7
  // 4  == ADC2 CH0
  // 2  == ADC2 CH2
  // 15 == ADC2 CH3
  // 13 == ADC2 CH4
  // 12 == ADC2 CH5
  // 14 == ADC2 CH6
  // 27 == ADC2 CH7
  // 25 == ADC2 CH8
  // 26 == ADC2 CH9

const uint8_t VoltageSensor_ADC1_PINSA[] = { 36, 39, 32, 33, 34, 35, 0 };
const uint8_t VoltageSensor_ADC2_PINSA[] =  { 4, 2, 15, 13, 12, 14, 27, 25, 26, 0 };
const uint8_t VoltageSensor_ADC_ALL_PINSA[] = { 36, 39, 32, 33, 34, 35, 4, 2, 15, 13, 12, 14, 27, 25, 26, 0 };
const uint8_t * VoltageSensor_ADC1_PINS = &VoltageSensor_ADC1_PINSA[0];
const uint8_t * VoltageSensor_ADC2_PINS = &VoltageSensor_ADC2_PINSA[0];
const uint8_t * VoltageSensor_ADC_ALL_PINS = &VoltageSensor_ADC_ALL_PINSA[0];



#endif
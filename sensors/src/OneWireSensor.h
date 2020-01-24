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

#define VALUES_CHARACTERISTIC_UUID "f3f7dd8f-6fee-47fd-a1bf-8785cf19b622"
#define IDS_CHARACTERISTIC_UUID "ee0c174e-deb8-459b-a981-a4bfd9b18ffe"
DeviceAddress sensor1 = { 0x28, 0xFF, 0x51, 0x2A, 0xA3, 0x16, 0x3, 0xCD };





class OneWireSensor: public SkSensor, public BLECharacteristicCallbacks {
public:
  typedef struct {
      unsigned long age;
      uint16_t readDuration;
      uint8_t nSensors;
      float temperature[MAX_SENSORS];
  } one_wire_readings;

  typedef struct {
      uint8_t nSensors;
      uint8_t addresses[sizeof(DeviceAddress)*MAX_SENSORS];
  } one_wire_ids;

  OneWireSensor(uint8_t oneWireBus) {
    oneWire = new OneWire(oneWireBus);
    sensors = new DallasTemperature(oneWire);
    sensors->begin();

    findSensors();

    if ( oneWireIds.nSensors == 0 ) {
      enabled = false;
      log("No 1wire devices attached.");  
      return;    
    }
    enabled = true;
    logb("Number Found :");
    log(oneWireIds.nSensors);

    valuesCharacteristic = new BLEUUID(VALUES_CHARACTERISTIC_UUID);
    idsCharacteristic = new BLEUUID(IDS_CHARACTERISTIC_UUID);


    readDelay = 10000;


     log("OneWireSensort setup complete ");

  }

  ~OneWireSensor() {
  }

  void findSensors() {
    oneWire->reset_search();

    for( int i = 0; i < MAX_SENSORS; i++ ) {
      // allocate space for the sensor address, its a unit8_t[] 
      if ( !oneWire->search( &oneWireIds.addresses[sizeof(DeviceAddress)*i]) ) {
        oneWireIds.nSensors = i;
        temperatures.nSensors = i;
        break;
      }
      logb("Found Sensor at :");
      printAddress(&oneWireIds.addresses[sizeof(DeviceAddress)*i]);
    }
    for (int i = 0; i < oneWireIds.nSensors; i++) {
        sensors->setResolution(&oneWireIds.addresses[sizeof(DeviceAddress)*i], 12); // 12 bit precision 0.0625 C
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

  /*
   *  SkSensor interface
   */

  const char * getName() {
    return "OneWire at values f3f7dd8f-6fee-47fd-a1bf-8785cf19b622 and ids ee0c174e-deb8-459b-a981-a4bfd9b18ffe";
  }

  BLEUUID getCharacteristicUUID(uint8_t i) {
    if ( i == 0 ) {
      return *valuesCharacteristic;;
    } 
    return *idsCharacteristic;
  }
  uint8_t getAccess(uint8_t i) {
    if ( i == 0 ) {
      return BLECharacteristic::PROPERTY_READ;
    }
    return BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
  }

  uint8_t getNCharacteristics() {
    if ( enabled ) {
      return 2;
    } 
    return 0;
  }


  BLECharacteristicCallbacks * getCallbacks(uint8_t i) {
    return this;
  }
  /*
   *  BLECharacteristicCallbacks interface
   */

  void onRead(BLECharacteristic* pcharac) {
    lock();
    BLEUUID uuid = pcharac->getUUID();
    if ( idsCharacteristic->equals(uuid)  ) {
      pcharac->setValue((uint8_t *)&oneWireIds, sizeof(one_wire_ids));
    } else {
      temperatures.age = millis() - lastRead;
      pcharac->setValue((uint8_t *)&temperatures, sizeof(one_wire_readings));
    }
    unlock();
  }

  void onWrite(BLECharacteristic* pcharac) {
    lock();
    BLEUUID uuid = pcharac->getUUID();
    if ( idsCharacteristic->equals(uuid) ) {
      uint8_t * data = pcharac->getData();
      readDelay = ((unsigned long *)data)[0];
      logb("Read Delay now :");
      log(readDelay);
      findSensors();
    }
    unlock();
  }

  /*
   *  PeriodicOperation interface
   */
  void started() {
    log("ReadSensors Started");
  }
  void finished() {
    log("ReadSensors Exited");
  }

  void lock() {
//    xSemaphoreTake(sem, portMAX_DELAY);
  }
  void unlock() {
//    xSemaphoreGive(sem);
  }


  void execute() {
    lock();
    unsigned long now = millis();
    sensors->requestTemperatures();
    // if the last read value wasnt read, then extend the duration
    for (int i = 0; i < temperatures.nSensors; i++) {
      temperatures.temperature[i] = sensors->getTempC(&oneWireIds.addresses[sizeof(DeviceAddress)*i]); //*sensorAddress[i]);
      logb(temperatures.temperature[i]);
      logb(" ");
      printAddress(&oneWireIds.addresses[sizeof(DeviceAddress)*i]); //*sensorAddress[i]);
    }
    lastRead = millis();
    temperatures.readDuration = (uint16_t) (lastRead - now);
    unlock();
  }

  unsigned long getDelay() {  
    return readDelay;
  }


private:
  DallasTemperature * sensors;
  OneWire * oneWire;
  unsigned long readDelay;
  unsigned long lastRead;
  SemaphoreHandle_t sem;
  BLEUUID * valuesCharacteristic;
  BLEUUID * idsCharacteristic;
  one_wire_ids oneWireIds;
  one_wire_readings temperatures;

};





#endif
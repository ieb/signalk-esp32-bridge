#ifndef __TESTMOCKS_H__
#define __TESTMOCKS_H__

#include <unistd.h>
#include <iostream>
#include <string.h>

#define logb(x) std::cout << x
#define log(x) std::cout << x << std::endl
#define F(x) x
#define delay(x) std::cout << "delay:" << x << std::endl 

unsigned long mils = 0;
unsigned long millis() {
  return mils++;
}

class BLEUUID {
public:
  BLEUUID(std::string uuid) {

  }
  static BLEUUID fromString(std::string uuid) {
    return *(new BLEUUID(uuid));
  }
  bool equals(BLEUUID uuid) {
    return false;
  }


};
class BLECharacteristicCallbacks {

};

uint8_t testDataWritten[] = { 36, 39, 32, 33, 34, 35, 0 };

class BLECharacteristic {
public:
  void setCallbacks(BLECharacteristicCallbacks  * callbacks) {

  }
  void setValue(uint8_t * buffer, uint32_t size) {
    std::cout << "Setting value for characteristed to " << size << " long" << std::endl;
  }

  uint8_t*  getData() {
    return &testDataWritten[0];
  }
  BLEUUID getUUID() {
    return *(new BLEUUID("nothing"));
  }
  static const uint32_t PROPERTY_READ      = 1<<0;
  static const uint32_t PROPERTY_WRITE     = 1<<1;
  static const uint32_t PROPERTY_NOTIFY    = 1<<2;
  static const uint32_t PROPERTY_BROADCAST = 1<<3;
  static const uint32_t PROPERTY_INDICATE  = 1<<4;
  static const uint32_t PROPERTY_WRITE_NR  = 1<<5;
};



class BLEServerCallbacks {

};

class BLEAdvertising {
public:
   void addServiceUUID(const char * serviceUID) {

   }
   void setMinPreferred(uint8_t i) {

   }
   void setMaxPreferred(uint8_t i) {
    
   }
   void setMinInterval(uint8_t i) {
    
   }
   void setMaxInterval(uint8_t i) {
    
   }
   void setScanResponse(bool i) {
    
   }

};


class BLEService {
public:
  BLECharacteristic*  createCharacteristic(const char * uuid, uint16_t options) {
    return new BLECharacteristic();
  }
  BLECharacteristic* createCharacteristic(BLEUUID uuid, uint32_t properties) {
    return new BLECharacteristic();    
  }
  void start() {

  }
private:
};



class BLEServer {
public:
  BLEService* createService(const char * uuid) {
    return new BLEService();
  }
  BLEService*  createService(BLEUUID uuid, uint32_t numHandles=15, uint8_t inst_id=0) {
    return new BLEService();

  }

  void startAdvertising() {
    
  }
  void setCallbacks(BLEServerCallbacks* callbacks) {

  }

private:
  BLEService service;

};


class BLEDevice {

public:
  static void init(const char * ) {

  }
  static BLEServer* createServer() {
    return new BLEServer();
  }
  static BLEAdvertising* getAdvertising() {
    return new BLEAdvertising();
  }
private:
  BLEServer bleServer;
  BLEAdvertising bleAdvertising;
};



 typedef struct {
    float temperature;
    float pressure;
    float humidity;
  } sensors_event_t;

class Adafruit_Sensor {
public:
  void printSensorDetails() {

  }
  void getEvent(sensors_event_t * event) {

  }
};

class Adafruit_BMP280 {
public:
  bool begin(uint8_t id1, uint8_t id2) {
    return true;
  }
  Adafruit_Sensor* getTemperatureSensor() {
    return new Adafruit_Sensor();
  }
  Adafruit_Sensor* getPressureSensor() {
    return new Adafruit_Sensor();
  }
  void setSampling(uint32_t op_mode,     /* Operating Mode. */
                    uint32_t temp_sampling,     /* Temp. oversampling */
                    uint32_t pressure_sampling,
                    uint32_t filtering,
                    uint32_t standby_time) {

  }
  static const uint32_t MODE_NORMAL = 0x00000001;
  static const uint32_t SAMPLING_X2 = 0x00000002;
  static const uint32_t SAMPLING_X16 = 0x00000004;
  static const uint32_t FILTER_X16  = 0x00000008;
  static const uint32_t STANDBY_MS_500 = 500;

};

typedef uint8_t DeviceAddress[8];

class OneWire {
public:
  OneWire(uint8_t pin) {

  }
  void reset_search() {
    deviceN = 4;
  }
  bool search(DeviceAddress address) {
    if (deviceN == 0) {
      return false;
    }
    address[0] = deviceN;
    deviceN--;
    return true;
  }

private:
  uint8_t deviceN;

};

class DallasTemperature {
public:
  DallasTemperature(OneWire * onewire) {

  }
  void begin() {

  }
  void setResolution(DeviceAddress sensorAddress, uint8_t  precision) {

  }
  void requestTemperatures() {

  }
  float getTempC(DeviceAddress address) {
    return 20+(address[0]);
  }
};





#endif


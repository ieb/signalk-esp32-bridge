#ifndef __BLESENSORSERVER_H__
#define __BLESENSORSERVER_H__

#include "common.h"

#ifndef TEST
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLECharacteristic.h>

#define log(x) Serial.println(x)

#endif

class ServerDebugCallback: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      log("Connected");
    };

    void onDisconnect(BLEServer* pServer) {
      log("Disconnected");
    }
    
};


class SkBLEService {
public:
  SkBLEService(const char * serviceUID, SkSensor ** sensors ) {
       BLEDevice::init("ESP32");

      // Create the BLE Server
      pServer = BLEDevice::createServer();
      pServer->setCallbacks(new ServerDebugCallback());



      // Create the BLE Service
      this->serviceUID = serviceUID;
      BLEService *pService = pServer->createService(serviceUID);

      // Create a BLE Characteristic for each sensor.
      SkSensor ** psensors = sensors;
      while(true) {
        SkSensor * sensor = *psensors; 
        if (sensor == NULL) {
          break;
        }
        if ( sensor->isEnabled() ) {
          BLECharacteristic* pCharacteristic = pService->createCharacteristic(
                          sensor->getCharacteristicUUID(),
                          BLECharacteristic::PROPERTY_READ  
                        );
          pCharacteristic->setCallbacks(sensor->getCallbacks());
          psensors++;                  
          logb("Enabled ");
        } else {
          logb("Disabled ");

        }
        log(sensor->getName());
      }
      // Start the service
      pService->start();
       
  }
  void start() {

      // Start advertising
      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->addServiceUUID(serviceUID);
    
      // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml.
      //pAdvertising->setApperance(0x0);  // controls the Icon used for the service when it is displayed. Core Spec section 3.2.5.3
      pAdvertising->setMinPreferred(0x06);  // I think these are the delays between connection packets in 1.25ms increments.
      pAdvertising->setMaxPreferred(0x12);  // These values come from others testing with iOS, although 0 works fine with a Pi.
      pAdvertising->setMinInterval(0x20); // advertising interval in 0.625 ms intervals max can be 10s
      pAdvertising->setMaxInterval(0x80); // increasing will increase battery life. default is 0x40.
      pAdvertising->setScanResponse(false);
      pServer->startAdvertising();
      log("Advertising, waiting for connect");
  }
private:
  const char *serviceUID;
  BLEServer* pServer;
};


#endif

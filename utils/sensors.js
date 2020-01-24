/*jshint node:true */
"use strict";

const EventEmitter = require('events');
const util = require('util');


class BMP280Sensor extends EventEmitter {
    constructor(peripheral, interval) {
        super();
        interval = interval || 15000;
        var that = this;
        // find services of interest to poll
        // wathching notifies doesnt work on a pi due to problems low down
        const BMP280_CHARACTERISTIC = "beb5483e36e14688b7f5ea07361b26a8";
        peripheral.findAll((service, characteristic) => {
            console.log("Found BMP280Sensor ",service.uuid, characteristic.uuid);
          setInterval( async () => {
            var data = await characteristic.readValue();
            console.log("BMPData",data);
            // temp, pressure, humidity as 3 32bit floats, 12 bytes long see BMP280Sensor.h pt_sensor_t
            var enviro = {
              service: service.uuid,
              temperature: data.readFloatLE(0),
              pressure: data.readFloatLE(4),
              humidity: data.readFloatLE(8)
            };
            that.emit("data", enviro);
          }, interval);
        }, undefined, BMP280_CHARACTERISTIC);

    }

}

class OneWireTemperatureSensor extends EventEmitter {
    constructor(peripheral, interval) {
        super();
        interval = interval || 15000;
        var that = this;
        const ONEWIRE_VALUES_CHARACTERISTIC_UUID = "f3f7dd8f6fee47fda1bf8785cf19b622"
        const ONEWIRE_IDS_CHARACTERISTIC_UUID = "ee0c174edeb8459ba981a4bfd9b18ffe";
        peripheral.findAll( async (service, characteristic) => {
            console.log("Found OneWireTemperatureSensor ",service.uuid, characteristic.uuid);
          // setup required
          var idsCharacteristic = service.getCharacteristic(ONEWIRE_IDS_CHARACTERISTIC_UUID);
          var ids = await idsCharacteristic.readValue();
          // first byte is the number of sensors, followed by n*8bytes of device IDs.
          var nsensors = ids[0];
          var sensorIDs = [];
          for (var i = 0; i < nsensors*8; i+=8) {
            sensorIDs.push(ids.toString("hex",i+1,i+8));
          };
          console.log("Found OneWire sensors ",sensorIDs);

          setInterval( async () => {
            var data = await characteristic.readValue();
            console.log("OneWire",data);
            /*
      typedef struct {
          unsigned long age;
          uint16_t readDuration;
          uint8_t nSensors;
          float temperature[MAX_SENSORS];
      } one_wire_readings;
            */
            var enviro = {
              service: service.uuid,
              age: data.readUInt32LE(0),
              duration: data.readUInt16LE(4),
              nSensors: data[6],
              temperature: []
            };
            // the struct has been padded to word boundaries.
            for (var i = 0; i < 5; i++) {
              enviro.temperature.push(data.readFloatLE(8+i*4));
            };
            that.emit("data", enviro);
          }, interval);
        }, undefined, ONEWIRE_VALUES_CHARACTERISTIC_UUID);
    }
}

class VoltageSensor extends EventEmitter {
    constructor(peripheral, interval) {
        super();
        interval = interval || 15000;
        var that = this;
        const VoltageSensor_ADC1_PINS = [ 36, 39, 32, 33, 34, 35, 0 ];
        const VOLTAGE_CHARACTERISTIC = "2bfa8db4868a4e78a841e11c70083fc1";
        peripheral.findAll((service, characteristic) => {
            console.log("Found VoltageSensor ",service.uuid, characteristic.uuid);
          setInterval( async () => {
            var data = await characteristic.readValue();
            console.log("Volatages",data);
            // voltages all unit16_t 
            var voltages = {
              service: service.uuid,
              pins: VoltageSensor_ADC1_PINS,
              measurement: []
            };
            for (var i = 0; i < data.length; i+=2) {
                voltages.measurement.push(3.3*data.readUInt16LE(i)/4096);
            };
            that.emit("data", voltages);
          }, interval);
        }, undefined, VOLTAGE_CHARACTERISTIC);
    }
}
class UptimeSensor extends EventEmitter {
    constructor(peripheral, interval) {
        super();
        interval = interval || 15000;
        var that = this;

        const UPTIME_CHARACTERISTIC = "9122a4c3ac9247c48d3dfb0f5f8e137a";
        peripheral.findAll((service, characteristic) => {
            console.log("Found UptimeSensor ",service.uuid, characteristic.uuid);
          setInterval( async () => {
            var data = await characteristic.readValue();
            console.log("Uptime",data);
            // uptime in millis, unsigned long 
            var uptime = {
              service: service.uuid,
              millis: 0
            };
            uptime.millis = data.readUInt32LE(0);
            that.emit("data", uptime);
          }, interval);
        }, undefined, UPTIME_CHARACTERISTIC);

    }
}


module.exports = {
    BMP280Sensor,
    OneWireTemperatureSensor,
    VoltageSensor,
    UptimeSensor
};
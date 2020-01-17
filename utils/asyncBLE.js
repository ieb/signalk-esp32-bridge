/*jshint node:true */
"use strict";


// these classes wrap callbacks with promises
// so that the methods can be awaited on used as promises
// Inside the promise where there is annother callback, that
// if it also needs to wait, then it is made async and the call inside
// is awaited on.
// for instance, calling  BLEPeripheral.connect will not return until
// the services has been read, the characteristics read, the desciptors have been read.


class BLEPeripheral {
    constructor(peripheral) {
        this.peripheral = peripheral;
        this.services = [];
    }

    // returns a promise so await on this or use then on the promise
    connect() {
        var that = this;
        that.peripheral.on('disconnect', function() {
            console.log("Disconnecting");
            process.exit(0);
        });

        return new Promise((resolve, reject) => {
            // making the callback async ensures await will work.
            that.peripheral.connect(async (error) => {
                console.log("Connected");
                if (error) {
                    reject(error);
                } else {
                    await that.readServices();                        
                    resolve();

                }
            });
        });
    }

    // returns a promise so await on this or use then on the promise
    readServices() {
        console.log("Reading Services");
        var that = this;
        return new Promise((resolve, reject) => {
            that.peripheral.discoverServices([], async (error, servs) => {
              if (error) {
                console.log("Got Errors reading services");
                reject(error);
              } else {
                console.log("Got services ",servs.length);
                for (var i = 0; i < servs.length; i++) {
                    var serviceOjb = new BLEService(servs[i]);
                    that.services.push(serviceOjb);
                    await serviceOjb.readCharacteristics();
                };
                resolve();
              }
            });
        });
    }


}

class BLEService {
    constructor(service) {
        this.service = service;
        this.characteristics = [];
    }

    readCharacteristics() {
        console.log("Reading Characteristics ");
        var that = this;
        return new Promise((resolve, reject) => {
            // call back is async, so it will allow awaits to resolve before
            // returning. 
          that.service.discoverCharacteristics([], async (error, characs) => {
            if(error) {
                console.log("Got Errors reading characteristics");
                reject(error);
            } else {
                console.log("Got Characteristics ", characs.length);
                for (var i = 0; i < characs.length; i++) {
                    var characteristic = new BLECharacteristic(characs[i]);
                    that.characteristics.push(characteristic);
                    await characteristic.readDescriptors();
                };
                resolve();
           }
          });
        });
    }
}


class BLECharacteristic {
    constructor(characteristic) {
        this.characteristic = characteristic;
        this.descriptors = [];
    }

    readDescriptors() {
        console.log("Reading Descriptors ");
        var that = this;
        return new Promise((resolve, reject) => {
            that.characteristic.discoverDescriptors( async (error, descs) => {
                if(error) {
                console.log("Got Errors reading descriptors");
                    reject(error);
                } else {
                    console.log("Reading Descriptors ", descs.length);
                    for (var i = 0; i < descs.length; i++) {
                        var descriptor = new BLEDescriptor(descs[i]);
                        await that.descriptors.push(descriptor);
                    };
                    resolve();
               }
          });
        });
    }
    readValue() {
        var that = this;
        return new Promise((resolve, reject) => {
            if (that.characteristic.properties.indexOf('read') === -1) {
                reject("Nothing to read");
            } else {
                that.characteristic.read((error, data) => {
                    if ( error ) {
                        reject(error);
                    } else {
                        resolve(data);
                    }
                });
            }
        });
    }
}

class BLEDescriptor {
    constructor(descriptor) {
        this.descriptor = descriptor;
    }

    readDescriptor() {
        var that = this;
        return new Promise((resolve, reject) => {
            if (that.descriptor.uuid === '2901') {
                resolve();
            } else {
                that.descriptor.readValue((error, data) => {
                    if ( error ) {
                        reject(error);
                    } else if ( data ) {
                        this.data = data.toString();
                    }
                    resolve();
                });
            }
        });
    }
}


module.exports = {
    BLEPeripheral,
    BLEService,
    BLECharacteristic,
    BLEDescriptor
};
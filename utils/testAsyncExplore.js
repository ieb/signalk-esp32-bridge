var noble = require('@abandonware/noble');
var {BLEPeripheral} = require('./asyncBLE');

var peripheralIdOrAddress = process.argv[2].toLowerCase();

noble.on('stateChange', function(state) {
  if (state === 'poweredOn') {
    noble.startScanning();
  } else {
    noble.stopScanning();
  }
});

noble.on('discover', async function(peripheral) {
  console.log("Found ",peripheral);
  if (peripheral.id === peripheralIdOrAddress || peripheral.address === peripheralIdOrAddress) {
    noble.stopScanning();

    console.log('peripheral with ID ' + peripheral.id + ' found');
    var advertisement = peripheral.advertisement;

    var localName = advertisement.localName;
    var txPowerLevel = advertisement.txPowerLevel;
    var manufacturerData = advertisement.manufacturerData;
    var serviceData = advertisement.serviceData;
    var serviceUuids = advertisement.serviceUuids;

    if (localName) {
      console.log('  Local Name        = ' + localName);
    }

    if (txPowerLevel) {
      console.log('  TX Power Level    = ' + txPowerLevel);
    }

    if (manufacturerData) {
      console.log('  Manufacturer Data = ' + manufacturerData.toString('hex'));
    }

    if (serviceData) {
      console.log('  Service Data      = ' + JSON.stringify(serviceData, null, 2));
    }

    if (serviceUuids) {
      console.log('  Service UUIDs     = ' + serviceUuids);
    }

    console.log();

    var peripheral = new BLEPeripheral(peripheral);
    await peripheral.connect();
    console.log("Connect finished ");
    peripheral.services.forEach((service) => {
      var serviceInfo = service.service.uuid;

      if (service.service.name) {
        serviceInfo += ' (' + service.service.name + ')';
      }
      console.log("Service",serviceInfo);
      service.characteristics.forEach((characteristic) => {
        var characteristicInfo = '  ' + characteristic.characteristic.uuid;

        if (characteristic.name) {
          characteristicInfo += ' (' + characteristic.characteristic.name + ')';
        }
        console.log("Charateristic", characteristicInfo);
        console.log('    properties  ' + characteristic.characteristic.properties.join(', '));
        characteristic.descriptors.forEach((descriptor) => {
          if (descriptor.data) {
            console.log('(',descriptor.data.toString(),')');
          }
        });
      });
    });
  }
});




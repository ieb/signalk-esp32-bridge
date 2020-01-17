var noble = require('@abandonware/noble');

var peripheralIdOrAddress = process.argv[2].toLowerCase();

noble.on('stateChange', function(state) {
  if (state === 'poweredOn') {
    noble.startScanning();
  } else {
    noble.stopScanning();
  }
});

noble.on('discover', function(peripheral) {
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

    explore(peripheral);
  }
});

function explore(peripheral) {
  console.log('services and characteristics:');

  peripheral.on('disconnect', function() {
    process.exit(0);
  });

  peripheral.connect(function(error) {
    console.log("Connect Error ",error);
    peripheral.discoverServices([], function(error, services) {
      console.log("Discover Error ",error);
      var serviceIndex = 0;
      console.log("Services ",services.length);

      for (var i = 0; i < services.length; i++) {
        var service = services[i];
        var serviceInfo = service.uuid;

        if (service.name) {
          serviceInfo += ' (' + service.name + ')';
        }
        console.log(serviceInfo);

        service.discoverCharacteristics([], function(error, characteristics) {
          characteristics.forEach((characteristic)=> {
              var characteristicInfo = '  ' + characteristic.uuid;

              if (characteristic.name) {
                characteristicInfo += ' (' + characteristic.name + ')';
              }

              characteristic.discoverDescriptors(function(error, descriptors) {
                descriptors.forEach((descriptor) => {
                  if (descriptor.uuid === '2901') {
                      userDescriptionDescriptor.readValue(function(error, data) {
                          if (data) {
                              characteristicInfo += ' (' + data.toString() + ')';
                          }
                      });
                  }
                });
                characteristicInfo += '\n    properties  ' + characteristic.properties.join(', ');
                if (characteristic.properties.indexOf('read') !== -1) {
                  characteristic.read(function(error, data) {
                      if (data) {
                        var string = data.toString('ascii');
                        characteristicInfo += '\n    value       ' + data.toString('hex') + ' | \'' + string + '\'';
                      }
                      console.log(characteristicInfo);
                    });
                } else {
                  console.log(characteristicInfo);
                }
              });
          });
        });
      }
    });
  });
}

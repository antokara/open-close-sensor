#include <Arduino.h>
#include "Device.h"
#include "Mag_Sensor.h"
#include "Open_Sensor.h"

// Device instance
Device *device;

// Mag_Sensor instance
Mag_Sensor *mag_Sensor;

// Open_Sensor instance
Open_Sensor *open_Sensor;

void setup()
{
  device = new Device();
  device->setup();
  mag_Sensor = new Mag_Sensor(device);
  mag_Sensor->setup();
  open_Sensor = new Open_Sensor(device, mag_Sensor);
  open_Sensor->setup();
  if (!device->has_fault())
  {
    device->log("Setup completed successfully.", true, DEBUG_LEVEL_INFO);
  }
  else
  {
    device->log("Setup failed!", true, DEBUG_LEVEL_ERROR);
  }
}

void loop()
{
  device->loop();
  if (device->has_fault())
  {
    // do not proceed if in fault state
    return;
  }

  mag_Sensor->loop();
  open_Sensor->loop();
}

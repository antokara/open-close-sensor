#include <Arduino.h>
#include <Device.h>
#include <Mag_Sensor.h>

// Device instance
Device *device;

// Mag_Sensor instance
Mag_Sensor *mag_Sensor;

void setup()
{
  device = new Device();
  device->setup();
  mag_Sensor = new Mag_Sensor(device);
  mag_Sensor->setup();
  if (!device->has_fault())
  {
    Serial.println("Setup completed successfully.");
  }
  else
  {
    Serial.println("Setup failed!");
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
  if (mag_Sensor->isUpdated())
  {
    Serial.print("MagX: ");
    Serial.print(mag_Sensor->getMagX());
    Serial.print(" MagY: ");
    Serial.print(mag_Sensor->getMagY());
    Serial.print(" MagZ: ");
    Serial.println(mag_Sensor->getMagZ());
  }
}

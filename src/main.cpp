#include <Arduino.h>
#include <Device.h>
#include <Sensor.h>

// Device instance
Device *device;

// Sensor instance
Sensor *sensor;

void setup()
{
  device = new Device();
  device->setup();
  sensor = new Sensor(device);
  sensor->setup();
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

  sensor->loop();
  if (sensor->isUpdated())
  {
    Serial.print("MagX: ");
    Serial.print(sensor->getMagX());
    Serial.print(" MagY: ");
    Serial.print(sensor->getMagY());
    Serial.print(" MagZ: ");
    Serial.println(sensor->getMagZ());
  }
}

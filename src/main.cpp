#include <Wire.h> // Used to establish serial communication on the I2C bus
#include <Arduino.h>
#include <Device.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>

TMAG5273 sensor; // Initialize hall-effect sensor

// I2C default address
uint8_t i2cAddress = TMAG5273_I2C_ADDRESS_INITIAL;

float oldMagX = 0;
float oldMagY = 0;
float oldMagZ = 0;
float delta = 1.5;

// Device instance
Device *device;

void setup()
{
  device = new Device();
  device->setup();

  Wire.begin();
  delay(5000);

  // Start serial communication at 115200 baud
  Serial.begin(115200);

  // If begin is successful (1), then start example
  if (sensor.begin(i2cAddress, Wire) != 1)
  {
    // failure
    Serial.println("Device failed to setup - Freezing code.");
    device->set_fault(true);
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

  // Checks if mag channels are on - turns on in setup
  if (sensor.getMagneticChannel() != 0)
  {
    float magX = sensor.getXData();
    float magY = sensor.getYData();
    float magZ = sensor.getZData();
    if (abs(magX - oldMagX) >= delta || abs(magY - oldMagY) >= delta || abs(magZ - oldMagZ) >= delta)
    {
      oldMagX = magX;
      oldMagY = magY;
      oldMagZ = magZ;

      Serial.print("(");
      Serial.print(magX);
      Serial.print(", ");
      Serial.print(magY);
      Serial.print(", ");
      Serial.print(magZ);
      Serial.println(") mT");
    }
  }
  else
  {
    // If there is an issue, stop the magnetic readings and restart sensor/example
    Serial.println("Mag Channels disabled, stopping..");
    device->set_fault(true);
  }

  delay(100);
}

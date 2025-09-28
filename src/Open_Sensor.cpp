#include "Open_Sensor.h"

Open_Sensor::Open_Sensor(Device *device, Mag_Sensor *mag_sensor)
{
    this->device_ = device;
    this->mag_sensor_ = mag_sensor;
}

void Open_Sensor::setup()
{
}

void Open_Sensor::loop()
{
    if (this->mag_sensor_->isUpdated())
    {
        Serial.print("MagX: ");
        Serial.print(this->mag_sensor_->getMagX());
        Serial.print(" MagY: ");
        Serial.print(this->mag_sensor_->getMagY());
        Serial.print(" MagZ: ");
        Serial.println(this->mag_sensor_->getMagZ());
    }
}

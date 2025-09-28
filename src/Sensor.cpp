#include "Sensor.h"

Sensor::Sensor(Device *device)
{
    this->device_ = device;
}

void Sensor::setup()
{
    Wire.begin();

    // If begin is successful (1), then start example
    if (sensor_.begin(i2cAddress_, Wire) != 1)
    {
        // failure
        Serial.println("Device failed to setup - Freezing code.");
        this->device_->set_fault(true);
    }
}

void Sensor::loop()
{
    if (this->updated_)
    {
        this->updated_ = false;
    }

    // Checks if mag channels are on - turns on in setup
    if (sensor_.getMagneticChannel() != 0)
    {
        float magX = sensor_.getXData();
        float magY = sensor_.getYData();
        float magZ = sensor_.getZData();
        if (abs(magX - oldMagX_) >= MAGNETIC_DELTA_THRESHOLD || abs(magY - oldMagY_) >= MAGNETIC_DELTA_THRESHOLD || abs(magZ - oldMagZ_) >= MAGNETIC_DELTA_THRESHOLD)
        {
            oldMagX_ = magX;
            oldMagY_ = magY;
            oldMagZ_ = magZ;
            this->updated_ = true;
        }
    }
    else
    {
        // If there is an issue, stop the magnetic readings and restart sensor/example
        Serial.println("Mag Channels disabled, stopping..");
        device_->set_fault(true);
    }
}

float Sensor::getMagX()
{
    return oldMagX_;
}

float Sensor::getMagY()
{
    return oldMagY_;
}

float Sensor::getMagZ()
{
    return oldMagZ_;
}

bool Sensor::isUpdated()
{
    return this->updated_;
}

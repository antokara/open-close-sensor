#include "Mag_Sensor.h"

Mag_Sensor::Mag_Sensor(Device *device)
{
    this->device_ = device;
}

void Mag_Sensor::setup()
{
    Wire.begin();

    // If begin is successful (1), then start example
    if (Mag_Sensor_.begin(i2cAddress_, Wire) != 1)
    {
        // failure
        Serial.println("Mag_Sensor failed to setup - Freezing code.");
        this->device_->set_fault(true);
    }
}

void Mag_Sensor::loop()
{
    if (this->updated_)
    {
        this->updated_ = false;
    }

    // Checks if mag channels are on - turns on in setup
    if (Mag_Sensor_.getMagneticChannel() != 0)
    {
        float magX = Mag_Sensor_.getXData();
        float magY = Mag_Sensor_.getYData();
        float magZ = Mag_Sensor_.getZData();
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
        // If there is an issue, stop the magnetic readings and restart Mag_Sensor/example
        Serial.println("Mag Channels disabled, stopping..");
        device_->set_fault(true);
    }
}

float Mag_Sensor::getMagX()
{
    return oldMagX_;
}

float Mag_Sensor::getMagY()
{
    return oldMagY_;
}

float Mag_Sensor::getMagZ()
{
    return oldMagZ_;
}

bool Mag_Sensor::isUpdated()
{
    return this->updated_;
}

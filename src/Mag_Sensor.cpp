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
        this->device_->log("Mag_Sensor failed to setup - Freezing code.", true, DEBUG_LEVEL_ERROR);
        this->device_->set_fault(true);
    }
}

void Mag_Sensor::loop()
{
    if (this->updated_x_)
    {
        this->updated_x_ = false;
    }
    if (this->updated_y_)
    {
        this->updated_y_ = false;
    }
    if (this->updated_z_)
    {
        this->updated_z_ = false;
    }

    // Checks if mag channels are on - turns on in setup
    if (Mag_Sensor_.getMagneticChannel() != 0)
    {
        float magX = Mag_Sensor_.getXData();
        float magY = Mag_Sensor_.getYData();
        float magZ = Mag_Sensor_.getZData();
        if (abs(magX - oldMagX_) >= MAG_SENSOR_DELTA_THRESHOLD)
        {
            oldMagX_ = magX;
            this->updated_x_ = true;
        }
        if (abs(magY - oldMagY_) >= MAG_SENSOR_DELTA_THRESHOLD)
        {
            oldMagY_ = magY;
            this->updated_y_ = true;
        }
        if (abs(magZ - oldMagZ_) >= MAG_SENSOR_DELTA_THRESHOLD)
        {
            oldMagZ_ = magZ;
            this->updated_z_ = true;
        }
    }
    else
    {
        // If there is an issue, stop the magnetic readings and restart Mag_Sensor/example
        this->device_->log("Mag Channels disabled, stopping..", true, DEBUG_LEVEL_ERROR);
        this->device_->set_fault(true);
    }
}

float Mag_Sensor::get_mag_x()
{
    return oldMagX_;
}

float Mag_Sensor::get_mag_y()
{
    return oldMagY_;
}

float Mag_Sensor::get_mag_z()
{
    return oldMagZ_;
}

bool Mag_Sensor::is_x_updated()
{
    return this->updated_x_;
}

bool Mag_Sensor::is_y_updated()
{
    return this->updated_y_;
}

bool Mag_Sensor::is_z_updated()
{
    return this->updated_z_;
}
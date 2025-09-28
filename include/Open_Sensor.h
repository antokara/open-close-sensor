#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include <Device.h>
#include "Mag_Sensor.h"

/**
 * Open_Sensor class to interface with the open/close sensor
 */
class Open_Sensor
{
private:
    /**
     * pointer to Device instance (for fault handling)
     */
    Device *device_;

    /**
     * pointer to Mag_Sensor instance (for magnetic field readings)
     */
    Mag_Sensor *mag_sensor_;

public:
    Open_Sensor(Device *device, Mag_Sensor *mag_sensor);

    /**
     * initialize Open_Sensor
     * Should be called in Arduino setup()
     */
    void setup();

    /**
     * process Open_Sensor state
     * should be called in Arduino loop()
     */
    void loop();
};
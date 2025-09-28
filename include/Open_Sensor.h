#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include <Device.h>

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

public:
    Open_Sensor(Device *device);

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
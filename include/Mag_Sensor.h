#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_TMAG5273_Arduino_Library.h"
#include "Device.h"

/**
 * threshold for the raw magnetic field change to be considered significant
 */
#define MAGNETIC_DELTA_THRESHOLD 0.25

/**
 * Mag_Sensor class to interface with the TMAG5273 magnetic sensor
 */
class Mag_Sensor
{
private:
    /**
     * last read magnetic X value with a significant change
     */
    float oldMagX_ = 0;

    /**
     * last read magnetic Y value with a significant change
     */
    float oldMagY_ = 0;

    /**
     * last read magnetic Z value with a significant change
     */
    float oldMagZ_ = 0;

    /**
     * true if Mag_Sensor X value was updated in last loop()
     * with significant change
     */
    bool updated_x_ = false;

    /**
     * true if Mag_Sensor X value was updated in last loop()
     * with significant change
     */
    bool updated_y_ = false;

    /**
     * true if Mag_Sensor X value was updated in last loop()
     * with significant change
     */
    bool updated_z_ = false;

    /**
     * TMAG5273 Mag_Sensor instance
     */
    TMAG5273 Mag_Sensor_;

    /**
     * I2C address of the TMAG5273 Mag_Sensor
     */
    uint8_t i2cAddress_ = TMAG5273_I2C_ADDRESS_INITIAL;

    /**
     * pointer to Device instance (for fault handling)
     */
    Device *device_;

public:
    Mag_Sensor(Device *device);

    /**
     * initialize Mag_Sensor
     * Should be called in Arduino setup()
     */
    void setup();

    /**
     * process Mag_Sensor state
     * should be called in Arduino loop()
     */
    void loop();

    /**
     * get last read magnetic X value with a significant change
     */
    float get_mag_x();

    /**
     * get last read magnetic Y value with a significant change
     */
    float get_mag_y();

    /**
     * get last read magnetic Z value with a significant change
     */
    float get_mag_z();

    /**
     * returns true if Mag_Sensor X value was updated in last loop()
     * with significant change
     */
    bool is_x_updated();

    /**
     * returns true if Mag_Sensor X value was updated in last loop()
     * with significant change
     */
    bool is_y_updated();

    /**
     * returns true if Mag_Sensor X value was updated in last loop()
     * with significant change
     */
    bool is_z_updated();
};
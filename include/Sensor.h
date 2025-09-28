#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include <Device.h>

/**
 * threshold for magnetic field change to be considered significant
 */
#define MAGNETIC_DELTA_THRESHOLD 1.5

class Sensor
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
     * true if sensor values were updated in last loop()
     * with significant change
     */
    bool updated_ = false;

    /**
     * TMAG5273 sensor instance
     */
    TMAG5273 sensor_;

    /**
     * I2C address of the TMAG5273 sensor
     */
    uint8_t i2cAddress_ = TMAG5273_I2C_ADDRESS_INITIAL;

    /**
     * pointer to Device instance (for fault handling)
     */
    Device *device_;

public:
    Sensor(Device *device);

    /**
     * initialize Sensor (e.g. pin modes).
     * Should be called in Arduino setup()
     */
    void setup();

    /**
     * process Sensor state (e.g. update sensor values).
     * should be called in Arduino loop()
     */
    void loop();

    /**
     * get last read magnetic X value with a significant change
     */
    float getMagX();

    /**
     * get last read magnetic Y value with a significant change
     */
    float getMagY();

    /**
     * get last read magnetic Z value with a significant change
     */
    float getMagZ();

    /**
     * returns true if sensor values were updated in last loop()
     * with significant change
     */
    bool isUpdated();
};
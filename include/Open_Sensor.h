#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_TMAG5273_Arduino_Library.h"
#include "Device.h"
#include "Mag_Sensor.h"
#include "Moving_Average.h"

#define OPEN_SENSOR_FILTER_WINDOW_SIZE 15

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

    /**
     * moving average filter for mag X axis
     */
    Moving_Average *mag_x_filter_{0};

    /**
     * last read filtered magnetic X value with a significant change
     */
    float old_filtered_mag_x_ = 0;

    /**
     * moving average filter for mag Y axis
     */
    Moving_Average *mag_y_filter_{0};

    /**
     * last read filtered magnetic Y value with a significant change
     */
    float old_filtered_mag_y_ = 0;

    /**
     * moving average filter for mag Z axis
     */
    Moving_Average *mag_z_filter_{0};

    /**
     * last read filtered magnetic Z value with a significant change
     */
    float old_filtered_mag_z_ = 0;

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
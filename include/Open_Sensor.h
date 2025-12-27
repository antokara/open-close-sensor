#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_TMAG5273_Arduino_Library.h"
#include "Device.h"
#include "Mag_Sensor.h"
#include "Moving_Average.h"
#include "XYZPointList.h"

/**
 * size of the moving average filter window
 * used to filter the magnetic field readings
 */
#define OPEN_SENSOR_FILTER_WINDOW_SIZE 10

/**
 * threshold for significant change
 * in the filtered magnetic field readings
 */
#define OPEN_SENSOR_DELTA_THRESHOLD 0.5

/**
 * maximum number of calibration points to store
 */
#define OPEN_SENSOR_MAX_CALIBRATION_POINTS 20

/**
 * threshold distance to consider a calibration point
 * as matching an existing point
 *
 * (the higher the value, the more likely a match is found and
 * thus, the more forgiving it is to slight variations in the magnetic field)
 *
 * @see MAG_SENSOR_DELTA_THRESHOLD
 * @see OPEN_SENSOR_DELTA_THRESHOLD
 */
#define OPEN_SENSOR_CALIBRATION_POINT_THRESHOLD 0.75

/**
 * minimum distance from the largest calibration point
 * to consider the sensor as open, as a multiplier.
 *
 * @example 0.25 means the sensor needs to be at least
 *          25% as far from the magnet (largest calibration point) to be considered open.
 *          This will make the sensor more sensitive to opening.
 *
 * @example 0.5 means the sensor needs to be at least half as far
 *          from the magnet (largest calibration point) to be considered open.
 *          This is a balanced sensitivity.
 *
 * @example 0.75 means the sensor needs to be at least 75% as far
 *          from the magnet (largest calibration point) to be considered open.
 *          This will make the sensor less sensitive to opening.
 */
#define OPEN_SENSOR_CALIBRATION_POINTS_DISTANCE_THRESHOLD_MULTIPLIER 0.5

/**
 * possible states of the open/close sensor
 */
enum Open_Sensor_State
{
    STATE_UNKNOWN = 0,
    STATE_OPEN = 1,
    STATE_CLOSED = 2,
    STATE_TAMPERED = 3
};

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

    /**
     * current state of the open/close sensor
     */
    Open_Sensor_State state_ = STATE_UNKNOWN;

    /**
     * list of recent (x,y,z) magnetic field points from calibration
     */
    XYZPointList *calibrationPoints_{nullptr};

    /**
     * true if sensor is in calibration mode
     */
    bool is_calibrating_{false};

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

    /**
     * get current state of the open/close sensor
     */
    Open_Sensor_State get_state();
};
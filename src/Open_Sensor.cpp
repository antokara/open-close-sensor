#include "Open_Sensor.h"

Open_Sensor::Open_Sensor(Device *device, Mag_Sensor *mag_sensor)
{
    this->device_ = device;
    this->mag_sensor_ = mag_sensor;
    this->calibrationPoints_ = new XYZPointList(MAX_CALIBRATION_POINTS);

    /**
     * ignore pulses, since those could be:
     *    - the sensor stabilizing
     *    - environmental magnetic noise (burst of constant)
     * a pulse is any sudden change in the magnetic field
     * that is not sustained and goes back close to previous value.
     *
     * how? use a moving average filter to smooth out the readings
     * and only consider a change if the smoothed value changes
     */
    this->mag_x_filter_ = new Moving_Average(OPEN_SENSOR_FILTER_WINDOW_SIZE);
    this->mag_y_filter_ = new Moving_Average(OPEN_SENSOR_FILTER_WINDOW_SIZE);
    this->mag_z_filter_ = new Moving_Average(OPEN_SENSOR_FILTER_WINDOW_SIZE);
}

void Open_Sensor::setup()
{
}

void Open_Sensor::loop()
{
    if (this->mag_sensor_->is_x_updated())
    {
        this->mag_x_filter_->add(this->mag_sensor_->get_mag_x());
    }
    if (this->mag_sensor_->is_y_updated())
    {
        this->mag_y_filter_->add(this->mag_sensor_->get_mag_y());
    }
    if (this->mag_sensor_->is_x_updated())
    {
        this->mag_z_filter_->add(this->mag_sensor_->get_mag_z());
    }

    // get filtered magnetic values
    float filtered_mag_x = this->mag_x_filter_->get();
    float filtered_mag_y = this->mag_y_filter_->get();
    float filtered_mag_z = this->mag_z_filter_->get();
    // check for significant changes
    bool updated = false;
    if (abs(this->old_filtered_mag_x_ - filtered_mag_x) >= OPEN_SENSOR_DELTA_THRESHOLD)
    {
        updated = true;
        this->old_filtered_mag_x_ = filtered_mag_x;
    }
    if (abs(this->old_filtered_mag_y_ - filtered_mag_y) >= OPEN_SENSOR_DELTA_THRESHOLD)
    {
        updated = true;
        this->old_filtered_mag_y_ = filtered_mag_y;
    }
    if (abs(this->old_filtered_mag_z_ - filtered_mag_z) >= OPEN_SENSOR_DELTA_THRESHOLD)
    {
        updated = true;
        this->old_filtered_mag_z_ = filtered_mag_z;
    }

    if (updated)
    {
        // handle calibration mode
        if (this->device_->is_calibrating())
        {
            if (!this->is_calibrating_)
            {
                // just entered calibration mode
                this->is_calibrating_ = true;
                this->calibrationPoints_->clear();
                this->device_->log("Open_Sensor: started calibration - clearing previous points", true, DEBUG_LEVEL_INFO);
            }
            int index = this->calibrationPoints_->find(
                filtered_mag_x,
                filtered_mag_y,
                filtered_mag_z,
                0,
                CALIBRATION_POINT_THRESHOLD);
            if (index == -1)
            {
                this->device_->log("Open_Sensor: adding new calibration point", true, DEBUG_LEVEL_INFO);
                this->calibrationPoints_->add(filtered_mag_x, filtered_mag_y, filtered_mag_z);
            }
            else
            {
                this->device_->log("Open_Sensor: calibration point already exists with index " + String(index), true, DEBUG_LEVEL_INFO);
            }
        }
        else if (this->is_calibrating_)
        {
            // just exited calibration mode
            this->is_calibrating_ = false;
            this->device_->log("Open_Sensor: exited calibration mode", true, DEBUG_LEVEL_INFO);
            // TODO: report the collected calibration points

            // TODO: save calibration points to non-volatile storage
        }
        else
        {
            // normal operation
            int index = this->calibrationPoints_->find(
                filtered_mag_x,
                filtered_mag_y,
                filtered_mag_z,
                0,
                CALIBRATION_POINT_THRESHOLD);
            if (index != -1)
            {
                if (index > (int)this->calibrationPoints_->size() / 2)
                {
                    this->state_ = STATE_OPEN;
                    this->device_->log("Open_Sensor: OPEN " + String(index), true, DEBUG_LEVEL_INFO);
                }
                else
                {
                    this->state_ = STATE_CLOSED;
                    this->device_->log("Open_Sensor: CLOSED", true, DEBUG_LEVEL_INFO);
                }
            }
            else
            {
                // TODO: add tampering detection if no calibration point matches;
                this->device_->log("Open_Sensor: TAMPERED state", true, DEBUG_LEVEL_ERROR);
                this->state_ = STATE_TAMPERED;
            }
        }

        // report for debugging
        // this->device_->log(String(filtered_mag_x));
        // this->device_->log("\t");
        // this->device_->log(String(filtered_mag_y));
        // this->device_->log("\t");
        // this->device_->log(String(filtered_mag_z), true);
    }
}

Open_Sensor_State Open_Sensor::get_state()
{
    return this->state_;
}

/**
 * determine if the sensor is open or closed:
 *  when the filtered magnetic field changes significantly,
 *  it means the sensor state changed and the sensor's state should be toggled.
 *
 * when becoming open, the magnetic field should drop significantly (all axes)
 *
 * when becoming closed, the magnetic field should increase significantly (some axes and primarily one axis).
 * the readings may fluctuate a bit when closed due to:
 *   - the door moving/vibrating slightly (e.g. weather stripping gap)
 *   - the magnetic field stabilizing
 *
 * once closed, the readings should be relatively stable but may still fluctuate a bit:
 * - due to the door moving from the weather stripping gap movement
 *   (someone pushing/pulling slightly on the door or just be wind/room pressure changes).
 * - due to the magnetic field changing over time (e.g. temperature changes) but that should be very slow change
 *   that the moving average filter should handle and "auto-calibrate" to the new value.
 *
 * we need a switch to closed "stabilizing" period or debounce time,
 * to trigger a closed state only if the magnetic field remains high for a certain period of time.
 *
 * same for open state, to avoid triggering open state on a brief magnetic field drop.
 *
 * TODO: implement open/close state detection with hysteresis and debounce time
 */

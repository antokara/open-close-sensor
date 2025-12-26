#include "Open_Sensor.h"

Open_Sensor::Open_Sensor(Device *device, Mag_Sensor *mag_sensor)
{
    this->device_ = device;
    this->mag_sensor_ = mag_sensor;
    this->xyzPointList_ = new XYZPointList(MAX_CALIBRATION_POINTS);

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

    bool updated = false;
    if (abs(this->old_filtered_mag_x_ - this->mag_x_filter_->get()) >= OPEN_SENSOR_DELTA_THRESHOLD)
    {
        updated = true;
        this->old_filtered_mag_x_ = this->mag_x_filter_->get();
    }
    if (abs(this->old_filtered_mag_y_ - this->mag_y_filter_->get()) >= OPEN_SENSOR_DELTA_THRESHOLD)
    {
        updated = true;
        this->old_filtered_mag_y_ = this->mag_y_filter_->get();
    }
    if (abs(this->old_filtered_mag_z_ - this->mag_z_filter_->get()) >= OPEN_SENSOR_DELTA_THRESHOLD)
    {
        updated = true;
        this->old_filtered_mag_z_ = this->mag_z_filter_->get();
    }

    if (this->state_ == STATE_UNKNOWN)
    {

        if (abs(0 - this->mag_x_filter_->get()) <= OPEN_THRESHOLD && abs(0 - this->mag_y_filter_->get()) <= OPEN_THRESHOLD &&
            abs(0 - this->mag_z_filter_->get()) <= OPEN_THRESHOLD)
        {
            this->device_->log("OPEN", true, DEBUG_LEVEL_INFO);
            this->state_ = STATE_OPEN;
        }
        else
        {
            this->device_->log("CLOSED", true, DEBUG_LEVEL_INFO);
            this->state_ = STATE_CLOSED;
        }
    }
    else if (this->state_ == STATE_OPEN)
    {

        if (abs(0 - this->mag_x_filter_->get()) > OPEN_THRESHOLD || abs(0 - this->mag_y_filter_->get()) > OPEN_THRESHOLD ||
            abs(0 - this->mag_z_filter_->get()) > OPEN_THRESHOLD)
        {
            this->device_->log("CLOSING", true, DEBUG_LEVEL_INFO);
            this->state_ = STATE_CLOSING;
            // TODO: mark the magentic field change during closing, to know when the door moves due to the gap, if that's normal or tampering
            // TODO: use the time to time out the closing event if it takes too long
            // TODO: handle ajar state with opening/closing timeout;
        }
    }
    else if (this->state_ == STATE_CLOSING)
    {

        if (abs(0 - this->mag_x_filter_->get()) > CLOSE_THRESHOLD || abs(0 - this->mag_y_filter_->get()) > CLOSE_THRESHOLD ||
            abs(0 - this->mag_z_filter_->get()) > CLOSE_THRESHOLD)
        {
            this->device_->log("CLOSED", true, DEBUG_LEVEL_INFO);
            this->state_ = STATE_CLOSED;
        }
    }
    else if (this->state_ == STATE_CLOSED)
    {

        if (abs(0 - this->mag_x_filter_->get()) <= CLOSE_THRESHOLD && abs(0 - this->mag_y_filter_->get()) <= CLOSE_THRESHOLD &&
            abs(0 - this->mag_z_filter_->get()) <= CLOSE_THRESHOLD)
        {
            this->device_->log("OPENING", true, DEBUG_LEVEL_INFO);
            this->state_ = STATE_OPENING;
        }
    }
    else if (this->state_ == STATE_OPENING)
    {

        if (abs(0 - this->mag_x_filter_->get()) <= OPEN_THRESHOLD && abs(0 - this->mag_y_filter_->get()) <= OPEN_THRESHOLD &&
            abs(0 - this->mag_z_filter_->get()) <= OPEN_THRESHOLD)
        {
            this->device_->log("OPEN", true, DEBUG_LEVEL_INFO);
            this->state_ = STATE_OPEN;
        }
    }

    if (updated)
    {
        if (this->device_->is_calibrating())
        {
        }

        // report for debugging
        this->device_->log(String(this->mag_x_filter_->get()));
        this->device_->log("\t");
        this->device_->log(String(this->mag_y_filter_->get()));
        this->device_->log("\t");
        this->device_->log(String(this->mag_z_filter_->get()), true);
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

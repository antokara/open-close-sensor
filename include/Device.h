#pragma once
#include <Arduino.h>
#include "Adafruit_FreeTouch.h"

/**
 * has fast to be blinking the LED,
 * when in fault state
 */
#define FAULT_LED_BLINK_INTERVAL_MS 250

/**
 * delay in ms after Serial.begin() before using Serial
 * to allow time for serial monitor to connect
 */
#define SETUP_DELAY_MS 2500

/**
 * 0 = no debug messages, higher numbers = more verbose
 */
#define DEBUG_LEVEL DEBUG_LEVEL_VERBOSE

/**
 * pin used for calibration input (e.g. button or touch sensor)
 */
#define CALIBRATION_INPUT_PIN A0

/**
 * touch threshold for capacitive input
 */
#define TOUCH_THRESHOLD 800

/**
 * time in ms the calibration input must be held
 * to enter calibration mode
 */
#define CALIBRATION_HOLD_TIME_MS 2000

enum Debug_Level
{
    DEBUG_LEVEL_NONE = 0,
    DEBUG_LEVEL_ERROR = 1,
    DEBUG_LEVEL_INFO = 2,
    DEBUG_LEVEL_VERBOSE = 3
};

class Device
{
private:
    /**
     * true if device is in fault state
     */
    bool has_fault_{false};

    /**
     * timestamp of last led toggle
     */
    uint32_t last_led_toggle_{0};

    /**
     * check and handle fault state (e.g. blink LED)
     */
    void check_fault();

    /**
     * true if device is in calibration mode
     */
    bool is_calibrating_{false};

    /**
     * Adafruit_FreeTouch instance for the calibration input
     */
    Adafruit_FreeTouch qtCalibration_;

    /**
     * check if calibration button is pressed
     */
    void check_calibration_button();

    /**
     * timestamp when calibration button was first pressed
     *
     * 0 if not currently pressed
     * -1 if pressed and toggled but not yet released
     */
    u_int32_t calibration_button_pressed_time_{0};

    /**
     * boolean flag indicating if calibration was toggled
     * since the last button press, to prevent retriggering
     */
    bool calibration_toggled_{false};

public:
    Device();

    /**
     * initialize device (e.g. pin modes).
     * Should be called in Arduino setup()
     */
    void setup();

    /**
     * process device state (e.g. blink LED if in fault state).
     * should be called in Arduino loop()
     */
    void loop();

    /**
     * returns true if device is in fault state
     */
    bool has_fault();

    /**
     * set or clear fault state
     */
    void set_fault(bool fault);

    /**
     * log message to serial if level >= DEBUG_LEVEL
     */
    void log(const String &message, bool ln = true, Debug_Level level = DEBUG_LEVEL_INFO);

    /**
     * start calibration mode
     */
    void start_calibration();

    /**
     * stop calibration mode
     */
    void stop_calibration();
};
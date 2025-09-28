#pragma once
#include <Arduino.h>

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
};
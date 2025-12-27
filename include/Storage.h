#pragma once
#include <Arduino.h>
#include <FlashStorage_SAMD.h>

class Storage
{
private:
    /** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
    int address = 0;

public:
    Storage();

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
};
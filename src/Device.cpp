#include "Device.h"

Device::Device() {}

void Device::setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Start serial communication at 115200 baud
    Serial.begin(115200);

    // Wait for serial monitor to open
    delay(SETUP_DELAY_MS);
}

void Device::loop()
{
    if (this->has_fault_)
    {
        uint32_t now = millis();
        if (now - this->last_led_toggle_ >= FAULT_LED_BLINK_INTERVAL_MS)
        {
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            this->last_led_toggle_ = now;
        }
    }
}

bool Device::has_fault()
{
    return this->has_fault_;
}

void Device::set_fault(bool fault)
{
    this->has_fault_ = fault;
    if (!fault)
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

#include "Device.h"

Device::Device() {}

void Device::setup()
{

    // Start serial communication at 115200 baud
    Serial.begin(115200);

    // Wait for serial monitor to open
    delay(SETUP_DELAY_MS);

    pinMode(LED_BUILTIN, OUTPUT);

    // setup the calibration input pin
    this->qtCalibration_ = Adafruit_FreeTouch(CALIBRATION_INPUT_PIN, OVERSAMPLE_4, RESISTOR_0, FREQ_MODE_NONE);

    if (!this->qtCalibration_.begin())
    {
        this->log("Failed to start touch sensor for the Calibration Input", true, DEBUG_LEVEL_ERROR);
    }
}

void Device::loop()
{
    this->check_fault();
    this->check_calibration_button();
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
        digitalWrite(LED_BUILTIN, HIGH);
    }
}

void Device::check_fault()
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

void Device::log(const String &message, bool ln, Debug_Level level)
{
    if (level <= DEBUG_LEVEL)
    {
        if (ln)
            Serial.println(message);
        else
            Serial.print(message);
    }
}

void Device::start_calibration()
{
    is_calibrating_ = true;
    digitalWrite(LED_BUILTIN, LOW);
    this->log("Entered calibration mode", true, DEBUG_LEVEL_INFO);
}

void Device::stop_calibration()
{
    is_calibrating_ = false;
    digitalWrite(LED_BUILTIN, HIGH);
    this->log("Exited calibration mode", true, DEBUG_LEVEL_INFO);
}

void Device::check_calibration_button()
{
    if (this->qtCalibration_.measure() >= TOUCH_THRESHOLD)
    {
        if (this->calibration_button_pressed_time_ != 0 && !this->calibration_toggled_)
        {
            // button held down
            if (millis() - this->calibration_button_pressed_time_ >= CALIBRATION_HOLD_TIME_MS)
            {
                // long enough to toggle calibration state
                if (!this->is_calibrating_)
                {
                    this->start_calibration();
                }
                else
                {
                    this->stop_calibration();
                }
                this->calibration_toggled_ = true;
            }
        }
        else if (this->calibration_button_pressed_time_ == 0)
        {
            // first press
            this->calibration_button_pressed_time_ = millis();
        }
    }
    else if (this->calibration_button_pressed_time_ != 0)
    {
        // not pressed
        this->calibration_button_pressed_time_ = 0;
        this->calibration_toggled_ = false;
    }
}

bool Device::is_calibrating()
{
    return this->is_calibrating_;
}
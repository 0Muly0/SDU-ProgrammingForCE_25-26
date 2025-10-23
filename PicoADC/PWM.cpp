#include "PWM.h"
#include <iostream>
using namespace std;

PWM::PWM(uint gpioPin, bool inverted)
    : _gpioPin(gpioPin), _inverted(inverted),
      _frequency(1000.0f), _duty(50.0f),
      _divider(1.0f), _wrap(0), _enabled(false)
{
    gpio_set_function(_gpioPin, GPIO_FUNC_PWM);
    _sliceNum = pwm_gpio_to_slice_num(_gpioPin);
    configurePWM();
    applyDuty();
    enable(true);
}

// -------------------------------------------------------------
// Set frequency and keep duty cycle consistent
// -------------------------------------------------------------
void PWM::setFrequency(float freqHz) {
    if (freqHz < 1) freqHz = 1;
    _frequency = freqHz;

    const float clockFreq = 125000000.0f;
    const uint16_t maxWrap = 65535;

    for (_divider = 1.0f; _divider <= 255.0f; _divider += 0.5f) {
        float wrapCalc = (clockFreq / (_frequency * _divider)) - 1.0f;
        if (wrapCalc <= maxWrap) {
            _wrap = (uint16_t)wrapCalc;
            break;
        }
    }

    configurePWM();
    applyDuty();  // Reapply duty automatically after frequency change
}

// -------------------------------------------------------------
// Set duty cycle independently (no need to re-set frequency)
// -------------------------------------------------------------
void PWM::setDutyCycle(float dutyPercent) {
    if (dutyPercent < 0) dutyPercent = 0;
    if (dutyPercent > 100) dutyPercent = 100;

    _duty = dutyPercent;
    applyDuty();
}

// -------------------------------------------------------------
// Apply duty to hardware
// -------------------------------------------------------------
void PWM::applyDuty() {
    float duty = _duty;
    if (_inverted) duty = 100.0f - duty;

    uint16_t level = (uint16_t)((duty / 100.0f) * _wrap);
    pwm_set_gpio_level(_gpioPin, level);
}

// -------------------------------------------------------------
void PWM::enable(bool on) {
    _enabled = on;
    pwm_set_enabled(_sliceNum, _enabled);
}

// -------------------------------------------------------------
void PWM::configurePWM() {
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, _divider);
    pwm_config_set_wrap(&config, _wrap);
    pwm_init(_sliceNum, &config, _enabled);
}

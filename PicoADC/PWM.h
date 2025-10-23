#ifndef PWM_H
#define PWM_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <cstdint>

class PWM {
public:
    explicit PWM(uint gpioPin, bool inverted = false);

    void setFrequency(float freqHz);      // Set PWM frequency in Hz
    void setDutyCycle(float dutyPercent); // Set duty cycle in %
    void enable(bool on);                 // Turn PWM on/off

private:
    uint _gpioPin;       // GPIO number
    uint _sliceNum;      // PWM slice
    bool _inverted;      // Invert output (for buzzer)
    float _frequency;    // Current frequency in Hz
    float _duty;         // Current duty cycle in %
    float _divider;      // Clock divider
    uint16_t _wrap;      // Counter wrap value
    bool _enabled;       // PWM enabled state

    void configurePWM(); // Helper to configure frequency and divider
    void applyDuty();    // Helper to apply duty safely
};

#endif

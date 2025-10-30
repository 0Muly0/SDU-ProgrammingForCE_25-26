// =========================================================================
// File: ADC.h
// Purpose: Declaration of ADC base class for analog-to-digital conversion
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#ifndef ADC_H
#define ADC_H

#include "pico/stdlib.h"                                                    // Pico SDK base definitions

// -----------------------------------------------------------------------------
// Class: ADC
// -----------------------------------------------------------------------------
// Base class that provides access to the RP2040 ADC hardware.
// Handles initialization, raw readings, and averaged readings.
// Used as superclass for sensor-specific classes (e.g., Pot, LDR, DieTemp).
// -----------------------------------------------------------------------------
class ADC {
public:
    ADC(uint gpioPin);                                                      // Constructor (defines ADC input pin)
    int  read();                                                            // Reads one raw 12-bit ADC value
    int  readAvg(int avgVal);                                               // Returns averaged ADC reading

protected:
    uint _pin;                                                              // GPIO pin number for ADC input
    uint _channel;                                                          // ADC channel number
    void init();                                                            // Initializes ADC hardware and channel
};

#endif // ADC_H

// =========================================================================
// File: LightSensorLDR.h
// Purpose: Declaration of LightSensorLDR class derived from ADC
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#ifndef LIGHTSENSORLDR_H
#define LIGHTSENSORLDR_H

#include "ADC.h"                                                           // Base ADC class

// -----------------------------------------------------------------------------
// Class: LightSensorLDR
// -----------------------------------------------------------------------------
// Derived from the ADC base class.
// Represents the PicoBricks light sensor (LDR) connected to GPIO28 (ADC2).
// Provides a method to convert the ADC reading into an approximate illuminance
// value in lux based on the LDR characteristics and voltage divider formula.
// -----------------------------------------------------------------------------
class LightSensorLDR : public ADC {
public:
    LightSensorLDR();                                                      // Constructor (fixed to GPIO28)
    float readLux();                                                       // Returns approximate light intensity in lux

private:
    const float Rfixed = 10000.0f;                                         // Fixed resistor in voltage divider (Ω)
};

#endif // LIGHTSENSORLDR_H


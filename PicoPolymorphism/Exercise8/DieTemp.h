// =========================================================================
// File: DieTemp.h
// Purpose: Declaration of DieTemp class derived from ADC
// Author: Lucia Famoso
// Date: 2025-10-25
// =========================================================================

#ifndef DIET_H
#define DIET_H

#include "ADC.h"                                                           // Base ADC class

// -----------------------------------------------------------------------------
// Class: DieTemp
// -----------------------------------------------------------------------------
// Derived from the ADC base class.
// Represents the PicoBricks temperature sensor physically connected to (ADC4), 
// which is accessed by enabling the internal sensor and then performing a 
// normal ADC read. GPIO30 is not a real pin: It is used only as a symbolic mapping 
// to reuse the ADC class infrastructure.
// Provides a convenient method for reading the temperture sensor value as both
// celsius and fahrenheit.
// -----------------------------------------------------------------------------
class DieTemp : public ADC {
public:
    DieTemp();                                                             // Constructor (fixed to GPIO30)
    float readCelsius();                                                   // Returns temperature value in celsius
    float readFahrenheit();                                                // Returns temperature value in fahrenheit
};

#endif // DIET_H
// =========================================================================
// File: Pot.cpp
// Purpose: Implementation of Pot class derived from ADC
// Author: Lucia Famoso
// Date: 2025-10-30
// =========================================================================

#include "Pot.h"                                                        // Class declaration

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
// Calls the ADC base-class constructor withthe potentiometer fixed pin GPIO26.
// This setup matches the PicoBricks hardware layout.
// -----------------------------------------------------------------------------
Pot::Pot()
    : ADC(26) { };                                                      // PicoBricks potentiometer on GPIO26

// -----------------------------------------------------------------------------
// Function: isPressed()
// -----------------------------------------------------------------------------
// Provides a direct query for the average value read by the potentiometer and
// converted by the ADC. Internally reuses the ADC base class method readAvg()
// to extract the average of 5 potentiometer reads and return it as a float.
// -----------------------------------------------------------------------------
float Pot::readPercent() {
    int raw = readAvg(5);
    float normVal = static_cast<float>(raw) * 100.0f / 4095.0f;

    return normVal;
}
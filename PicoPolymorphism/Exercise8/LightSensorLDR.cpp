// =========================================================================
// File: LightSensorLDR.cpp
// Purpose: Implementation of LightSensorLDR class derived from ADC
// Author: Lucia Famoso
// Date: 2025-10-30
// =========================================================================

#include <cmath>
#include "LightSensorLDR.h"                                             // Class declaration

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
// Calls the ADC base-class constructor withthe light sensor fixed pin GPIO27.
// This setup matches the PicoBricks hardware layout.
// -----------------------------------------------------------------------------
LightSensorLDR::LightSensorLDR()
    : ADC(27) { };                                                      // PicoBricks light sensor on GPIO27

// -----------------------------------------------------------------------------
// Function: readLux()
// -----------------------------------------------------------------------------
// Provides a direct query for the average value read by the light sensor and
// converted by the ADC. Internally reuses the ADC base class method readAvg()
// to extract the average of 5 light sensor reads and return it as a float.
// -----------------------------------------------------------------------------
float LightSensorLDR::readLux() {
    int raw = readAvg(5);
    float rLDR = 10000.0f * (raw / (4095.0f - raw));

    const float A = 2.0e6f;
    const float gamma = 1.4f;
    float lux = powf((A / rLDR), 1.0f / gamma);

    return lux;
}
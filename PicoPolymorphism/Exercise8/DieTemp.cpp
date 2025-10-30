// =========================================================================
// File: DieTemp.cpp
// Purpose: Implementation of DieTemp class derived from ADC
// Author: Lucia Famoso
// Date: 2025-10-30
// =========================================================================

#include <hardware/adc.h>
#include "DieTemp.h"                                                     // Class declaration

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
// Calls the ADC base-class constructor withthe potentiometer fixed pin GPIO26.
// This setup matches the PicoBricks hardware layout.
// -----------------------------------------------------------------------------
DieTemp::DieTemp()
    : ADC(30) {                                  // PicoBricks potentiometer on GPIO30
        adc_set_temp_sensor_enabled(true);       // Physically in the board
    };                                                     

// -----------------------------------------------------------------------------
// Function: isPressed()
// -----------------------------------------------------------------------------
// Provides a direct query for the average value read by the potentiometer and
// converted by the ADC. Internally reuses the ADC base class method readAvg()
// to extract the average of 5 potentiometer reads and return it as a float.
// -----------------------------------------------------------------------------
float DieTemp::readCelsius() {
    adc_set_temp_sensor_enabled(true); 
    uint16_t raw = readAvg(20);

    const float Vconv = 3.3f / 4095.0f;
    float voltage = raw * Vconv;

    float tempC = 27.0f - (voltage - 0.706f) / 0.001721f;
    return tempC;
}

float DieTemp::readFahrenheit() {
    return readCelsius() * 9.0f / 5.0f + 32.0f;
}
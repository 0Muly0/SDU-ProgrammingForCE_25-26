// =========================================================================
// File: MyApp.cpp
// Purpose: Implementation of MyApp class for PicoBricks sensor system
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#include "MyApp.h"
#include <iostream>
#include <iomanip>                                                          // For formatted console output
#include "pico/stdlib.h"                                                    // Pico SDK base functions
#include "dbop.h"                                                           // Debug macros (C_MyApp, etc.)

using namespace std;

// -----------------------------------------------------------------------------
// Constructor: initializes middleware components
// -----------------------------------------------------------------------------
MyApp::MyApp() {
    cout << "EXERCISE 8A-8D: Sensor readings triggered by PicoBrickKeyPress"
         << endl;                                                           C_MyApp("MyApp constructor called");
}

#define AlternativeTerminalPrintout

#ifndef AlternativeTerminalPrintout
// -----------------------------------------------------------------------------
// run(): main application loop
// -----------------------------------------------------------------------------
void MyApp::run() {
    while (true) {
        if (keyPress.isPressed()) {                                         C_MyApp("KeyPress event detected");

            // Read sensor values
            float potPercent = pot.readPercent();                           C_MyApp("Read POT percent value");
            float dieTempC   = dieTemp.readCelsius();                       C_MyApp("Read MCU die temperature (Celsius)");
            float lightLux   = lightSensor.readLux();                       C_MyApp("Read LDR light sensor (lux)");

            // Console output
            cout << "----------------------------------------------" << endl;

            cout << left  << setw(28) << "  POT Percent value:"
                 << right << setw(8)  << fixed << setprecision(1)
                 << potPercent << " %" << endl;                             C_MyApp("Print POT reading");

            cout << left  << setw(28) << "  LDR Light Sensor value:"
                 << right << setw(8)  << fixed << setprecision(1)
                 << lightLux << " lux" << endl;                             C_MyApp("Print LDR reading");

            cout << left  << setw(28) << "  MCU Die Temperature:"
                 << right << setw(8)  << fixed << setprecision(1)
                 << dieTempC << " Degree C" << endl;                        C_MyApp("Print DieTemp reading");
        }

        sleep_ms(10);                                                       // Small delay to avoid excessive polling
    }
}
#endif

#ifdef AlternativeTerminalPrintout
// -----------------------------------------------------------------------------
// run(): main application loop
// -----------------------------------------------------------------------------
void MyApp::run() {
    while (true) {
        {                                                                   C_MyApp("Loop for Readings");

            // Read sensor values
            float potPercent = pot.readPercent();                           C_MyApp("Read POT percent value");
            float dieTempC   = dieTemp.readCelsius();                       C_MyApp("Read MCU die temperature (Celsius)");
            float lightLux   = lightSensor.readLux();                       C_MyApp("Read LDR light sensor (lux)");

            // Console output
            cout << "  POT Percent value:"
                 << setprecision(1)
                 << potPercent << " %"                             

                 << "  LDR Light Sensor value:"
                 << setprecision(1)
                 << lightLux << " lux"                            

                 << "  MCU Die Temperature:"
                 << setprecision(1)
                 << dieTempC << " Degree C" 
                 
                 << endl;                       
        }

        sleep_ms(500);                                                       // Medium delay to avoid excessive polling
    }
}
#endif

// // =========================================================================
// // File: MyApp.h
// // Purpose: Declaration of the main application function for PicoBricks system
// // Author: Tommy Nielsen
// // Date: 2025-10-25
// // =========================================================================

// #ifndef MYAPP_H
// #define MYAPP_H

// #include "dbop.h"                                                           // Debug macros (C_MyApp, etc.)
// #include "PushButton.h"                                                     // Generic Button class

// // -----------------------------------------------------------------------------
// // Function: MyApp()
// // -----------------------------------------------------------------------------
// // Entry point for EXERCISE 8A–8D demonstration.
// // Executes the main loop that reads PicoBricks sensor data when the KeyPress
// // button is pressed. Used in combination with MyApp.cpp implementation.
// // -----------------------------------------------------------------------------
// void MyApp();

// #endif // MYAPP_H


// =========================================================================
// File: MyApp.h
// Purpose: Class definition for PicoBricks application
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#ifndef MYAPP_H
#define MYAPP_H

#include "KeyPress.h"
#include "Pot.h"
#include "LightSensorLDR.h"
#include "DieTemp.h"

// -----------------------------------------------------------------------------
// MyApp class: Application layer entry point
// -----------------------------------------------------------------------------
// Represents the main PicoBricks application that interacts with middleware
// sensor classes and handles formatted console output.
// -----------------------------------------------------------------------------
class MyApp {
public:
    MyApp();           // Constructor
    void run();        // Main execution loop

private:
    KeyPress        keyPress;       // PicoBricks button
    Pot             pot;            // Potentiometer
    LightSensorLDR  lightSensor;    // LDR sensor
    DieTemp         dieTemp;        // Internal MCU temperature sensor
};

#endif // MYAPP_H

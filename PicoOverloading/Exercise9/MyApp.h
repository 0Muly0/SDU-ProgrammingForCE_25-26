//=========================================================================
//  MyApp.h                                                                
//  Application class combining OLED display and LDR sensor.               
//=========================================================================

#ifndef MYAPP_H
#define MYAPP_H

//-------------------------------------------------------------------------
//  SELECT MODE: uncomment FUNCTION_OVERLOAD to use function overloading.  
//-------------------------------------------------------------------------
// #define FUNCTION_OVERLOAD                                                  // Comment this line to use templates.
//-------------------------------------------------------------------------

#include "OLEDDisplay.h"
#include "LightSensorLDR.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>

//-------------------------------------------------------------------------
//  MyApp class: handles sensor reading and display output.                
//-------------------------------------------------------------------------
class MyApp {
public:
    MyApp();                                                               // Constructor
    void run();                                                            // Main loop

#ifdef FUNCTION_OVERLOAD
    std::string formatLux(float value);                                    // Overload for float
    std::string formatLux(uint16_t value);                                 // Overload for uint16_t
#else
    template <typename T>
    std::string formatLux(T value) {                                       // Template for both float & int
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        std::string str = oss.str();
        str += " LUX";
        return str;
    }
#endif

private:
    LightSensorLDR ldr;                                                    // Light sensor instance
    OLEDDisplay display;                                                   // OLED display instance
};

#endif

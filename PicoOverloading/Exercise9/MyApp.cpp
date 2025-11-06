//=========================================================================
//  MyApp.cpp                                                              
//  Implementation of MyApp class using LDR sensor and OLED display.       
//=========================================================================

#include "MyApp.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

//-------------------------------------------------------------------------
//  Constructor: initializes I2C and OLED display.                         
//-------------------------------------------------------------------------
MyApp::MyApp()
    : ldr(),                                                               // Initialize LDR sensor (GPIO27)
      display(i2c_default, 0x3C, 128, 32)                                  // OLED: 128x32 @ I²C 0x3C
{
    stdio_init_all();                                                      // Initialize stdio
    i2c_init(i2c_default, 400 * 1000);                                     // I²C at 400 kHz
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);            // SDA pin
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);            // SCL pin
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);                                // Enable pull-ups
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    display.init();                                                        // Initialize display
    display.clear();                                                       // Clear screen
}

//-------------------------------------------------------------------------
//  FUNCTION OVERLOADING IMPLEMENTATIONS                                   
//-------------------------------------------------------------------------
#ifdef FUNCTION_OVERLOAD

std::string MyApp::formatLux(float value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    std::string str = oss.str();
    str += " LUX";
    return str;
}

//-------------------------------------------------------------------------
//  TODO: Implement the uint16_t version of formatLux().                   
//  Use the float version above as inspiration.                            
//-------------------------------------------------------------------------
std::string MyApp::formatLux(uint16_t value) {                                                  
    // Hints:                                                             
    // 1. Create an output stream (std::ostringstream). 
    std::ostringstream oss;                  
    // 2. Insert the integer 'value' into the stream.  
    oss << value;                    
    // 3. Convert the stream into a string using oss.str().  
    std::string str = oss.str();             
    // 4. Append the unit " LUX" to the string.   
    str += " LUX";                        
    // 5. Return the completed string below.                                                              
    return str;                                                          
}
#endif

//-------------------------------------------------------------------------
//  Main loop: reads LDR, converts, and shows Lux values.                  
//-------------------------------------------------------------------------
void MyApp::run() {
    while (true) {
        float lux = ldr.readLux();                                         // Read light as float
        uint16_t luxInt = static_cast<uint16_t>(lux);                      // Integer version

        display.clear();                                                   // Clear OLED buffer
        display.writeText(5, 0, "LIGHT SENSOR");                           // Title
        display.writeText(5, 8, formatLux(lux).c_str());                   // Float format
        display.writeText(5, 16, formatLux(luxInt).c_str());               // Integer format
        display.render();                                                  // Render to OLED

        sleep_ms(1000);                                                    // Wait 1 second
    }
}

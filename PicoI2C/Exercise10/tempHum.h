#ifndef TEMP_HUM_SENSOR_H
#define TEMP_HUM_SENSOR_H

#include <stdlib.h>            // Standard utilities
#include <cstdint>
#include "stdio.h"             // Standard I/O operations
#include "pico/stdlib.h"
#include "hardware/i2c.h"      // I²C interface

class SHTC3 {
public:
    SHTC3(i2c_inst_t* i2c, uint8_t addr);  // Constructor with default I2C address
    void wakeUp();                         // Wake the sensor
    void sleep();                          // Put the sensor to sleep
    float readTemperature();               // Return temperature in °C
    float readHumidity();                  // Return humidity percentage

private:
    i2c_inst_t* _i2c;                            // I²C instance (i2c0 / i2c1)
    uint8_t _addr;                               // I2C address

    void sendCommand(uint8_t high, uint8_t low); // Send a command
    void readData(uint8_t *data, uint8_t len);   // Read data from sensor
};

#endif
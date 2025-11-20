#ifndef MOTOR_H
#define MOTOR_H

#include <stdlib.h>            // Standard utilities
#include <cstdint>
#include "stdio.h"             // Standard I/O operations
#include "pico/stdlib.h"
#include "hardware/i2c.h"      // I²C interface

class motorDriver {
public:
    motorDriver(i2c_inst_t* i2c, uint8_t addr);         // Constructor
    void dc(int num, int speed, int direction);         // Control DC motor (number, speed 0-255, direction 0/1)
    void servo(int num, int angle);                     // Control servo motor (number 1-4, angle 0-180)
private: 
    i2c_inst_t* _i2c;
    uint8_t _addr; 
};

#endif
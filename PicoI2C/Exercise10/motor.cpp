/*****************************
MOTOR DRIVER LIBRARY
******************************/
#include "motor.h"

motorDriver::motorDriver(i2c_inst_t* i2c, uint8_t addr) 
  : _i2c(i2c), _addr(addr) {}

/**
 * Function to control a DC motor (1 or 2) with a given speed (0-255) and direction (0 or 1)
 * num: 1 or 2 (selects motor)
 * speed: 0 to 255 (controls the motor speed)
 * direction: 0 for one direction, 1 for the opposite direction
 */
void motorDriver::dc(int num, int speed, int direction) { 
  uint8_t buf[4];
  buf[0] = num;
  buf[1] = speed, 
  buf[2] = direction, 
  buf[3] = num ^ speed ^ direction;
  i2c_write_blocking(_i2c, _addr, buf, 4, false);
}

/**
 * Function to control a servo motor (1 to 4) with a given angle (0-180)
 * num: 1, 2, 3, or 4 (selects servo motor)
 * angle: 0 to 180 (controls the servo angle)
 */
void motorDriver::servo(int num, int angle) { 
  uint8_t buf[5];
  buf[0] = 0x26;
  buf[1] = num + 2;
  buf[2] = 0x00;
  buf[3] = angle;
  buf[4] = (num + 2) ^ angle;
  i2c_write_blocking(_i2c, _addr, buf, 5, false);
}
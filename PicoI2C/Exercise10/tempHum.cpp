/*****************************
SHTC3 LIBRARY
******************************/
#include "tempHum.h"
#include "font.h"

SHTC3::SHTC3(i2c_inst_t* i2c, uint8_t addr) 
    : _i2c(i2c), _addr(addr) {}

/**
 * Sends a command to the SHTC3 sensor.
 * @param high The high byte of the command.
 * @param low The low byte of the command.
 */
void SHTC3::sendCommand(uint8_t high, uint8_t low) {
    uint8_t buf[2] = { high, low }; 
    i2c_write_blocking(_i2c, _addr, buf, 2, false);
}   

/**
 * Reads data from the SHTC3 sensor.
 * @param data Pointer to the buffer where the data will be stored.
 * @param len The number of bytes to read.
 */
void SHTC3::readData(uint8_t *data, uint8_t len) {
    i2c_read_blocking(_i2c, _addr, data, len, false);
}

/**
 * Wakes up the SHTC3 sensor to begin measurement.
 * Sends the appropriate wake-up command and waits for the sensor to be ready.
 */
void SHTC3::wakeUp() {
    sendCommand(0x35, 0x17);    // Wake up command (start measurement)
    sleep_ms(500);              // Delay to allow sensor to wake up
}

/**
 * Puts the SHTC3 sensor to sleep to conserve power.
 * Sends the sleep command to turn off the sensor's measurement functions.
 */
void SHTC3::sleep() {
    sendCommand(0xB0, 0x64); // Sleep command
}

/**
 * Reads the temperature value from the SHTC3 sensor.
 * The temperature is in Celsius and is calculated based on the sensor's raw data.
 * @return The temperature in Celsius.
 */
float SHTC3::readTemperature() {
    uint8_t data[6];
    sendCommand(0x78, 0x66);    // Start measurement for temperature
    sleep_ms(500);              // Wait for the measurement
    readData(data, 6);          // Read the 6 bytes of data (2 for temperature, 2 for humidity)
    
    uint16_t rawTemp = (data[0] << 8) | data[1];        // Combine the 2 bytes
    rawTemp = rawTemp & 0xFFFC;                         // Mask the last two bits (for the CRC check)
    return (((4375 * rawTemp) >> 14) - 4500) / 100.0;   // Convert raw data to temperature in Celsius based on sensor's formula
}

/**
 * Reads the humidity value from the SHTC3 sensor.
 * The humidity is in percentage and is calculated based on the sensor's raw data.
 * @return The humidity in percentage.
 */
float SHTC3::readHumidity() {
    uint8_t data[6];
    sendCommand(0x78, 0x66);    // Start measurement for humidity
    sleep_ms(100);              // Wait for the measurement
    readData(data, 6);          // Read the 6 bytes of data (2 for temperature, 2 for humidity)

    uint16_t rawHumidity = (data[3] << 8) | data[4];    // Combine the 2 bytes for humidity
    rawHumidity = rawHumidity & 0xFFFC;                 // Mask the last two bits (for the CRC check)
    return ((625 * rawHumidity) >> 12) / 100.0;         // Convert raw humidity data to percentage based on sensor's formula
}

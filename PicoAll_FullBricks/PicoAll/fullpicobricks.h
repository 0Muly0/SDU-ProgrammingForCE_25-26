#ifndef FULL_H
#define FULL_H
        
#include <stdlib.h>             // Standard cpp utilities
#include "pico/stdlib.h"        // pico cpp utilities
#include <cstdint>              // Standard int types
#include <iostream>
#include "stdio.h"              // Standard I/O operations
#include <cmath>                // Standard math
#include "hardware/i2c.h"       // I²C interface
#include "hardware/pwm.h"       // PWM
#include "ws2812.pio.h"         // Custom PIO program for RGBLedDriver (WS2812)
#include "dbop.h"               // Debug Operations
#include "ADC.h"

/* Button */
#define MAX_GPIO     32   // max number of GPIOs we support
#define DEBOUNCE_MS  50   // debounce time in ms

class Button {
private:
    uint pin;                  // GPIO pin
    uint32_t edge;             // edge type for interrupt
    volatile int pressCount;   // antal validerede tryk
    volatile bool toggleStateValue;

    alarm_id_t debounceAlarm;  // alarm id til debounce
    volatile bool lastState;
    volatile bool eventFlag;   // event-flag til hasEvent()

    static Button* instances[MAX_GPIO];  // array over instanser pr. pin

    static void gpio_isr(uint gpio, uint32_t events);           // statisk ISR
    void startDebounce();                                       // start debounce timer
    static int64_t debounceTimerCallback(alarm_id_t id, void*); // statisk timer callback
    int64_t handleDebounce();                                   // tjek tilstand efter debounce

public:
    Button(uint pinNumber, uint32_t edgeType);  // constructor

    int getPressCount() const;    // returns how many times the button has been pressed
    bool isPressed() const;       // returns wether the button is pressed or not
    bool toggleState() const;     // returns toggle state
    bool hasEvent();              // true if the button went through a debounce
};

/* Single-color LED */
class Led {
private:
    uint pin;       // GPIO pin for the LED
    bool state;     // Current LED state
public:
    Led(uint pinNumber);       // Constructor – initializes the LED GPIO
    void on();                 // Turn LED ON
    void off();                // Turn LED OFF
    void toggle();             // Toggle LED state
    void setState(bool s);     // Set LED state directly
    bool isOn() const;         // Return current LED state
};

/* RGB LED */
class RGBLedDriver {
public:
    RGBLedDriver(uint8_t  pinNumber, uint16_t numberOfPixels);                    // Constructor using default PIO
    RGBLedDriver(uint8_t  pinNumber, uint16_t numberOfPixels, PIO pio, uint sm);  // Constructor using specific PIO and state machine
    virtual ~RGBLedDriver(){};                                                    // Destructor

    void Init(uint8_t  pinNumber, uint16_t numberOfPixels);                   // Initialize LED strip
    void setPixelColor(uint16_t pixel_number, uint8_t r=0, uint8_t g=0, uint8_t b=0); // Set specific LED color
    void Fill(uint8_t r=0, uint8_t g=0, uint8_t b=0);                         // Fill entire strip with a color
    void Show(void);                                                          // Push buffer to LEDs

private:
    PIO pixelPio;                           // PIO to use
    uint pixelOffset;                       // Offset for the PIO program
    uint pixelSm;                           // PIO state machine
    uint16_t actual_number_of_pixels;       // Number of LEDs
    uint8_t pixelBuffer[1024][3];           // RGB values for each LED

    uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);  // Convert RGB to 24-bit GRB format
    void putPixel(uint32_t pixel_grb);                   // Send GRB pixel to PIO
};

/* Buzzer */
class PWM {
public:
    explicit PWM(uint gpioPin, bool inverted = false);

    void setFrequency(float freqHz);      // Set PWM frequency in Hz
    void setDutyCycle(float dutyPercent); // Set duty cycle in %
    void enable(bool on);                 // Turn PWM on/off

private:
    uint _gpioPin;       // GPIO number
    uint _sliceNum;      // PWM slice
    bool _inverted;      // Invert output (for buzzer)
    float _frequency;    // Current frequency in Hz
    float _duty;         // Current duty cycle in %
    float _divider;      // Clock divider
    uint16_t _wrap;      // Counter wrap value
    bool _enabled;       // PWM enabled state

    void configurePWM(); // Helper to configure frequency and divider
    void applyDuty();    // Helper to apply duty safely
};

/* Relay */
class RelayDriver {
public:
    RelayDriver(uint8_t pinNumber);
    void activate();
    void deactivate();
private:
    uint8_t pinNumber;
};

/* Potentiometer */
class Potentiometer : public ADC {
public:
    Potentiometer();                                // Constructor (fixed to GPIO26)
    float readPercent();                            // Returns potentiometer value in percent (0–100 %)

private:
    float _normVal;                                 // Stores last normalized reading
};

/* Temperature and humidity sensor */
class TempHumDriver {
public:
    TempHumDriver(i2c_inst_t* i2c, uint8_t addr);       // Constructor with default I2C address
    void wakeUp();                                      // Wake the sensor
    void sleep();                                       // Put the sensor to sleep
    float readTemperature();                            // Return temperature in °C
    float readHumidity();                               // Return humidity percentage

private:
    i2c_inst_t* _i2c;                                   // I²C instance (i2c0 / i2c1)
    uint8_t _addr;                                      // I2C address

    void sendCommand(uint8_t high, uint8_t low);        // Send a command
    void readData(uint8_t *data, uint8_t len);          // Read data from sensor
};

/* Light sensor */
class LightSensorLDR : public ADC {
public:
    LightSensorLDR();                                                      // Constructor (uses GPIO27)
    float readLux();                                                       // Convert ADC reading to Lux
};

/* Continuous rotation servo motor */
class MotorDriver {
public:
    MotorDriver(i2c_inst_t* i2c, uint8_t addr);         // Constructor
    void dc(int num, int speed, int direction);         // Control DC motor (number, speed 0-255, direction 0/1)
    void servo(int num, int angle);                     // Control servo motor (number 1-4, angle 0-180)
private: 
    i2c_inst_t* _i2c;
    uint8_t _addr; 
};

#endif
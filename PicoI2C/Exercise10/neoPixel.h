/******************************
 NeoPixel (WS2812) LED Strip Control
*******************************/
#ifndef LED_H
#define LED_H

#include <stdlib.h>            // Standard utilities
#include <cstdint>
#include "stdio.h"             // Standard I/O operations
#include "pico/stdlib.h"
#include "hardware/i2c.h"      // I²C interface
#include "ws2812.pio.h"        // Custom PIO program for NeoPixel (WS2812)

class NeoPixel {
public:
    NeoPixel(uint8_t  pinNumber, uint16_t numberOfPixels);                    // Constructor using default PIO
    NeoPixel(uint8_t  pinNumber, uint16_t numberOfPixels, PIO pio, uint sm);  // Constructor using specific PIO and state machine
    virtual ~NeoPixel(){};                                                    // Destructor

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
#endif
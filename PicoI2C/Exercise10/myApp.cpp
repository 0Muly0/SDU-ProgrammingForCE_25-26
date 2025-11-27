#include "myApp.h"
#include <iostream>
#include <iomanip>                                                          
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// #define TempHum
// #define Motor 
// #define RGBLed
#define Relay

MyApp::MyApp() {           
        stdio_init_all();                                                      // Initialize stdio

        i2c_init(i2c_default, 400 * 1000); 
        gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);            // SDA pin
        gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);            // SCL pin
        gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);                                // Enable pull-ups
        gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    }

#ifdef TempHum
void MyApp::run() {
    SHTC3 tempHumSensor(i2c_default, 0x70);
    tempHumSensor.wakeUp();
    
    while(true) {
        float temp = tempHumSensor.readTemperature();
        float hum = tempHumSensor.readHumidity();
        printf("Temperature: %.1f C, Humidity: %.1f %%\n", temp, hum);

        sleep_ms(500); 
    }
}
#endif

#ifdef Motor
void MyApp::run() {
    motorDriver motor(i2c_default, 0x22);

    while(true) {
        printf("Moving to 180 \n");
        motor.servo(1, 180);
        sleep_ms(1000);

        
        printf("Moving to 0 \n");
        motor.servo(1, 0);
        sleep_ms(1000);
    }
}
#endif

#ifdef RGBLed
void MyApp::run() {
    NeoPixel pixel = NeoPixel(6, 1);

    while(true) {
        printf("Starting color round");
        pixel.setPixelColor(0, 255, 0, 0);
        sleep_ms(500);
        pixel.setPixelColor(0, 0, 255, 0);
        sleep_ms(500);
        pixel.setPixelColor(0, 0, 0, 255);
        sleep_ms(500);
        pixel.setPixelColor(0, 255, 255, 255);
        sleep_ms(500);
        pixel.setPixelColor(0, 0, 0, 0);
        sleep_ms(500);
    }
}
#endif

#ifdef Relay
void MyApp::run() {
    RelayDriver relay = RelayDriver(12);

    while(true) {
        relay.activate();
        sleep_ms(1000);
        relay.deactivate();
        sleep_ms(1000);
    }
}
#endif
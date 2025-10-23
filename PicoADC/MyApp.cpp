#include <iostream>                                                                     // For console output
#include "pico/stdlib.h"                                                                // Pico SDK base
#include "hardware/adc.h"                                                               // ADC hardware
#include "PushButton.h"                                                                 // Button class
#include "ADC.h"                                                                        // ADC class
#include "dbop.h"                                                                       // Debug macros

// -----------------------------------------------------------------------------
// Select which exercise to compile
// -----------------------------------------------------------------------------
// #define EXERCISE_7A                                                                  // <-- choose which version to build

// -----------------------------------------------------------------------------
// EXERCISE 7A – Button-triggered ADC read
// -----------------------------------------------------------------------------
#ifdef EXERCISE_7A

using namespace std;

void MyApp() {
    ADC adcInput(26);                                                                   // Potentiometer on GPIO26
    Button button1(10, GPIO_IRQ_EDGE_RISE);                                             // Button on GPIO10

    cout << "EXERCISE 7A started: ADC read triggered by button events" << endl;         C_MyApp("Startup message");

    while (true) {
        if (button1.hasEvent()) {                                                       C_MyApp("Button event detected");
            int adcValue = adcInput.read();                                             C_MyApp("Read raw ADC value");
            int adcAvg = adcInput.readAvg(100);                                         C_MyApp("Read averaged ADC value");

            cout << "Button pressed > ADC value: " << adcValue
                 << " | Average: " << adcAvg << endl;                                   C_MyApp("Print ADC readings");
        }
        sleep_ms(10);                                                                   // Loop delay
    }
}

#endif // EXERCISE_7A



// -----------------------------------------------------------------------------
// EXERCISE 7B – Button sets frequency from potentiometer
// -----------------------------------------------------------------------------
#ifndef EXERCISE_7A

#include "PWM.h"                                                                        // PWM class
#include "hardware/pwm.h"                                                               // PWM hardware

using namespace std;

void MyApp() {
    stdio_init_all();
    cout << "EXERCISE 7B started: Button sets PWM frequency from potentiometer" << endl;
                                                                                        C_MyApp("Startup message");

    // --- Object creation ---
    ADC adcInput(26);                                                                   C_MyApp("Create ADC on GPIO26 (potentiometer)");
    Button button1(10, GPIO_IRQ_EDGE_RISE);                                             C_MyApp("Create Button on GPIO10 (rising edge)");
    PWM ledPWM(7);                                                                      C_MyApp("Create PWM for LED on GPIO7");
    PWM buzzerPWM(20, true);                                                            C_MyApp("Create PWM for buzzer on GPIO20 (inverted)");

    // --- Fixed duty cycle (intensity/volume) ---
    ledPWM.setDutyCycle(3.0f);
    buzzerPWM.setDutyCycle(5.0f);                                              

    ledPWM.setFrequency(20);                                                            C_MyApp("Set default LED PWM frequency");
    buzzerPWM.setFrequency(20);                                                         C_MyApp("Set default buzzer PWM frequency");

    while (true) {
        if (button1.hasEvent()) {                                                       C_MyApp("Button event detected");
            int adcAvg = adcInput.readAvg(100);                                         C_MyApp("Read averaged ADC value");

            ledPWM.setFrequency(adcAvg);                                                C_MyApp("Update LED PWM frequency");
            buzzerPWM.setFrequency(adcAvg);                                             C_MyApp("Update buzzer PWM frequency");

            cout << "Button pressed > New frequency set: " << adcAvg << " Hz" << endl;
        }

        sleep_ms(10);                                                                   // Loop delay
    }
}

#endif // EXERCISE_7B


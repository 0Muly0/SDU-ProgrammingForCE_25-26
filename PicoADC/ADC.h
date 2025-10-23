/*
@startuml
skinparam classAttributeIconSize 0
class ADC {

..attributes..
- _pin : uint
- _channel : uint

..methods..
+ ADC(gpioPin : uint)
+ read() : int
+ readAvg(avgVal : int) : int
- init() : void
}
@enduml
*/

#ifndef ADC_H
#define ADC_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "dbop.h"

class ADC {
private:
    uint _pin;
    uint _channel;
    void init();
    
public:
    // Constructor
    ADC(uint gpioPin);
    int read();
    int readAvg(int avgVal);
};
#endif // ADC_H
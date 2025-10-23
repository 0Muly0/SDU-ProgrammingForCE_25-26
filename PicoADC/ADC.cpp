#include <stdio.h>
#include "ADC.h"

ADC::ADC(uint gpioPin) : _pin(gpioPin){
    // Call init
    init();
}

void ADC::init() {
    C_ADC("ADC Initialization");
    adc_init();
    adc_gpio_init(_pin);

    if(_pin >= 26 && _pin <= 28) {
        _channel = _pin - 26;
    } else {
        _channel = 0;
    }

    adc_select_input(_channel);
}

int ADC::read() {
    C_ADC("Reading input from selected channel");
    adc_select_input(_channel);
    return (int) adc_read();
}

int ADC::readAvg(int avgVal) {
    if (avgVal <= 0) avgVal = 1;

    int tot = 0;
    for (int i = 0; i < avgVal; ++i) {
        tot += read();
        sleep_ms(2);
    }
    return tot / avgVal;
}
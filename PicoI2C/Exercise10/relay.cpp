/**
 * RELAY
 */
#include "relay.h"

RelayDriver::RelayDriver(uint8_t pinNumber): pinNumber(pinNumber) {
    gpio_init(pinNumber);               // Initializes relay gpio 
    gpio_set_dir(pinNumber, true);      // Initializes relay as output  
    gpio_put(pinNumber, false);         // Initializes as closed   
}

// Open Switch
void RelayDriver::activate() {                                         
    gpio_put(pinNumber, true);                                  
}

// Close Switch
void RelayDriver::deactivate() {                                        
    gpio_put(pinNumber, false);                                   
}

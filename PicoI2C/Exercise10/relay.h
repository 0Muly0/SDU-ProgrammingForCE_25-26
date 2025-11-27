#ifndef RELAY_H
#define RELAY_H

#include <stdlib.h>            // Standard utilities
#include <cstdint>
#include "stdio.h"             // Standard I/O operations
#include "pico/stdlib.h"

class RelayDriver {
public:
    RelayDriver(uint8_t pinNumber);
    void activate();
    void deactivate();
private:
    uint8_t pinNumber;
};
#endif
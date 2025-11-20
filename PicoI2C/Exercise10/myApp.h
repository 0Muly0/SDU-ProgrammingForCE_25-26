#ifndef MYAPP_H
#define MYAPP_H

#include "tempHum.h"
#include "motor.h"
#include "neoPixel.h"
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>                                                      

class MyApp {
public:
    MyApp(); 
    void run(); 
};

#endif
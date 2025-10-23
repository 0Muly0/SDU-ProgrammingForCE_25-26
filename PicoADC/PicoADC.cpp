// main.cpp
#include "pico/stdlib.h"
#include "MyApp.h"
#include "dbop.h"

int main() {
    stdio_init_all();                                   // Initialize USB serial and stdio

    MyApp();                                            // Call MyApp to run application");
    return 0;                                           // Program ended (never reached)");                                           
}

#include <cstdio>
#include "pico/stdlib.h"
// 8. Imported my_app header to be able to call the my_app function
#include "my_app.h"

//  --------------------------------------------------------
//  Debug Macro - enable/disable debug prints here
//  --------------------------------------------------------
#define DEBUG_INFO

#ifdef DEBUG_INFO
    #define COMMENT(msg) do { if(debug_flag) printf("DEBUG: %s\n", msg); } while(0)
#else
    #define COMMENT(msg)
#endif
//  --------------------------------------------------------

/* Pin constants declaration */
const uint LED_PIN = 7;
const uint BTN_PIN = 10;
/* High and Low GPIO mode */
const uint LED_OFF = 0;
const uint LED_ON = 1;

//  --------------------------------------------------------
//  Application function
//  --------------------------------------------------------
// 6. Defined my_app function to print a counter
void my_app(bool debug_flag) {
    static bool initialized = false;

    if(!initialized) {
        gpio_init(LED_PIN);                              
        COMMENT("Initialize LED_PIN GPIO");
        gpio_set_dir(LED_PIN, true);                     
        COMMENT("LED configured as output");

        gpio_init(BTN_PIN);                              
        COMMENT("Initialize BTN_PIN GPIO");
        gpio_set_dir(BTN_PIN, false);                    
        COMMENT("BTN_PIN configured as input");
        gpio_pull_down(BTN_PIN);                         
        COMMENT("Button configured as input with pull-down (active-high)");

        initialized = true;
    }

    bool pressed = (gpio_get(BTN_PIN) == 1);     
    COMMENT("Read the state of BTN_PIN (pressed=one, released=zero)");

    if (pressed) {
        gpio_put(LED_PIN, LED_ON);              
        COMMENT("Button pressed => LED ON");
    } else {
        gpio_put(LED_PIN, LED_OFF);              
        COMMENT("Button released => LED OFF");
    }

    printf("Hello from my_app\n"); 
    sleep_ms(1000);                      
}
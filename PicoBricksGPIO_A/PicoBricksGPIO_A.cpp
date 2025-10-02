#include <stdio.h>
#include "pico/stdlib.h"

/* Comment this line to disable debug info */ 
#define DEBUG_INFO
#ifdef DEBUG_INFO
    #define COMMENT(msg) printf("DEBUG: %s\n", msg)
#else
    #define COMMENT(msg)
#endif

/* Pin constants declaration */
const uint LED_PIN = 7;
const uint BTN_PIN = 10;
/* High and Low GPIO mode */
const uint LED_OFF = 0;
const uint LED_ON = 1;

int main()
{
    stdio_init_all();                               COMMENT("System initialized");

    gpio_init(LED_PIN);                             COMMENT("Initialize LED_PIN GPIO7");
    gpio_set_dir(LED_PIN, true);                    COMMENT("LED configured as output");
    
    gpio_init(BTN_PIN);                             COMMENT("Initialize BTN_PIN GPIO10");
    gpio_set_dir(BTN_PIN, false);                   COMMENT("BTN configured as input");
    /* 
    * By default the button state is floating around between 0 and 1. We need to set it.
    * By setting the button as pull down input, we assign 0 to it when it's un-pressed and 1 when it's pressed.
    * By setting the button as pull up input, we assign 1 to it when it's un-pressed and 0 when it's pressed.
    */
    gpio_pull_down(BTN_PIN);            

    while (true) {
        bool pressed = (gpio_get(BTN_PIN) == 1);    COMMENT("Read the state of BTN_PIN (pressed=1, released=0)");

        if (pressed) {
            gpio_put(LED_PIN, LED_ON);              COMMENT("Button pressed => LED_ON");
        } else {
            gpio_put(LED_PIN, LED_OFF);             COMMENT("Button released => LED_OFF");
        }

        printf("Hello, world!\n");                  COMMENT("Print 'Hello, World!' to the terminal for activity check");
        sleep_ms(1000);                             COMMENT("Wait 1000ms before repeating the loop");
    }
}

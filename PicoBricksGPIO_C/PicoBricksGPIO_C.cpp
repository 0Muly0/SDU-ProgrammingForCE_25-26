#include <stdio.h>
#include "pico/stdlib.h"

/* Comment this line to disable debug info */ 
#define DEBUG_INFO
#ifdef DEBUG_INFO
    #define COMMENT(msg) printf("DEBUG: %s\n", msg)
#else
    #define COMMENT(msg)
#endif

/* Symbolic constant for one second delay in ms */
#define ONE_SECOND 1000  

/* Pin constants declaration */
const uint LED_PIN = 7;
const uint BTN_PIN = 10;
/* High and Low GPIO mode */
const uint LED_OFF = 0;
const uint LED_ON = 1;

/* Custom sleep function with debug output every hundred ms */
void mySleep_ms(uint delay_ms) {
    uint steps = delay_ms / 100;                      COMMENT("Calculate number of hundred ms steps");
    for (uint i = 0; i < steps; i++) {
        /* Read current LED state */
        bool led_state = gpio_get_out_level(LED_PIN); 
        /* Read button state (true = pressed, false = not pressed) */
        bool btn_state = gpio_get(BTN_PIN);

        /* Print numeric status line (only numbers here) */
        printf("%d %d\n",
               led_state ? 1 : 0,   // LED state: one or zero
               btn_state ? 2 : 0    // Button state: two or zero
            );  
        sleep_ms(100);                                COMMENT("Wait hundred ms before next check");
    }
}

int main() {
    stdio_init_all();                                COMMENT("System initialized");

    gpio_init(LED_PIN);                              COMMENT("Initialize LED_PIN GPIO");
    gpio_set_dir(LED_PIN, true);                     COMMENT("LED configured as output");

    gpio_init(BTN_PIN);                              COMMENT("Initialize BTN_PIN GPIO");
    gpio_set_dir(BTN_PIN, false);                    COMMENT("BTN_PIN configured as input");
    /* 
    * By default the button state is floating around between 0 and 1. We need to set it.
    * By setting the button as pull down input, we assign 0 to it when it's un-pressed and 1 when it's pressed.
    * By setting the button as pull up input, we assign 1 to it when it's un-pressed and 0 when it's pressed.
    */
    gpio_pull_down(BTN_PIN);                         COMMENT("Button configured as input with pull-down (active-high)");

    while (true) {
        bool pressed = (gpio_get(BTN_PIN) == 1);     COMMENT("Read the state of BTN_PIN (pressed=one, released=zero)");

        if (pressed) {
            gpio_put(LED_PIN, LED_ON);               COMMENT("Button pressed => LED ON");
        } else {
            gpio_put(LED_PIN, LED_OFF);              COMMENT("Button released => LED OFF");
        }

        printf("Loop cycle started, waiting one second...\n"); 
        mySleep_ms(ONE_SECOND);                      COMMENT("Custom sleep with debug output every hundred ms");
    }
}
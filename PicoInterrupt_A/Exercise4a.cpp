#include <stdio.h>
#include "pico/stdlib.h"

// Uncomment this line to enable debug prints
//#define DEBUG_INFO

#ifdef DEBUG_INFO
  #define COMMENT(msg) printf("DEBUG: %s\n", msg)
#else
  #define COMMENT(msg)
#endif

#define ONE_SECOND 1000   // symbolic constant for one second delay in ms

const uint LED_PIN  = 7;    // PicoBricks LED
const uint BTN_PIN  = 10;   // PicoBricks Button
const uint LED_OFF  = 0;    // GPIO LOW  => LED_OFF
const uint LED_ON   = 1;    // GPIO HIGH => LED_ON
bool LED_STATE = 0;

// Global variable to store LED state
volatile bool led_on = false;

// Interrupt service routine for button press
void button_isr(uint gpio, uint32_t events) {
    COMMENT("Button interrupt triggered");
    if (gpio == BTN_PIN) {
        // TODO: Toggle the LED state and update the LED output
        // Hint: Flip the led_on variable and write it to the LED pin
        LED_STATE = !LED_STATE;
        gpio_put(LED_PIN,LED_STATE);
    }
}

// Custom sleep function with debug output every 10 ms
void mySleep_ms(uint delay_ms) {
    uint steps = delay_ms / 10;                       COMMENT("Calculate number of 10 ms steps");
    for (uint i = 0; i < steps; i++) {
        // Read current LED state
        bool led_state = gpio_get_out_level(LED_PIN); 
        // Read button state (true = pressed, false = not pressed)
        bool btn_state = gpio_get(BTN_PIN);

        // Print numeric status line (LED as 0/1, Button as 0/2)
        printf("%d %d\n",
               led_state ? 1 : 0,   // LED state: one or zero
               btn_state ? 2 : 0);  // Button state: two or zero

        sleep_ms(10);                                 COMMENT("Wait 10 ms before next check");
    }
}

int main() {
    stdio_init_all();                                COMMENT("System initialized");

    gpio_init(LED_PIN);                              COMMENT("Initialize LED_PIN GPIO");
    gpio_set_dir(LED_PIN, true);                     COMMENT("LED configured as output");
    gpio_put(LED_PIN, LED_STATE);                      COMMENT("Ensure LED starts OFF");

    gpio_init(BTN_PIN);                              COMMENT("Initialize BTN_PIN GPIO");
    gpio_set_dir(BTN_PIN, false);                    COMMENT("BTN_PIN configured as input");
    gpio_pull_down(BTN_PIN);                         COMMENT("Button configured with pull-down (active-high)");

    // Register interrupt on button pin (rising edge)
    // TODO: Use gpio_set_irq_enabled_with_callback to register ISR on rising edge
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE, true,&button_isr);
    COMMENT("Interrupt enabled on BTN_PIN (rising edge)");

    printf("Exercise 4a started. Press the button to toggle the LED.\n");

    while (true) {
        // Even though we sleep here, the LED still toggles immediately thanks to the interrupt
        printf("Main loop sleeping for one thousand ms...\n"); 
        mySleep_ms(ONE_SECOND);                      COMMENT("Custom sleep with debug output every 10 ms");
    }
}

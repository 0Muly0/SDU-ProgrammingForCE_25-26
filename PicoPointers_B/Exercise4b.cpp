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

// Struct to hold LED data
struct ToggleData {
    int toggle_count;   // number of toggles
    bool is_led_on;     // current LED state
};

// Global instance + pointer to struct
ToggleData data = {0, false};
ToggleData* pData = &data;

// Interrupt service routine for button press
void button_isr(uint gpio, uint32_t events) {
    COMMENT("Button interrupt triggered");
    if (gpio == BTN_PIN) {
        pData->toggle_count++;
        pData->is_led_on = !pData->is_led_on;
        gpio_put(LED_PIN, pData->is_led_on);
        // TODO: Increase toggle counter via pointer
        // TODO: Toggle LED state via pointer
        // TODO: Update LED output using pointer data
        //other way:
        //(*pData).toggle_count++;
        //(*pData).is_led_on = !(*pData).is_led_on;
        //gpio_put(LED_PIN, (*pData).is_led_on);
    }
}

int main() {
    stdio_init_all();                                 COMMENT("System initialized");

    gpio_init(LED_PIN);                               COMMENT("Initialize LED_PIN GPIO");
    gpio_set_dir(LED_PIN, true);                      COMMENT("LED configured as output");
    gpio_put(LED_PIN, LED_OFF);                       COMMENT("Ensure LED starts OFF");

    gpio_init(BTN_PIN);                               COMMENT("Initialize BTN_PIN GPIO");
    gpio_set_dir(BTN_PIN, false);                     COMMENT("BTN_PIN configured as input");
    gpio_pull_down(BTN_PIN);                          COMMENT("Button configured with pull-down (active-high)");

    // Register interrupt on button pin (rising edge)
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE, true, &button_isr);
    COMMENT("Interrupt enabled on BTN_PIN (rising edge)");

    printf("Exercise 4b started. Press the button to toggle the LED.\n");

    while (true) {
        // Sleep for one second
        sleep_ms(ONE_SECOND);

        // Access struct directly from the data structure
        printf("Direct access -> LED toggled %d times, current state: %s\n",
               data.toggle_count,
               data.is_led_on ? "ON" : "OFF");

        // WRONG: printing the pointer itself, not the data
//        printf("Pointer value (wrong use): %p\n", pData);

        // Access struct indirectly via pointer to the data structure
        printf("Pointer access -> LED toggled %d times, current state: %s\n\n",
               pData->toggle_count,
               pData->is_led_on ? "ON" : "OFF");
    }
}

#include <stdio.h>
#include "pico/stdlib.h"
#include <chrono>
#include <stdlib.h>   // For rand() and srand()
#include <time.h>     // For time()

using namespace std;
using namespace std::chrono;

// Uncomment this line to enable debug prints
// #define DEBUG_INFO

#ifdef DEBUG_INFO
  #define COMMENT(msg) printf("DEBUG: %s\n", msg)
#else
  #define COMMENT(msg)
#endif

// PicoBricks pin configuration
const uint LED_PIN  = 7;   // PicoBricks LED
const uint BTN_PIN  = 10;  // PicoBricks Button
const uint LED_OFF  = 0;   // GPIO LOW  => LED_OFF
const uint LED_ON   = 1;   // GPIO HIGH => LED_ON

// Struct to hold timing data
struct ReactionData {
    time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> stop;
};

// Global instance + pointer to struct
ReactionData R_data;
ReactionData* pData = &R_data;

// Interrupt service routine for button press
void button_isr(uint gpio, uint32_t events) {
    COMMENT("Button interrupt triggered -> store stop time");
    // Storing the stop time in the struct 
    pData->stop = high_resolution_clock::now();
}

int main() {
    stdio_init_all();                               COMMENT("System initialized");

    gpio_init(LED_PIN);                             COMMENT("Initialize LED_PIN GPIO");
    gpio_set_dir(LED_PIN, true);                    COMMENT("LED configured as output");

    gpio_init(BTN_PIN);                             COMMENT("Initialize BTN_PIN GPIO");
    gpio_set_dir(BTN_PIN, false);                   COMMENT("BTN_PIN configured as input");
    gpio_pull_down(BTN_PIN);                        COMMENT("Button configured with pull-down (active-high)");

    // Register interrupt on button pin (rising edge when pressed)
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE, true, &button_isr);
    COMMENT("Interrupt enabled on BTN_PIN (rising edge)");

    // Initialize random seed
    srand(time(NULL));                              COMMENT("Seed random number generator with current time");

    printf("Reaction Time Game started!\n");
    printf("---------------------------------\n");

    while (true) {
        int waitTime = 1 + (rand() % 3);            COMMENT("Pick random wait time (1–3 seconds)");
        printf("\nGet ready... Waiting %d seconds before LED flash!\n", waitTime);
        sleep_ms(waitTime * 1000);                  

        // Blink LED shortly
        printf("Be ready to press the button!\n");
        pData->start = high_resolution_clock::now(); COMMENT("Store start time (LED ON moment)");
        gpio_put(LED_PIN, LED_ON);                   COMMENT("LED ON -> signal to user");
        printf("LED flashed! PRESS NOW!\n");
        sleep_ms(100);                               
        gpio_put(LED_PIN, LED_OFF);                  COMMENT("LED OFF again");

        // Reset stop time before waiting
        pData->stop = time_point<high_resolution_clock>();
        COMMENT("Reset stop time before waiting for user response");

        // Wait for button interrupt to set stop time
        while (pData->stop.time_since_epoch().count() == 0) {
            tight_loop_contents();                  COMMENT("Idle loop until button interrupt fires");
        }

        // Calculate and print reaction time
        auto reaction = duration_cast<milliseconds>(pData->stop - pData->start).count();
        printf("Your reaction time: %lld ms\n", reaction);
        COMMENT("Reaction time printed to console");

        printf("Try again in 1 second...\n");
        sleep_ms(1000);                             
    }
}

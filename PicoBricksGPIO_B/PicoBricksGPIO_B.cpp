#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/regs/sio.h"
#include "hardware/structs/sio.h"

#define DEBUG_INFO
#ifdef DEBUG_INFO
  #define COMMENT(msg) printf("DEBUG: %s\n", msg)
#else
  #define COMMENT(msg)
#endif

/* Constants for memory locations */
#define SIO_BASE        0xd0000000
#define GPIO_OUT_SET    (SIO_BASE + 0x014)
#define GPIO_OUT_CLR    (SIO_BASE + 0x018)

/* Pin constants declaration */
const uint LED_PIN  = 7;   
const uint LED_OFF  = 0;  
/* High and Low GPIO mode */
const uint LED_ON   = 1;   

int main() {
    stdio_init_all();                                       COMMENT("System initialized");

    gpio_init(LED_PIN);                                     COMMENT("Initialize LED_PIN GPIO");
    gpio_set_dir(LED_PIN, true);                            COMMENT("LED configured as output");

    while (true) {
        // --- Method 1: HAL functions ---
        gpio_put(LED_PIN, LED_ON);                          COMMENT("HAL: LED ON using gpio_put()");
        sleep_ms(500);                                      COMMENT("Wait 500ms with LED ON");
        gpio_put(LED_PIN, LED_OFF);                         COMMENT("HAL: LED OFF using gpio_put()");
        sleep_ms(500);                                      COMMENT("Wait 500ms with LED OFF");

        // --- Method 2a: Direct register access ---
        /* We shift 1 to the left 'LED_PIN' times and load it in the SET memory cell from the sio_hw STRUCT */
        sio_hw->gpio_set = 1u << LED_PIN;                   COMMENT("STRUCT: LED ON using sio_hw->gpio_set");
        sleep_ms(500);                                      COMMENT("Wait 500ms with LED ON");
        /* We shift 1 to the left 'LED_PIN' times and load it in the CLEAR memroy cell from the sio_hw STRUCT */
        sio_hw->gpio_clr = 1u << LED_PIN;                   COMMENT("STRUCT: LED OFF using sio_hw->gpio_clr");
        sleep_ms(500);                                      COMMENT("Wait 500ms with LED OFF");

        // --- Method 2b: Direct register access (Expanded) ---
        /* We shift 1 to the left 'LED_PIN' times and load it in the SET memory cell by address */
        *(volatile uint32_t *)GPIO_OUT_SET = 1u << LED_PIN; COMMENT("RAW: Write bitmask directly to memory address 0xd0000014 -> LED ON");
        sleep_ms(500);                                      COMMENT("Wait 500ms with LED ON");
        /* We shift 1 to the left 'LED_PIN' times and load it in the CLEAR memory cell by address */
        *(volatile uint32_t *)GPIO_OUT_CLR = 1u << LED_PIN; COMMENT("RAW: Write bitmask directly to memory address 0xd0000018 -> LED OFF");
        sleep_ms(500);                                      COMMENT("Wait 500ms with LED OFF");
                                                            COMMENT(" "); //New line
    }
}
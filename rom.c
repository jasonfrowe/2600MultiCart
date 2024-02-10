/*
* PicoMultiRom for ATARI 2600+
* Jason Rowe (jasonfrowe@gmail.com)
* Based on PicoROM by Nick Bild (nick.bild@gmail.com)
* and Otaku-flash by karrika (https://github.com/karrika/Otaku-flash)
* January 2024
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pin_definitions.h"
#include <stdlib.h>
#include "hardware/adc.h"

#include "cart_setup.h" // variables needed for ROM

void setup_gpio(); // uses pin defintions and sets address and data bus
int get_requested_address(); // get address from address bus
void put_data_on_bus(uint8_t); // puts data on bus

uint16_t ce;  // chip-enable value
uint16_t last_ce; // last value to monitor changes in ce 


int main() {

    // Set system clock speed.
    set_sys_clock_khz(270000, true);

    // enable serial
    // stdio_usb_init();
    // sleep_ms(2000); // Wait two seconds to make sure USB-serial is alive

    // Turn on built in LED to see that we have power
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // A12 Cartridge, Data-ready pin (0 == data on address)
    const uint CE_PIN = 20;
    gpio_init(CE_PIN);
    gpio_set_dir(CE_PIN, GPIO_IN);
    // gpio_pull_up(CE_PIN);

    setup_cart(); // Get rom contents and setup bankswitching

    // printf("Cart %d \n", cart);
    // printf("Rom %d \n", rom_contents[1]);
    
    last_ce = 1;
    
    // GPIO setup for address and data bus
    setup_gpio();
    
    // Continually check address lines and
    // put associated data on bus.
    while (true) {

        ce = gpio_get(CE_PIN);
        if (ce != last_ce) {
            last_ce = ce;
            if (last_ce) {
                gpio_set_dir_out_masked(0xFF); // data_bus_input();
            } else{
                gpio_set_dir_in_masked(0xFF); // data_bus_output();
            }
        }
        if (last_ce == 1){
            addr = get_requested_address();
            put_data_on_bus(rom_contents[addr + romoffset]);

            // bankswitching function
            (*fpBankSwitching)(addr);

        }

    }
}



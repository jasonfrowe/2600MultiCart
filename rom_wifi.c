/*
* PicoMultiRom for ATARI 2600+
* Jason Rowe (jasonfrowe@gmail.com)
* Now with WIFI!! 
* January 2024
*
*/

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pin_definitions.h"
#include <stdlib.h>
#include "hardware/adc.h"

#include "pico/multicore.h"  // enable multi-core

#include "pico/cyw43_arch.h" // Library for PICO_W 

#include "lwip/dns.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"

#include "cart_setup.h" // variables and functions needed for cartrdige ROM

#include "mqtt_setup.h" // functions for MQTT communication 

void setup_gpio(); // uses pin defintions and sets address and data bus
int get_requested_address(); // get address from address bus
void put_data_on_bus(uint8_t); // puts data on bus


// A12 Cartridge, Data-ready pin (0 == data on address)
uint16_t ce;  // chip-enable value
uint16_t last_ce; // last value to monitor changes in ce 
const uint CE_PIN = 27;

void cartridge(){
    // Continually check address lines and
    // put associated data on bus.
    while (true) {

        ce = gpio_get(CE_PIN);
        if (ce != last_ce) {
            last_ce = ce;
            if (last_ce) {
                gpio_set_dir_in_masked(0xFF); // data_bus_input();
            } else{
                gpio_set_dir_out_masked(0xFF); // data_bus_output();
            }
        }
        if (last_ce == 0){
            addr = get_requested_address();
            put_data_on_bus(rom_contents[addr + romoffset]);

            // bankswitching function
            (*fpBankSwitching)(addr);

        }

    }
}


int main() {

    // Set system clock speed.
    set_sys_clock_khz(250000, true);

    // enable serial
    // stdio_init_all();
    stdio_usb_init();
    sleep_ms(2000); // Wait two seconds to make sure USB-serial is alive


    // *** Set up the ROM and run the Cartridge Service *** //

    // A12 Cartridge, Data-ready pin (0 == data on address)
    gpio_init(CE_PIN);
    gpio_set_dir(CE_PIN, GPIO_IN);
    // gpio_pull_up(CE_PIN);

    setup_cart(); // Get rom contents and setup bankswitching
    
    last_ce = 1;
    
    // GPIO setup for address and data bus
    setup_gpio();

    // Launch the cartridge service on the 2nd core
    multicore_launch_core1(cartridge); 


    // *** Set up WIFI services *** ///

    
    char wifi_pass[] = "YourPassWordHere"

    printf(WIFI_SSID);
    printf("\n");

    // Initialize the WIFI parts of the PICO_W -- including the internal LED
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");

    // change wifi_pass to WIFI_PASSWORD
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, wifi_pass, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
    }

    // Turn on the PICO-W led to let us know we have a WIFI connection.
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); 

    // *** Set up MQTT Service *** ///

    MQTT_CLIENT_T *state = mqtt_client_init();
     
    run_dns_lookup(state);
 
    mqtt_run_test(state);

    
    
}







// Variables about the ROM
uint romsize = 4096; // Will be set by setup_cart
uint8_t rom_contents[32768] = { 0 }; // Will be set by setup_cart
uint8_t cart = 0; // Will be set by GPIO pins
uint bankswitch = 0; // Will be set by setup_cart
uint16_t romoffset = 0; // offset for bankswitching
uint16_t addr;  // address read from address lines

void setup_cart();  // Will fetch the rom contents
void (*fpBankSwitching)(uint16_t); // function pointer
void BankSwitching_none(uint16_t);
void BankSwitching_F8(uint16_t); // 8K
void BankSwitching_F6(uint16_t); // 16K
void BankSwitching_F4(uint16_t); // 32K

// Function to set up ROM from DIP-switches

void setup_cart() {
    // Read in DIPs to get cart to use
    gpio_init(21);
    gpio_set_dir(21, GPIO_IN);
    gpio_pull_up(21);

    gpio_init(22);
    gpio_set_dir(22, GPIO_IN);
    gpio_pull_up(22);

    gpio_init(26);
    gpio_set_dir(26, GPIO_IN);
    gpio_pull_up(26);

    gpio_init(27);
    gpio_set_dir(27, GPIO_IN);
    gpio_pull_up(27);

    cart = 8*gpio_get(27) + 4*gpio_get(26) + 2*gpio_get(22) + gpio_get(21);
    // GPIO(27)*8 only works if called twice?? What is wrong?
    cart = 8*gpio_get(27) + 4*gpio_get(26) + 2*gpio_get(22) + gpio_get(21);
    // uint8_t gp27 = gpio_get(27);
    // cart = gp27*8 + 4*gpio_get(26) + 2*gpio_get(22) + gpio_get(21);

    // Specify contents of emulated ROM.
    if (cart == 0){
        #include "roms_00.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 1){
        #include "roms_01.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 2){
        #include "roms_02.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 3){
        #include "roms_03.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 4){
        #include "roms_04.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 5){
        #include "roms_05.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 6){
        #include "roms_06.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 7){
        #include "roms_07.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 8){
        #include "roms_08.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 9){
        #include "roms_09.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 10){
        #include "roms_10.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 11){
        #include "roms_11.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 12){
        #include "roms_12.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 13){
        #include "roms_13.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 14){
        #include "roms_14.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    } else if (cart == 15){
        #include "roms_15.h"
        romsize = romsize_in;
        for (int i = 0; i < romsize_in; i++){
            rom_contents[i] = rom_contents_in[i];
        }
    }

    if (romsize == 4096){
        bankswitch = 0; // No bankswitching
        fpBankSwitching = &BankSwitching_none;
    } else if (romsize == 8192){
        bankswitch = 1; // F8 bankswitching
        fpBankSwitching = &BankSwitching_F8;
    } else if (romsize == 16384){
        bankswitch = 2; // F6 bankswitching
        fpBankSwitching = &BankSwitching_F6;
    } else if (romsize == 32768){
        bankswitch = 3; // F4 bankswitching
        fpBankSwitching = &BankSwitching_F4;
    }

}

// Bank Switching Functions 

void BankSwitching_none(uint16_t addr_in) {
    romoffset = 0;
}

void BankSwitching_F8(uint16_t addr_in) {
    if (addr_in == 4088){
        romoffset = 0;
    } else if (addr_in == 4089){
        romoffset = 4096;
    }
}

void BankSwitching_F6(uint16_t addr_in) {
    if ((addr_in >= 4086) && (addr_in <= 4089)){
        romoffset = 4096 * (addr_in - 4086);
    }
}


void BankSwitching_F4(uint16_t addr_in) {

    if ((addr_in >= 4084) && (addr_in <= 4091)){
        romoffset = 4096 * (addr_in - 4084);
    }
}
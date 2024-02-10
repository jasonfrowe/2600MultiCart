// Don't change the pin definitions.
// Optimizations expect that they are in these positions.

// Address pins.
#define A0 8
#define A1 9
#define A2 10
#define A3 11
#define A4 12
#define A5 13
#define A6 14
#define A7 15
#define A8 16
#define A9 17
#define A10 18
#define A11 19

// Data pins.
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

// GPIO Setup Functions 

void setup_gpio() {
    // Address pins.
    gpio_init(A0);
    gpio_set_dir(A0, GPIO_IN);
    gpio_init(A1);
    gpio_set_dir(A1, GPIO_IN);
    gpio_init(A2); 
    gpio_set_dir(A2, GPIO_IN);
    gpio_init(A3);
    gpio_set_dir(A3, GPIO_IN);
    gpio_init(A4);
    gpio_set_dir(A4, GPIO_IN);
    gpio_init(A5);
    gpio_set_dir(A5, GPIO_IN);
    gpio_init(A6);
    gpio_set_dir(A6, GPIO_IN);
    gpio_init(A7);
    gpio_set_dir(A7, GPIO_IN);
    gpio_init(A8);
    gpio_set_dir(A8, GPIO_IN);
    gpio_init(A9);
    gpio_set_dir(A9, GPIO_IN);
    gpio_init(A10);
    gpio_set_dir(A10, GPIO_IN);
    gpio_init(A11);
    gpio_set_dir(A11, GPIO_IN);

    // Data pins.
    gpio_init(D0);
    gpio_set_dir(D0, GPIO_OUT);
    gpio_init(D1);
    gpio_set_dir(D1, GPIO_OUT);
    gpio_init(D2);
    gpio_set_dir(D2, GPIO_OUT);
    gpio_init(D3);
    gpio_set_dir(D3, GPIO_OUT);
    gpio_init(D4);
    gpio_set_dir(D4, GPIO_OUT);
    gpio_init(D5);
    gpio_set_dir(D5, GPIO_OUT);
    gpio_init(D6);
    gpio_set_dir(D6, GPIO_OUT);
    gpio_init(D7);
    gpio_set_dir(D7, GPIO_OUT);
}

int get_requested_address() {
    // Return only first 12 bits from address bus.
    return ((gpio_get_all() >> 8) & 4095);
}

void put_data_on_bus(uint8_t rom_cont) {
    // 8-bits onto the data bus
    gpio_put_masked(255, rom_cont);
}


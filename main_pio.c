#include "main.pio.h"
#include "hardware/clocks.h"

// Definição da função
void main_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = main_program_get_default_config(offset);

    // Set pin to be part of set output group, i.e. set by set instruction
    sm_config_set_set_pins(&c, pin, 1);

    // Attach pio to the GPIO
    pio_gpio_init(pio, pin);

    // Set pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    // Set pio clock to 8MHz, giving 10 cycles per LED binary digit
    float div = clock_get_hz(clk_sys) / 8000000.0;
    sm_config_set_clkdiv(&c, div);

    // Give all the FIFO space to TX (not using RX)
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    // Shift to the left, use autopull, next pull threshold 24 bits
    sm_config_set_out_shift(&c, false, true, 24);

    // Set sticky-- continue to drive value from last set/out.  Other stuff off.
    sm_config_set_out_special(&c, true, false, false);

    // Load configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);

    // enable this pio state machine
    pio_sm_set_enabled(pio, sm, true);
}
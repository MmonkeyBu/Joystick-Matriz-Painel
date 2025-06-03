/**
 * @file pio_main.c
 * @brief Implementation of PIO program for LED control
 */

 #include "main.pio.h"

 // Program instructions
 static const uint16_t main_program_instructions[] = {
     0x6621, //  0: out    x, 1            side 0 [6]
     0x1424, //  1: jmp    !x, 4           side 1 [4]
     0xf401, //  2: set    pins, 1         side 1 [4]
     0x1406, //  3: jmp    6               side 1 [4]
     0xf201, //  4: set    pins, 1         side 1 [2]
     0xe200, //  5: set    pins, 0         side 0 [2]
     0xe100, //  6: set    pins, 0         side 0 [1]
 };
 
 // Program definition
 const pio_program_t main_program = {
     .instructions = main_program_instructions,
     .length = 7,
     .origin = -1,
     .pio_version = MAIN_PIO_VERSION,
 #if PICO_PIO_VERSION > 0
     .used_gpio_ranges = 0x0
 #endif
 };
 
 pio_sm_config main_program_get_default_config(uint offset) {
     pio_sm_config c = pio_get_default_sm_config();
     sm_config_set_wrap(&c, offset + MAIN_WRAP_TARGET, offset + MAIN_WRAP);
     sm_config_set_sideset(&c, 1, false, false);
     return c;
 }
 
 void main_program_init(PIO pio, uint sm, uint offset, uint pin) {
     pio_sm_config c = main_program_get_default_config(offset);
     sm_config_set_set_pins(&c, pin, 1);
     pio_gpio_init(pio, pin);
     pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
     
     float div = clock_get_hz(clk_sys) / 8000000.0;
     sm_config_set_clkdiv(&c, div);
     sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
     sm_config_set_out_shift(&c, false, true, 24);
     sm_config_set_out_special(&c, true, false, false);
     
     pio_sm_init(pio, sm, offset, &c);
     pio_sm_set_enabled(pio, sm, true);
 }
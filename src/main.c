/**
 * @file main.c
 * @brief LED Matrix Demo Application
 *
 * Demonstrates LED matrix control with scrolling text and animations
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "main.pio.h"
#include "inc/define_pins.h"
#include "inc/drivers/frames_3x5.h"
#include "inc/drivers/frames_5x5.h"
#include "inc/drivers/letras_3x5.h"
#include "inc/drivers/letras_5x5.h"
#include "inc/led_functions.h"
#include "inc/joystick.h"
#include "inc/timers_callback.h"

#define DEBOUNCE_TIME_MS 400 // Button debounce time in milliseconds

/**
 * @brief Main application entry point
 * @return Exit status
 */
int main()
{
    // Initialize stdio for debug output
    stdio_init_all();
    init_GPIO(); // Initialize GPIO pins

    // PIO configuration variables
    PIO pio;
    uint sm, offset;

    // Initialize system clock and PIO
    if (!init_system(&pio, &sm, &offset))
    {
        return 1; // Exit if initialization fails
    }

    JoystickState js;
    RGBColor color = {0, 156, 255}; // Blue color
    double intensity = 0.1;         // LED intensity
    uint16_t speed = 150;           // Animation speed
    uint8_t font_width = 5;         // Font size (5x5)

    joystick_init();

    while (true)
    {
        display_joystick_frame(js, color, pio, sm, intensity);

        joystick_update(&js);
        sleep_ms(100);
        // Display all musical note frames
        // for (int i = 0; i < 15; i++)
        // {
        //     display_frame(all_notes[i], color, pio, sm, intensity);
        //     sleep_ms(DEBOUNCE_TIME_MS);

        //     if (!gpio_get(JSTICK))
        //     {
        //         reset_usb_boot(0, 0);
        //     }
        // }
    }
}

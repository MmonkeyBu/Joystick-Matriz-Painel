#include "inc/timers_callback.h"
#include "inc/joystick.h"

void btn_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    switch (gpio)
    {
    case JSTYCK_BTN:
        if (current_time - last_interruption_jstyck_btn > DEBOUNCE_TIME)
        {
            last_interruption_jstyck_btn = current_time;
            if (events & GPIO_IRQ_EDGE_FALL)
            {
                printf("Botão JOYSTICK_BTN pressionado\n");
                reset_usb_boot(0, 0); // enter in bootsel mode
            }
        }
        break;

    default:
        break;
    }
}

void gpio_pin_init(uint8_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
}
#include "inc/define_pins.h"

void init_GPIO()
{
    gpio_init(JSTICK);
    gpio_set_dir(JSTICK, GPIO_IN);
    gpio_pull_up(JSTICK);
}
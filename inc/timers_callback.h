#ifndef TIMERS_CALLBACK_H
#define TIMERS_CALLBACK_H

#include <stdbool.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define DEBOUNCE_TIME 250 // debounce for buttons

static volatile uint32_t last_interruption_jstyck_btn = 0;

void btn_callback(uint gpio, uint32_t events);
void gpio_pin_init(uint8_t pin);

#endif
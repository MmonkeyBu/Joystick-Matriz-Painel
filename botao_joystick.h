#ifndef BOTAO_JOYSTICK_H
#define BOTAO_JOYSTICK_H

#include <stdbool.h>
#include "init_GPIO.h"

extern volatile bool estado_inverter;

void botao_callback(uint gpio, uint32_t eventos);
void botao_init(uint8_t pino);

#endif

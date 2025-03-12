#ifndef FRAMES_3x5_H
#define FRAMES_3x5_H

#include "pico/stdlib.h"

#define NUM_LEDS_FINO 15

double f_01_fino[NUM_LEDS] = { // completamente apagado
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0};

double f_02_fino[NUM_LEDS] = { // todos acesos
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0};

#endif
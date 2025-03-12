#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "stdbool.h"

#define DEADZONE 500
#define ADC_MAX 4095
#define MID (ADC_MAX / 2)

#define lISTA_TAM 20


typedef enum {
    CENTRALIZADO,
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA,
    DIREITA_CIMA,
    DIREITA_BAIXO,
    ESQUERDA_CIMA,
    ESQUERDA_BAIXO
} Direction;


//para os gestos

typedef struct {
    int x;
    int y;
} Posicao;

//lista circular 
typedef enum {
    GESTO_NENHUM,
    GESTO_VOLTA_COMPLETA,
    GESTO_MEIA_LUA_DIREITA,
    GESTO_MEIA_LUA_ESQUERDA
} DirectionJ;

void atualizar_historico(int x, int y);
Direction identificar_gesto();
void detectar_gesto(int x, int y);

//porcentagem
void porcentagem(int x, int y);

Direction print_direction(int x, int y, bool estado_inverter);

#endif

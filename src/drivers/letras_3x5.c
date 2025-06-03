#include "inc/drivers/letras_3x5.h"

// Letra A
double f_A_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0
};

// Letra B
double f_B_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0
};

// Letra C
double f_C_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 1.0
};

// Letra D
double f_D_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0
};

// Letra E
double f_E_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 1.0
};

// Letra F
double f_F_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0
};

// Letra G
double f_G_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 1.0, 1.0
};

// Letra H
double f_H_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0
};

// Letra I
double f_I_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0
};

// Letra J
double f_J_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    0.0, 1.0, 0.0
};

// Letra K
double f_K_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0
};

// Letra L
double f_L_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 1.0
};

// Letra M
double f_M_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0
};

// Letra N
double f_N_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0
};

// Letra O
double f_O_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 1.0, 0.0
};

// Letra P
double f_P_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0
};

// Letra Q
double f_Q_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 1.0, 1.0
};

// Letra R
double f_R_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0
};

// Letra S
double f_S_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 0.0
};

// Letra T
double f_T_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0
};

// Letra U
double f_U_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 1.0, 0.0
};

// Letra V
double f_V_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0
};

// Letra W
double f_W_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0
};

// Letra X
double f_X_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0
};

// Letra Y
double f_Y_fino[NUM_LEDS_FINO] = {
    1.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0
};

// Letra Z
double f_Z_fino[NUM_LEDS_FINO] = {
    1.0, 1.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 1.0
};

// Exclamação
double f_exclamation_fino[NUM_LEDS_FINO] = {
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0
};

// Ponto
double f_ponto_fino[NUM_LEDS_FINO] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0
};

double *letras_3x5[] = {
    f_A_fino, f_B_fino, f_C_fino, f_D_fino, f_E_fino, f_F_fino,
    f_G_fino, f_H_fino, f_I_fino, f_J_fino, f_K_fino, f_L_fino,
    f_M_fino, f_N_fino, f_O_fino, f_P_fino, f_Q_fino, f_R_fino,
    f_S_fino, f_T_fino, f_U_fino, f_V_fino, f_W_fino, f_X_fino,
    f_Y_fino, f_Z_fino, f_01_fino, f_exclamation_fino, f_ponto_fino // Espaço, exclamação e ponto
};

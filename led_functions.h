#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "main.pio.h"
#include "init_GPIO.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "letras_3x5.h"
#include "letras_5x5.h"

#define MAX_FRASE_TAMANHO 50                // Número máximo de caracteres na frase
#define MAX_COLUNAS (5 * MAX_FRASE_TAMANHO) // Número máximo de colunas na frase concatenada


// Estrutura para representar uma cor RGB
typedef struct {
    double r; // Vermelho (0.0 a 1.0)
    double g; // Verde (0.0 a 1.0)
    double b; // Azul (0.0 a 1.0)
} RGBColor;

// Enumeração para mapear caracteres
typedef enum {
    CHAR_A, CHAR_B, CHAR_C, CHAR_D, CHAR_E, CHAR_F, CHAR_G, CHAR_H, CHAR_I, CHAR_J,
    CHAR_K, CHAR_L, CHAR_M, CHAR_N, CHAR_O, CHAR_P, CHAR_Q, CHAR_R, CHAR_S, CHAR_T,
    CHAR_U, CHAR_V, CHAR_W, CHAR_X, CHAR_Y, CHAR_Z, CHAR_SPACE, CHAR_EXCLAMATION, CHAR_PONTO
} Caractere;

// Declarações das funções
uint32_t matriz_rgb(double b, double r, double g);
void converterCorParaEscala(RGBColor *cor);
int mapear_indice_para_posicao(int indice);
void setar_led(int index, RGBColor cor, PIO pio, uint sm);
double **criar_frase_da_string(const char *texto, int tamanho_matriz);
void exibir_frame(double *frame, RGBColor cor, PIO pio, uint sm, double intensidade);
void concatenar_frase(double *frase[], int tamanho_frase, double frase_concatenada[5][MAX_COLUNAS]);
void exibir_frase_rolagem(const char *texto, RGBColor cor, PIO pio, uint sm, double intensidade, int velocidade, int tamanho_matriz);


#endif
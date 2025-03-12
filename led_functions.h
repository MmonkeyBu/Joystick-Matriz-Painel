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

#define NUM_LEDS 25                         // Número de LEDs na matriz
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

// Implementações das funções

/**
 * Codifica a cor no formato G | R | B.
 * @param b Componente azul (0.0 a 1.0).
 * @param r Componente vermelho (0.0 a 1.0).
 * @param g Componente verde (0.0 a 1.0).
 * @return Cor codificada no formato G | R | B.
 */
uint32_t matriz_rgb(double b, double r, double g) {
    unsigned char R, G, B;
    R = r * 255;                             // Converte vermelho para 0-255
    G = g * 255;                             // Converte verde para 0-255
    B = b * 255;                             // Converte azul para 0-255
    return (G << 24) | (R << 16) | (B << 8); // Codifica no formato G | R | B
}

/**
 * Converte a cor de uma escala de 0-255 para 0.0-1.0.
 * @param cor Ponteiro para a estrutura RGBColor a ser convertida.
 */
void converterCorParaEscala(RGBColor *cor) {
    // Impede que o número ultrapasse o intervalo [0, 255]
    if (cor->r > 255) cor->r = 255;
    if (cor->b > 255) cor->b = 255;
    if (cor->g > 255) cor->g = 255;

    if (cor->r < 0) cor->r = 0;
    if (cor->b < 0) cor->b = 0;
    if (cor->g < 0) cor->g = 0;

    // Converte para o intervalo [0.0, 1.0]
    cor->r = cor->r / 255.0;
    cor->b = cor->b / 255.0;
    cor->g = cor->g / 255.0;
}

/**
 * Mapeia o índice lógico para a posição física na matriz de LEDs.
 * @param indice Índice lógico do LED.
 * @return Índice físico correspondente na matriz.
 */
int mapear_indice_para_posicao(int indice) {
    int linha = 4 - (indice / 5);  // Linha (0 a 4, de baixo para cima)
    int coluna = 4 - (indice % 5); // Coluna (0 a 4, invertida para corrigir o espelhamento)

    // Inverte a coluna para linhas ímpares (1 e 3)
    if (linha == 1 || linha == 3) {
        coluna = 4 - coluna;
    }

    return (linha * 5) + coluna; // Retorna o índice físico
}

/**
 * Define a cor de um LED específico na matriz.
 * @param index Índice do LED.
 * @param cor Cor a ser definida.
 * @param pio Instância do PIO.
 * @param sm Estado da máquina de estados (state machine).
 */
void setar_led(int index, RGBColor cor, PIO pio, uint sm) {
    uint32_t valor_led = matriz_rgb(cor.b, cor.r, cor.g); // Codifica a cor
    pio_sm_put_blocking(pio, sm, valor_led);             // Envia o valor para o LED
}

/**
 * Cria uma lista de frames a partir de uma string.
 * @param texto String de entrada.
 * @param tamanho_matriz Tamanho da matriz (3 ou 5).
 * @return Ponteiro para a lista de frames.
 */
double **criar_frase_da_string(const char *texto, int tamanho_matriz) {
    // Define o número máximo de caracteres na frase
    int max_caracteres = strlen(texto);
    // Cada frase é seguida por um espaço, então o tamanho da lista de frames é (max_caracteres + 1)
    double **frase = (double **)malloc((max_caracteres + 1) * sizeof(double *));

    // Define qual fonte usar (3x5 ou 5x5)
    double **fonte = (tamanho_matriz == 3) ? letras_3x5 : letras_5x5;
    double *espaco = (tamanho_matriz == 3) ? f_01_fino : f_01; // Frame de espaço em branco

    // Adiciona os frames das letras
    for (int i = 0; i < max_caracteres; i++) {
        char c = toupper(texto[i]); // Converte para maiúscula

        switch (c) {
            case 'A': frase[i] = fonte[CHAR_A]; break;
            case 'B': frase[i] = fonte[CHAR_B]; break;
            case 'C': frase[i] = fonte[CHAR_C]; break;
            case 'D': frase[i] = fonte[CHAR_D]; break;
            case 'E': frase[i] = fonte[CHAR_E]; break;
            case 'F': frase[i] = fonte[CHAR_F]; break;
            case 'G': frase[i] = fonte[CHAR_G]; break;
            case 'H': frase[i] = fonte[CHAR_H]; break;
            case 'I': frase[i] = fonte[CHAR_I]; break;
            case 'J': frase[i] = fonte[CHAR_J]; break;
            case 'K': frase[i] = fonte[CHAR_K]; break;
            case 'L': frase[i] = fonte[CHAR_L]; break;
            case 'M': frase[i] = fonte[CHAR_M]; break;
            case 'N': frase[i] = fonte[CHAR_N]; break;
            case 'O': frase[i] = fonte[CHAR_O]; break;
            case 'P': frase[i] = fonte[CHAR_P]; break;
            case 'Q': frase[i] = fonte[CHAR_Q]; break;
            case 'R': frase[i] = fonte[CHAR_R]; break;
            case 'S': frase[i] = fonte[CHAR_S]; break;
            case 'T': frase[i] = fonte[CHAR_T]; break;
            case 'U': frase[i] = fonte[CHAR_U]; break;
            case 'V': frase[i] = fonte[CHAR_V]; break;
            case 'W': frase[i] = fonte[CHAR_W]; break;
            case 'X': frase[i] = fonte[CHAR_X]; break;
            case 'Y': frase[i] = fonte[CHAR_Y]; break;
            case 'Z': frase[i] = fonte[CHAR_Z]; break;
            case ' ': frase[i] = fonte[CHAR_SPACE]; break;
            case '!': frase[i] = fonte[CHAR_EXCLAMATION]; break;
            case '.': frase[i] = fonte[CHAR_PONTO]; break;
            default:  frase[i] = fonte[CHAR_SPACE]; break; // Caractere desconhecido vira espaço
        }
    }

    // Adiciona um espaço em branco no final da frase
    frase[max_caracteres] = fonte[CHAR_SPACE];

    return frase;
}

/**
 * Exibe um frame na matriz de LEDs.
 * @param frame Frame a ser exibido.
 * @param cor Cor do frame.
 * @param pio Instância do PIO.
 * @param sm Estado da máquina de estados.
 * @param intensidade Intensidade do frame (0.0 a 1.0).
 */
void exibir_frame(double *frame, RGBColor cor, PIO pio, uint sm, double intensidade) {
    // Garante que a intensidade esteja no intervalo [0.0, 1.0]
    if (intensidade < 0.0) intensidade = 0.0;
    if (intensidade > 1.0) intensidade = 1.0;

    converterCorParaEscala(&cor);

    for (int i = 0; i < NUM_LEDS; i++) {
        // Mapeia o índice lógico para o índice físico
        int indice_fisico = mapear_indice_para_posicao(i);

        // Aplica a cor ao LED, multiplicando pela intensidade do frame e pela intensidade geral
        RGBColor cor_led = {
            cor.r * frame[indice_fisico] * intensidade, // Vermelho
            cor.g * frame[indice_fisico] * intensidade, // Verde
            cor.b * frame[indice_fisico] * intensidade  // Azul
        };

        // Define a cor do LED
        setar_led(i, cor_led, pio, sm);
    }
}

/**
 * Concatena os frames das letras da frase.
 * @param frase Lista de frames das letras.
 * @param tamanho_frase Tamanho da frase.
 * @param frase_concatenada Matriz para armazenar a frase concatenada.
 */
void concatenar_frase(double *frase[], int tamanho_frase, double frase_concatenada[5][MAX_COLUNAS]) {
    for (int i = 0; i < tamanho_frase; i++) {
        for (int linha = 0; linha < 5; linha++) {
            for (int coluna = 0; coluna < 5; coluna++) {
                frase_concatenada[linha][(i * 5) + coluna] = frase[i][linha * 5 + coluna];
            }
        }
    }
}

/**
 * Exibe uma frase com efeito de rolagem na matriz de LEDs, começando com o display vazio.
 * @param texto Texto a ser exibido.
 * @param cor Cor do texto.
 * @param pio Instância do PIO.
 * @param sm Estado da máquina de estados.
 * @param intensidade Intensidade do texto.
 * @param velocidade Velocidade da rolagem.
 * @param tamanho_matriz Tamanho da matriz (3 ou 5).
 */
void exibir_frase_rolagem(const char *texto, RGBColor cor, PIO pio, uint sm, double intensidade, int velocidade, int tamanho_matriz) {
    // Verifica se o texto é válido
    if (!texto) {
        printf("Erro: Texto inválido!\n");
        return;
    }

    // Cria a lista de frames a partir da string
    double **frase = criar_frase_da_string(texto, tamanho_matriz);
    if (!frase) {
        printf("Erro ao criar a frase!\n");
        return;
    }

    int tamanho_frase = strlen(texto) + 1; // Apenas um espaço adicional no final

    // Define o número de colunas por letra com base no tamanho da matriz
    int colunas_por_letra = (tamanho_matriz == 3) ? 3 : 5;

    // Calcula a largura total da frase concatenada
    int largura_frase = colunas_por_letra * tamanho_frase;

    // Verifica se a frase é muito longa
    if (largura_frase > MAX_COLUNAS) {
        printf("Erro: Frase muito longa!\n");
        free(frase);
        return;
    }

    // Cria o array concatenado da frase
    double frase_concatenada[5][largura_frase]; // 5 linhas, largura variável
    memset(frase_concatenada, 0, sizeof(double) * 5 * largura_frase); // Inicializa com zeros

    // Concatena os frames das letras e dos espaços
    for (int i = 0; i < tamanho_frase; i++) {
        for (int linha = 0; linha < 5; linha++) {
            for (int coluna = 0; coluna < colunas_por_letra; coluna++) {
                frase_concatenada[linha][(i * colunas_por_letra) + coluna] = frase[i][linha * colunas_por_letra + coluna];
            }
        }
    }

    // Exibe a frase com rolagem, começando com o display vazio
    for (int posicao_inicial = -5; posicao_inicial < largura_frase; posicao_inicial++) {
        // Cria um frame temporário para exibir na matriz
        double frame[5][5] = {0}; // Sempre 5x5 para exibição

        // Preenche o frame com a "janela" atual da frase concatenada
        for (int linha = 0; linha < 5; linha++) {
            for (int coluna = 0; coluna < 5; coluna++) {
                int coluna_frase = posicao_inicial + coluna; // Calcula a coluna na frase concatenada
                if (coluna_frase >= 0 && coluna_frase < largura_frase) {
                    frame[linha][coluna] = frase_concatenada[linha][coluna_frase];
                } else {
                    frame[linha][coluna] = 0; // Fora dos limites, preenche com 0 (LEDs apagados)
                }
            }
        }

        // Exibe o frame na matriz de LEDs
        exibir_frame(&frame[0][0], cor, pio, sm, intensidade);

        // Verifica se o botão foi pressionado (exemplo)
        if (!gpio_get(JSTICK)) {
            reset_usb_boot(0, 0);
        }

        // Aguarda um tempo para controlar a velocidade da rolagem
        sleep_ms(velocidade);
    }

    // Libera a memória alocada para a frase
    free(frase);
}

#endif
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "init_GPIO.h"
#include "frames_5x5.h"
#include "frames_3x5.h"
#include "letras_5x5.h"
#include "letras_3x5.h"
#include "led_functions.h"
#include "botao_joystick.h"
#include "direcao.h"
#include "hardware/adc.h"
#include "menu.h"
#include "main.pio.h"

// Variáveis do PIO e sm
PIO pio;
uint sm, offset;

/**
 * Inicializa o sistema, configurando o clock e a PIO.
 * @param pio Ponteiro para a instância do PIO.
 * @param sm Ponteiro para o estado da máquina de estados (state machine).
 * @param offset Ponteiro para o offset do programa PIO.
 * @return true se a inicialização for bem-sucedida, false caso contrário.
 */
bool inicializar_sistema(PIO *pio, uint *sm, uint *offset) {
    // Configura o clock do sistema para 128 MHz
    if (!set_sys_clock_khz(128000, false)) {
        printf("Erro ao configurar o clock do sistema!\n");
        return false;
    }

    // Configuração da PIO
    *pio = pio0;
    *offset = pio_add_program(*pio, &main_program);
    *sm = pio_claim_unused_sm(*pio, true);

    if (*offset == -1 || *sm == -1) {
        printf("Erro ao configurar a PIO!\n");
        return false;
    }

    // Inicializa o programa PIO na matriz de LEDs
    main_program_init(*pio, *sm, *offset, OUT_PIN);
    return true;
}

/**
 * Função principal do programa.
 * Inicializa o sistema, exibe animações e texto na matriz de LEDs.
 */
int main() {
    // Inicializa o stdio (para debug via USB, se necessário)
    stdio_init_all();
    adc_init();
    adc_gpio_init(26); // init ADC pin 26
    adc_gpio_init(27); // init ADC pin 27

    // Inicializa o sistema (clock e PIO)
    if (!inicializar_sistema(&pio, &sm, &offset)) {
        return 1; // Encerra o programa se a inicialização falhar
    }

    // Inicia o botão joystick para inversão de eixo
    botao_init(JSTICK);
    // Inicia o botão B para bootsel_mode
    botao_init(BOTAO_B);
    botao_init(BOTAO_A);

    // Define a cor (R, G, B - 0 à 255)
    RGBColor cor = {0, 100, 255}; // Cor personalizada (R=219, G=0, B=91)

    // Inicia a tela
    menu_init();

    // Loop infinito
    while (1) {
        // Lê os valores dos eixos X e Y do joystick
        adc_select_input(0);
        int y = adc_read();  // Lê o valor do eixo Y (pino 26)
        adc_select_input(1);
        int x = adc_read();  // Lê o valor do eixo X (pino 27)

        // Atualiza o menu ou executa a função correspondente ao estado atual
        switch (estado_atual) {
            case ESTADO_MENU_PRINCIPAL:
            case ESTADO_MENU_MATRIX:
                menu_update(x, y, false); // Atualiza o menu com base na direção
                break;
            case ESTADO_EXIBIR_FRASE:
                display_frase(); // Exibe a frase
                estado_atual = ESTADO_MENU_MATRIX; // Volta ao submenu após exibir a frase
                break;
            case ESTADO_EXIBIR_ANIMACAO:
                display_animacao(); // Exibe a animação
                estado_atual = ESTADO_MENU_MATRIX; // Volta ao submenu após exibir a animação
                break;
            case ESTADO_AJUSTAR_BRILHO:
                ajustar_intensidade_leds(x, y); // Ajusta o brilho
                break;
        }

        sleep_ms(120);  // Adiciona um pequeno delay para evitar leituras excessivas
    }
}
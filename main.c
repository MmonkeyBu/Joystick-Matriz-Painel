#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "main.pio.h"
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

    // Inicia a tela
    menu_init();

    // Inicia o botão joystick para inversão de eixo
    botao_init(JSTICK);
    // Inicia o botão B para bootsel_mode
    botao_init(BOTAO_B);
    botao_init(BOTAO_A);

    // Variáveis para PIO
    PIO pio;
    uint sm, offset;

    // Inicializa o sistema (clock e PIO)
    if (!inicializar_sistema(&pio, &sm, &offset)) {
        return 1; // Encerra o programa se a inicialização falhar
    }

    // Define a cor (R, G, B - 0 à 255)
    RGBColor cor = {200, 0, 50}; // Cor personalizada (R=219, G=0, B=91)

    // Variável de controle qualquer (não utilizada no momento)
    uint32_t cont = 0;

    // Define a intensidade dos LEDs (0.0 a 1.0)
    double intensidade = 0.1; // Controle da intensidade (0.0 até 1.0)
    uint16_t vel = 150;       // Taxa de atualizações em ms
    uint8_t largura_fonte = 5; // Tamanho da fonte (3 ou 5)

    // Exibe uma frase com efeito de rolagem na matriz de LEDs
    exibir_frase_rolagem("CARLOS VIADO", cor, pio, sm, intensidade, vel, largura_fonte);

    // Loop infinito
    while (1) {
        // Lê os valores dos eixos X e Y do joystick
        adc_select_input(0);
        int y = adc_read();  // Lê o valor do eixo Y (pino 26)
        adc_select_input(1);
        int x = adc_read();  // Lê o valor do eixo X (pino 27)

        // Funções do Joystick
        
        //print_direction(x, y, estado_inverter);  // Imprime a direção no console
        menu_update(x, y, false);               // Atualiza o menu com base na direção
        //detectar_gesto(x, y);                 // Detecta gestos (se necessário)
        //porcentagem(x, y);                    // Exibe a porcentagem dos eixos (se necessário)

        sleep_ms(120);  // Adiciona um pequeno delay para evitar leituras excessivas
    }
}
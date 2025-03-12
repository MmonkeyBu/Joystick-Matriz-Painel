#include "pico/bootrom.h"//configuração para teste do bootseel :P
#include "pico/stdlib.h"
#include "botao_joystick.h"
#include "menu.h"


// Protótipos das funções
void botao_callback(uint gpio, uint32_t eventos);
void botao_init(uint8_t pino);

// Variáveis para debounce
static volatile uint32_t ultima_interrupcao_a = 0;
static volatile uint32_t ultima_interrupcao_b = 0;
static volatile uint32_t ultima_interrupcao_joystick = 0;

// Função de callback para tratar interrupções dos botões
void botao_callback(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    
    switch (gpio) {
        case BOTAO_B:
            if (tempo_atual - ultima_interrupcao_b > DEBOUNCE_TIME) {
                ultima_interrupcao_b = tempo_atual;
                if (eventos & GPIO_IRQ_EDGE_FALL) {
                    reset_usb_boot(0, 0); // Entra no modo bootsel
                }
            }
            break;
        
        case JSTICK:
            if (tempo_atual - ultima_interrupcao_joystick > DEBOUNCE_TIME) {
                ultima_interrupcao_joystick = tempo_atual;
                if (eventos & GPIO_IRQ_EDGE_FALL) {
                    estado_inverter = !estado_inverter; // Inverte o estado do joystick
                }
            }
            break;

        case BOTAO_A:
            if (tempo_atual - ultima_interrupcao_a > DEBOUNCE_TIME) {
                ultima_interrupcao_a = tempo_atual;
                if (eventos & GPIO_IRQ_EDGE_FALL) {
                    menu_handle_button_a(); // Alterna entre o menu principal e o submenu
                }
            }
            break;
    
        default:
            break;
    }
}

// Função para inicializar um botão
void botao_init(uint8_t pino) {
    gpio_init(pino);
    gpio_set_dir(pino, GPIO_IN);
    gpio_pull_up(pino);
    gpio_set_irq_enabled_with_callback(pino, GPIO_IRQ_EDGE_FALL, true, &botao_callback);
}
#include "pico/bootrom.h"  // Configuração para teste do bootsel
#include "pico/stdlib.h"
#include "botao_joystick.h"
#include "menu.h"

// Variáveis para debounce
static volatile uint32_t ultima_interrupcao_a = 0;
static volatile uint32_t ultima_interrupcao_b = 0;
static volatile uint32_t ultima_interrupcao_joystick = 0;

// Variáveis globais para armazenar os valores de x e y do joystick
extern int x;  // Declarado em outro arquivo (por exemplo, main.c)
extern int y;  // Declarado em outro arquivo (por exemplo, main.c)
extern volatile bool estado_inverter;  // Declarado em outro arquivo (por exemplo, main.c)

// Função de callback para tratar interrupções dos botões
void botao_callback(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    
    switch (gpio) {
        case BOTAO_B:
            if (tempo_atual - ultima_interrupcao_b > DEBOUNCE_TIME) {
                ultima_interrupcao_b = tempo_atual;
                if (eventos & GPIO_IRQ_EDGE_FALL) {
                    printf("Botão B pressionado\n");
                    reset_usb_boot(0, 0); // Entra no modo bootsel
                }
            }
            break;
        
        case JSTICK:
            if (tempo_atual - ultima_interrupcao_joystick > DEBOUNCE_TIME) {
                ultima_interrupcao_joystick = tempo_atual;
                if (eventos & GPIO_IRQ_EDGE_FALL) {
                    printf("Botão do joystick pressionado\n");
                    estado_inverter = !estado_inverter; // Inverte o estado do joystick
                }
            }
            break;

        case BOTAO_A:
            if (tempo_atual - ultima_interrupcao_a > DEBOUNCE_TIME) {
                ultima_interrupcao_a = tempo_atual;
                if (eventos & GPIO_IRQ_EDGE_FALL) {
                    printf("Botão A pressionado\n");
                    menu_handle_button_a(x, y, estado_inverter);
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
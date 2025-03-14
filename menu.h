#ifndef MENU_H
#define MENU_H

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "direcao.h"
#include "botao_joystick.h"
#include "main.pio.h"
#include "led_functions.h"
#include "hardware/clocks.h"

// Variáveis para PIO
extern PIO pio;
extern uint sm, offset;

// Enumeração para os estados do sistema
typedef enum {
    ESTADO_MENU_PRINCIPAL,   // Menu principal
    ESTADO_MENU_MATRIX,       // Submenu da matriz de LEDs
    ESTADO_EXIBIR_FRASE,      // Exibindo frase
    ESTADO_EXIBIR_ANIMACAO,   // Exibindo animação
    ESTADO_AJUSTAR_BRILHO     // Ajustando brilho
} EstadoSistema;

// Declaração da variável de estado
extern EstadoSistema estado_atual;

// Protótipos das funções
void menu_init();
void menu_update(int x, int y, bool estado_inverter);
void menu_handle_button_a(int x, int y, bool estado_inverter);
int menu_get_selected_option();

// Funções de exibição no OLED
void display_directions(int x, int y, bool estado_inverter);
void display_percentages(int x, int y);
void display_gestures(int x, int y);

// Funções de exibição na matriz de LEDs
void display_frase();
void display_animacao();
void ajustar_intensidade_leds(int x, int y);

#endif // MENU_H
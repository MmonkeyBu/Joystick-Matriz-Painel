#ifndef MENU_H
#define MENU_H

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "direcao.h"
#include "botao_joystick.h"
#include "main.pio.h"
#include "led_functions.h"

// Variáveis para PIO
extern PIO pio;
extern uint sm, offset;

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

#endif // MENU_H
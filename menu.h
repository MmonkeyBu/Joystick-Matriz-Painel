#ifndef MENU_H
#define MENU_H

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "direcao.h"

// Inicializa o menu
void menu_init();
void menu_update(int x, int y, bool estado_inverter);
int menu_get_selected_option();
void menu_handle_button_a();  // Declaração da função para tratar o botão A

#endif // MENU_H


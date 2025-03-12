#include "menu.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "direcao.h"  // Inclui o arquivo de direções
#include "botao_joystick.h"
#include <string.h>


// Definições
#define NUM_OPTIONS_MAIN 2              // Número de opções no menu principal
#define NUM_OPTIONS_SUB 3               // Número de opções no submenu
#define MENU_OPTION_SPACING 12          // Espaçamento vertical entre as opções
#define MENU_CURSOR_POSITION 10         // Posição horizontal do cursor (seta ">")

// Variáveis globais
static const char *menu_options_main[NUM_OPTIONS_MAIN] = {"Joysticks", "Matrix de Leds"}; // Opções do menu principal
static const char *menu_options_joysticks[NUM_OPTIONS_SUB] = {"Direcoes", "Porcentagens", "Gestos"}; // Submenu de Joysticks
static const char *menu_options_matrix[NUM_OPTIONS_SUB] = {"Opção M1", "Opção M2", "Opção M3"}; // Submenu de Matrix de Leds
static int selected_option = 0;         // Índice da opção selecionada
static bool in_sub_menu = false;        // Indica se está no submenu
static bool is_joysticks_selected = false; // Indica se o submenu de Joysticks está selecionado

// Função para desenhar o menu no display
static void menu_draw(const char *options[], int num_options) {
    ssd1306_Fill(Black);  // Limpa o display

    // Itera sobre as opções do menu
    for (int i = 0; i < num_options; i++) {
        // Define a posição do cursor para cada opção
        ssd1306_SetCursor(MENU_CURSOR_POSITION, i * MENU_OPTION_SPACING);

        // Desenha a seta ">" para a opção selecionada
        if (i == selected_option) {
            ssd1306_WriteString("> ", Font_7x10, White);
        } else {
            ssd1306_WriteString("  ", Font_7x10, White);
        }

        // Desenha o texto da opção
        ssd1306_WriteString(options[i], Font_7x10, White);
    }

    ssd1306_UpdateScreen();  // Atualiza o display
}

// Função para inicializar o menu
void menu_init() {
    ssd1306_Init();  // Inicializa o display
    menu_draw(menu_options_main, NUM_OPTIONS_MAIN);  // Desenha o menu principal inicial
}

// Função para atualizar o menu com base na direção
void menu_update(int x, int y, bool estado_inverter) {
    // Depuração: imprime os valores de x, y e estado_inverter
    printf("Valores de x: %d, y: %d, estado_inverter: %d\n", x, y, estado_inverter);

    // Obtém a direção atual usando a função print_direction
    Direction dir = print_direction(x, y, estado_inverter);

    // Depuração: imprime a direção detectada
    printf("Direção detectada: %d\n", dir);

    // Atualiza a opção selecionada com base na direção
    switch (dir) {
        case CIMA:
            selected_option = (selected_option - 1 + (in_sub_menu ? NUM_OPTIONS_SUB : NUM_OPTIONS_MAIN)) % (in_sub_menu ? NUM_OPTIONS_SUB : NUM_OPTIONS_MAIN);  // Move para cima
            break;
        case BAIXO:
            selected_option = (selected_option + 1) % (in_sub_menu ? NUM_OPTIONS_SUB : NUM_OPTIONS_MAIN);  // Move para baixo
            break;
        case ESQUERDA:
            // Adicione lógica para a direção ESQUERDA, se necessário
            break;
        case DIREITA:
            // Adicione lógica para a direção DIREITA, se necessário
            break;
        case DIREITA_CIMA:
        case DIREITA_BAIXO:
        case ESQUERDA_CIMA:
        case ESQUERDA_BAIXO:
            // Adicione lógica para direções diagonais, se necessário
            break;
        default:
            // Nada a fazer para CENTRALIZADO ou outras direções não tratadas
            break;
    }

    // Depuração: imprime a opção selecionada após a atualização
    printf("Opção selecionada após atualização: %d\n", selected_option);

    // Redesenha o menu atual
    if (in_sub_menu) {
        if (is_joysticks_selected) {
            menu_draw(menu_options_joysticks, NUM_OPTIONS_SUB);  // Desenha o submenu de Joysticks
        } else {
            menu_draw(menu_options_matrix, NUM_OPTIONS_SUB);  // Desenha o submenu de Matrix de Leds
        }
    } else {
        menu_draw(menu_options_main, NUM_OPTIONS_MAIN);  // Desenha o menu principal
    }
}

void menu_handle_button_a() {
    if (in_sub_menu) {
        // Se estiver no submenu, volta ao menu principal
        in_sub_menu = false;
        menu_draw(menu_options_main, NUM_OPTIONS_MAIN);
    } else {
        // Se estiver no menu principal, entra no submenu correspondente
        in_sub_menu = true;

        // Verifica qual opção do menu principal foi selecionada
        int selected_main_option = menu_get_selected_option();
        printf("Opção selecionada no menu principal: %d\n", selected_main_option);

        if (selected_main_option >= 0 && selected_main_option < NUM_OPTIONS_MAIN) {
            const char *selected_option_text = menu_options_main[selected_main_option];
            printf("Texto da opção selecionada: %s\n", selected_option_text);

            if (strcmp(selected_option_text, "Joysticks") == 0) {
                is_joysticks_selected = true;  // Entra no submenu de Joysticks
                printf("Submenu de Joysticks selecionado\n");
                menu_draw(menu_options_joysticks, NUM_OPTIONS_SUB);
            } else if (strcmp(selected_option_text, "Matrix de Leds") == 0) {
                is_joysticks_selected = false;  // Entra no submenu de Matrix de Leds
                printf("Submenu de Matrix de Leds selecionado\n");
                menu_draw(menu_options_matrix, NUM_OPTIONS_SUB);
            } else {
                printf("Opção desconhecida selecionada\n");
            }
        } else {
            printf("Índice de opção inválido: %d\n", selected_main_option);
        }
    }
}

// Função para obter a opção selecionada
int menu_get_selected_option() {
    return selected_option;
}
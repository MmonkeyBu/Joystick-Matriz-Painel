#include "menu.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "direcao.h"
#include "botao_joystick.h"
#include <string.h>

// Definições
#define NUM_OPTIONS_MAIN 2              // Número de opções no menu principal
#define NUM_OPTIONS_SUB_1 4             // Número de opções no submenu
#define NUM_OPTIONS_SUB_2 4             // Número de opções no submenu da matriz de LEDs (agora com 4 opções)
#define MENU_OPTION_SPACING 12          // Espaçamento vertical entre as opções
#define MENU_CURSOR_POSITION 10         // Posição horizontal do cursor (seta ">")

// Variáveis locais
RGBColor cor = {200, 0, 50};
int x = 0;
int y = 0;
static bool exibindo_funcao = false; // Controla se o sistema está exibindo uma função

// Variáveis globais
static const char *menu_options_main[NUM_OPTIONS_MAIN] = {"Joysticks", "Matrix de Leds"}; // Opções do menu principal
static const char *menu_options_joysticks[NUM_OPTIONS_SUB_1] = {"Direcoes", "Porcentagens", "Gestos", "Voltar"}; // Submenu de Joysticks
static const char *menu_options_matrix[NUM_OPTIONS_SUB_2] = {"Frase", "Animacao", "Ajustar Brilho", "Voltar"}; // Submenu de Matrix de Leds (nova opção)
static int selected_option = 0;         // Índice da opção selecionada
static bool in_sub_menu = false;        // Indica se está no submenu
static bool is_joysticks_selected = false; // Indica se o submenu de Joysticks está selecionado

// Variável global para armazenar o estado atual do sistema
EstadoSistema estado_atual = ESTADO_MENU_PRINCIPAL;

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

    // Se estiver no modo de exibição, apenas atualiza as funções de exibição
    if (exibindo_funcao) {
        if (is_joysticks_selected) {
            switch (selected_option) {
                case 0: // Direcoes
                    display_directions(x, y, estado_inverter);
                    break;
                case 1: // Porcentagens
                    display_percentages(x, y);
                    break;
                case 2: // Gestos
                    display_gestures(x, y);
                    break;
                default:
                    break;
            }
        } else {
            switch (selected_option) {
                case 0: // Frase
                    display_frase();
                    break;
                case 1: // Animacao
                    display_animacao();
                    break;
                case 2: // Ajustar Brilho
                    ajustar_intensidade_leds(x, y);
                    break;
                default:
                    break;
            }
        }
        return; // Ignora a navegação pelo menu
    }

    // Obtém a direção atual usando a função print_direction
    Direction dir = print_direction(x, y, estado_inverter);

    // Depuração: imprime a direção detectada
    printf("Direção detectada: %d\n", dir);

    // Define o número de opções com base no menu atual
    int num_opcoes = in_sub_menu ? (is_joysticks_selected ? NUM_OPTIONS_SUB_1 : NUM_OPTIONS_SUB_2) : NUM_OPTIONS_MAIN;

    // Atualiza a opção selecionada com base na direção
    switch (dir) {
        case CIMA:
            selected_option = (selected_option - 1 + num_opcoes) % num_opcoes;  // Move para cima
            break;
        case BAIXO:
            selected_option = (selected_option + 1) % num_opcoes;  // Move para baixo
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
            menu_draw(menu_options_joysticks, NUM_OPTIONS_SUB_1);
        } else {
            menu_draw(menu_options_matrix, NUM_OPTIONS_SUB_2);
        }
    } else {
        menu_draw(menu_options_main, NUM_OPTIONS_MAIN);
    }
}

// Função para ajustar a intensidade dos LEDs
void ajustar_intensidade_leds(int x, int y) {
    static double intensidade = 0.1; // Valor inicial da intensidade

    // Ajusta a intensidade com base no eixo Y do joystick
    if (x > MID + DEADZONE) {
        intensidade += intensidade < 1.0 ? 0.01 : 0.0; // Aumenta a intensidade
        if (intensidade > 1.0) intensidade = 1.0; // Limita o valor máximo
    } else if (x < MID - DEADZONE) {
        intensidade -= intensidade > 0.01 ? 0.01 : 0.0; // Diminui a intensidade
    }

    // Exibe a intensidade atual no OLED
    ssd1306_Fill(Black);
    ssd1306_SetCursor(10, 10);
    char intensidade_str[20];
    snprintf(intensidade_str, sizeof(intensidade_str), "Brilho: %.2f", intensidade);
    ssd1306_WriteString(intensidade_str, Font_7x10, White);
    ssd1306_UpdateScreen();

    // Exibe o frame f_02 (todos os LEDs acesos) com a intensidade ajustada
    exibir_frame(f_02, cor, pio, sm, intensidade);
}

// Função para exibir uma frase na matriz de LEDs
void display_frase() {
    printf("Exibindo frase...\n");
    const char *frase = "OI";
    RGBColor cor = {200, 0, 50};
    double intensidade = 0.1;
    int velocidade = 100;
    int tamanho_matriz = 5;

    exibir_frase_rolagem(frase, cor, pio, sm, intensidade, velocidade, tamanho_matriz);
}

void display_animacao() {
    printf("Exibindo animação...\n");
    RGBColor cor = {200, 0, 50};
    double intensidade = 0.1;
    int velocidade = 500, cont = 0;

    while (cont < 5) {
        exibir_frame(heart_01, cor, pio, sm, intensidade);
        sleep_ms(velocidade);
        exibir_frame(heart_02, cor, pio, sm, intensidade);
        sleep_ms(velocidade);

        if (!gpio_get(BOTAO_A)) {
            printf("Botão A pressionado, saindo da animação...\n");
            break;
        }
        cont++;
    }

    exibir_frame(f_01, cor, pio, sm, intensidade);
}

// Função para tratar o botão A
void menu_handle_button_a(int x, int y, bool estado_inverter) {
    if (exibindo_funcao) {
        // Se estiver exibindo uma função, volta ao submenu
        exibindo_funcao = false;
        estado_atual = ESTADO_MENU_MATRIX; // Volta ao submenu da matriz
        return;
    }

    if (in_sub_menu) {
        // Verifica se a opção "Voltar" foi selecionada
        if ((is_joysticks_selected && strcmp(menu_options_joysticks[selected_option], "Voltar") == 0) ||
            (!is_joysticks_selected && strcmp(menu_options_matrix[selected_option], "Voltar") == 0)) {
            in_sub_menu = false; // Volta ao menu principal
            selected_option = 0; // Reseta a seleção para a primeira opção
            estado_atual = ESTADO_MENU_PRINCIPAL; // Altera o estado para o menu principal
            menu_draw(menu_options_main, NUM_OPTIONS_MAIN);
            printf("Voltando ao menu principal...\n");
        } else {
            // Se estiver no submenu de Joysticks, entra no modo de exibição
            if (is_joysticks_selected) {
                exibindo_funcao = true; // Entra no modo de exibição
                switch (selected_option) {
                    case 0: // Direcoes
                        display_directions(x, y, estado_inverter);
                        break;
                    case 1: // Porcentagens
                        display_percentages(x, y);
                        break;
                    case 2: // Gestos
                        display_gestures(x, y);
                        break;
                    default:
                        break;
                }
            } else {
                // Se estiver no submenu da Matriz, entra no modo de exibição
                exibindo_funcao = true; // Entra no modo de exibição
                switch (selected_option) {
                    case 0: // Frase
                        estado_atual = ESTADO_EXIBIR_FRASE; // Altera o estado para exibir frase
                        break;
                    case 1: // Animacao
                        estado_atual = ESTADO_EXIBIR_ANIMACAO; // Altera o estado para exibir animação
                        break;
                    case 2: // Ajustar Brilho
                        estado_atual = ESTADO_AJUSTAR_BRILHO; // Altera o estado para ajustar brilho
                        break;
                    default:
                        break;
                }
            }
        }
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
                menu_draw(menu_options_joysticks, NUM_OPTIONS_SUB_1);
            } else if (strcmp(selected_option_text, "Matrix de Leds") == 0) {
                is_joysticks_selected = false;  // Entra no submenu de Matrix de Leds
                printf("Submenu de Matrix de Leds selecionado\n");
                menu_draw(menu_options_matrix, NUM_OPTIONS_SUB_2);
            } else {
                printf("Opção desconhecida selecionada\n");
            }
        } else {
            printf("Índice de opção inválido: %d\n", selected_main_option);
        }
    }
}

// Função para exibir as direções no OLED
void display_directions(int x, int y, bool estado_inverter) {
    ssd1306_Fill(Black);  // Limpa o display

    // Obtém a direção atual
    Direction dir = print_direction(x, y, estado_inverter);

    // Define a posição do cursor para exibir a direção
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("Direcao: ", Font_7x10, White);

    // Exibe a direção no OLED
    switch (dir) {
        case CIMA:
            ssd1306_WriteString("Cima", Font_7x10, White);
            break;
        case BAIXO:
            ssd1306_WriteString("Baixo", Font_7x10, White);
            break;
        case ESQUERDA:
            ssd1306_WriteString("Esquerda", Font_7x10, White);
            break;
        case DIREITA:
            ssd1306_WriteString("Direita", Font_7x10, White);
            break;
        case DIREITA_CIMA:
            ssd1306_WriteString("Dir-Cima", Font_7x10, White);
            break;
        case DIREITA_BAIXO:
            ssd1306_WriteString("Dir-Baixo", Font_7x10, White);
            break;
        case ESQUERDA_CIMA:
            ssd1306_WriteString("Esq-Cima", Font_7x10, White);
            break;
        case ESQUERDA_BAIXO:
            ssd1306_WriteString("Esq-Baixo", Font_7x10, White);
            break;
        default:
            ssd1306_WriteString("Centralizado", Font_7x10, White);
            break;
    }

    ssd1306_UpdateScreen();  // Atualiza o display
}

// Função para exibir as porcentagens no OLED
void display_percentages(int x, int y) {
    ssd1306_Fill(Black);  // Limpa o display

    // Calcula as porcentagens
    int x_processado = (x > MID + DEADZONE || x < MID - DEADZONE) ? x : MID;
    int y_processado = (y > MID + DEADZONE || y < MID - DEADZONE) ? y : MID;

    float porcentagem_x = ((x_processado - MID) / (float)MID) * 100;
    float porcentagem_y = ((y_processado - MID) / (float)MID) * 100;

    // Define a posição do cursor para exibir as porcentagens
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("X: ", Font_7x10, White);
    ssd1306_SetCursor(30, 10);
    char x_str[10];
    snprintf(x_str, sizeof(x_str), "%.2f%%", porcentagem_x);
    ssd1306_WriteString(x_str, Font_7x10, White);

    ssd1306_SetCursor(10, 20);
    ssd1306_WriteString("Y: ", Font_7x10, White);
    ssd1306_SetCursor(30, 20);
    char y_str[10];
    snprintf(y_str, sizeof(y_str), "%.2f%%", porcentagem_y);
    ssd1306_WriteString(y_str, Font_7x10, White);

    ssd1306_UpdateScreen();  // Atualiza o display
}

// Função para exibir os gestos no OLED
void display_gestures(int x, int y) {
    ssd1306_Fill(Black);  // Limpa o display

    // Detecta o gesto
    detectar_gesto(x, y);

    // Define a posição do cursor para exibir o gesto
    ssd1306_SetCursor(10, 10); // Posição X, Y (ajuste conforme necessário)
    ssd1306_WriteString("Gesto:", Font_7x10, White);

    // Exibe o gesto no OLED
    Direction gesto = identificar_gesto();
    switch (gesto) {
        case GESTO_VOLTA_COMPLETA:
            ssd1306_SetCursor(10, 25); // Posição X, Y (ajuste conforme necessário)
            ssd1306_WriteString("Volta completa", Font_7x10, White);
            break;
        case GESTO_MEIA_LUA_DIREITA:
            ssd1306_SetCursor(10, 25); // Posição X, Y (ajuste conforme necessário)
            ssd1306_WriteString("Meia-Dir", Font_7x10, White);
            break;
        case GESTO_MEIA_LUA_ESQUERDA:
            ssd1306_SetCursor(10, 25); // Posição X, Y (ajuste conforme necessário)
            ssd1306_WriteString("Meia-lua Esq", Font_7x10, White);
            break;
        default:
            ssd1306_SetCursor(10, 25); // Posição X, Y (ajuste conforme necessário)
            ssd1306_WriteString("Nenhum", Font_7x10, White);
            break;
    }

    ssd1306_UpdateScreen();  // Atualiza o display
}

// Função para obter a opção selecionada
int menu_get_selected_option() {
    return selected_option;
}
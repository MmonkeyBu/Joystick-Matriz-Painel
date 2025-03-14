#include "direcao.h"
#include "botao_joystick.h"
#include "hardware/adc.h"

volatile bool estado_inverter = false;

static Posicao historico[lISTA_TAM];
static int index_hist = 0;

Direction print_direction(int x, int y, bool estado_inverter) {
    if (!estado_inverter) {
        y = ADC_MAX - y;
    }

    if (x > MID + DEADZONE) {
        if (y > MID + DEADZONE) {
            printf("\r↘ Direita-Baixo\n");
            return DIREITA_BAIXO;
        } else if (y < MID - DEADZONE) {
            printf("\r↗ Direita-Cima\n");
            return DIREITA_CIMA;
        } else {
            printf("\r→ Direita\n");
            return DIREITA;
        }
    } else if (x < MID - DEADZONE) {
        if (y > MID + DEADZONE) {
            printf("\r↙ Esquerda-Baixo\n");  // Imprime antes de retornar
            return ESQUERDA_BAIXO;
        } else if (y < MID - DEADZONE) {
            printf("\r↖ Esquerda-Cima\n");  // Imprime antes de retornar
            return ESQUERDA_CIMA;
        } else {
            printf("\r← Esquerda\n");  // Imprime antes de retornar
            return ESQUERDA;
        }
    } else {
        if (y > MID + DEADZONE) {
            printf("\r↓ Baixo\n");
            return BAIXO;
        } else if (y < MID - DEADZONE) {
            printf("\r↑ Cima\n");
            return CIMA;
        } else {
            printf("\r○ Centralizado\n");
            return CENTRALIZADO;
        }
    }
}

//Leitor de Gestos

void atualizar_historico(int x, int y) {
    historico[index_hist].x = x;
    historico[index_hist].y = y;
    index_hist = (index_hist + 1) % lISTA_TAM;
}

Direction identificar_gesto() {
    bool direita = false, esquerda = false, cima = false, baixo = false;

    for (int i = 0; i < lISTA_TAM; i++) {
        if (historico[i].x > MID + DEADZONE) direita = true;
        if (historico[i].x < MID - DEADZONE) esquerda = true;
        if (historico[i].y > MID + DEADZONE) baixo = true;
        if (historico[i].y < MID - DEADZONE) cima = true;
    }

    if (direita && esquerda && cima && baixo) {
        return GESTO_VOLTA_COMPLETA;
    }
    if (direita && baixo && cima && !esquerda) {
        return GESTO_MEIA_LUA_DIREITA;
    }
    if (esquerda && baixo && cima && !direita) {
        return GESTO_MEIA_LUA_ESQUERDA;
    }

    return GESTO_NENHUM;
}

void detectar_gesto(int x, int y) {
    atualizar_historico(x, y);
    Direction gesto = identificar_gesto();

    switch (gesto) {
        case GESTO_VOLTA_COMPLETA:
            printf("\rVolta Completa Detectada\n");
            break;
        case GESTO_MEIA_LUA_DIREITA:
            printf("\rMeia Lua Direita Detectada\n");
            break;
        case GESTO_MEIA_LUA_ESQUERDA:
            printf("\rMeia Lua Esquerda Detectada\n");
            break;
        default:
            break;
    }
}

//Porcentagem dos eixos

void porcentagem(int x, int y) {
    int x_processado = (x > MID + DEADZONE || x < MID - DEADZONE) ? x : MID;
    int y_processado = (y > MID + DEADZONE || y < MID - DEADZONE) ? y : MID;

    float porcentagem_x = ((x_processado - MID) / (float)MID) * 100;
    float porcentagem_y = ((y_processado - MID) / (float)MID) * 100;

    printf("\rEixo X: %.2f%% | Eixo Y: %.2f%%    \n", porcentagem_x, porcentagem_y);
    fflush(stdout); 
}

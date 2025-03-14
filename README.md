# Projeto de Integração de Joystick e Matriz de LEDs

![Badge](https://img.shields.io/badge/Status-Em%20Desenvolvimento-yellow)  
![Badge](https://img.shields.io/badge/Licença-MIT-blue)  
![Badge](https://img.shields.io/badge/Version-1.0.0-green)

Este projeto é uma demonstração da integração entre uma biblioteca de controle de matriz de LEDs e uma biblioteca de joystick, desenvolvida para o Raspberry Pi Pico W. O sistema permite a navegação em um menu via joystick, exibição de animações, frases e ajuste de brilho na matriz de LEDs.

---

## 🚀 Funcionalidades

### 1. **Menu de Navegação**
   - **Menu Principal**: Escolha entre "Joysticks" e "Matrix de Leds".
   - **Submenu Joysticks**:
     - **Direções**: Exibe a direção atual do joystick.
     - **Porcentagens**: Mostra a porcentagem de movimento nos eixos X e Y.
     - **Gestos**: Detecta gestos como "Volta Completa" e "Meia Lua".
     - **Voltar**: Retorna ao menu principal.
   - **Submenu Matrix de Leds**:
     - **Frase**: Exibe uma frase com efeito de rolagem na matriz de LEDs.
     - **Animação**: Reproduz uma animação na matriz de LEDs.
     - **Ajustar Brilho**: Permite ajustar o brilho dos LEDs usando o joystick.
     - **Voltar**: Retorna ao menu principal.

### 2. **Controle de Joystick**
   - Leitura dos eixos X e Y para navegação no menu.
   - Inversão de eixos via botão do joystick.
   - Detecção de gestos como "Volta Completa" e "Meia Lua".

### 3. **Matriz de LEDs**
   - Exibição de frames, animações e frases com rolagem.
   - Ajuste de brilho dos LEDs.
   - Suporte para matrizes de LEDs 3x5 e 5x5.

### 4. **Botões**
   - **Botão A**: Confirma seleção no menu e interage com funcionalidades.
   - **Botão B**: Entra no modo bootsel para programação do Pico.
   - **Botão do Joystick**: Inverte os eixos X e Y.

---

## 🛠️ Tecnologias Utilizadas

- **Raspberry Pi Pico W**: Microcontrolador principal.
- **C/C++**: Linguagem de programação utilizada.
- **PIO (Programmable I/O)**: Para controle da matriz de LEDs.
- **ADC (Analog-to-Digital Converter)**: Para leitura dos eixos do joystick.
- **SSD1306**: Display OLED para exibição do menu.

---

## 📋 Pré-requisitos

Antes de começar, verifique se você tem os seguintes requisitos instalados:

- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
- [CMake](https://cmake.org/)
- [GCC Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

---

## �️ Instalação

Siga os passos abaixo para instalar e rodar o projeto localmente:

1. Clone o repositório:
```bash
git clone https://github.com/seu-usuario/nome-do-repositorio.git
```

2. Acesse a pasta do projeto:

```
cd nome-do-repositorio
```

3. Configure o ambiente de compilação:

```
mkdir build
cd build
cmake ..
```

4. Compile o projeto:
```
make
```

5. Conecte o Raspberry Pi Pico no modo bootsel e copie o arquivo .uf2 gerado:
```
cp projeto.uf2 /media/seu-usuario/RPI-RP2/
```
## 🎮 Como Usar
Navegação no Menu:

Use o joystick para navegar entre as opções do menu.

Pressione o Botão A para selecionar uma opção.

Pressione o Botão do Joystick para inverter os eixos X e Y.

Exibição de Frases:

No submenu "Matrix de Leds", selecione "Frase" para exibir uma frase com efeito de rolagem.

Reprodução de Animação:

No submenu "Matrix de Leds", selecione "Animação" para reproduzir uma animação na matriz de LEDs.

Ajuste de Brilho:

No submenu "Matrix de Leds", selecione "Ajustar Brilho" e use o joystick para ajustar o brilho dos LEDs.

Detecção de Gestos:

No submenu "Joysticks", selecione "Gestos" e movimente o joystick para detectar gestos como "Volta Completa" e "Meia Lua".

📂 Estrutura do Projeto
main.c: Contém a lógica principal do programa, inicialização do sistema e loop infinito.

menu.c: Implementa a navegação no menu e exibição no display OLED.

botao_joystick.c: Controla os botões e o joystick.

direcao.c: Implementa a leitura dos eixos do joystick e detecção de gestos.

led_functions.c: Contém funções para controle da matriz de LEDs, incluindo exibição de frames, animações e frases.

frames_*.c: Contém os frames para exibição na matriz de LEDs.

letras_*.c: Contém os frames das letras para exibição de frases.

📝 Licença
Este projeto está licenciado sob a licença MIT. Consulte o arquivo LICENSE para mais detalhes.

🤝 Contribuição
Contribuições são bem-vindas! Sinta-se à vontade para abrir issues e pull requests.

🙏 Agradecimentos
BitDogLab: Pelo suporte e recursos para desenvolvimento.

Colega de Trabalho: Pela biblioteca do joystick.

Feito com ❤️ por [Seu Nome].
📧 Contato: seu-email@example.com

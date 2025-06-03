/**
 * @file led_functions.h
 * @brief LED Matrix Control Library
 * 
 * Provides functions to control RGB LED matrices using PIO
 */

 #ifndef LED_FUNCTIONS_H
 #define LED_FUNCTIONS_H
 
 #include <ctype.h>
 #include <math.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "pico/stdlib.h"
 #include "main.pio.h"
 #include "pico/bootrom.h"
 #include "hardware/pio.h"
 #include "inc/define_pins.h"
 #include "inc/drivers/frames_3x5.h"
 #include "inc/drivers/frames_5x5.h"
 #include "inc/drivers/letras_3x5.h"
 #include "inc/drivers/letras_5x5.h"
 #include "inc/joystick.h"
 
 #define NUM_LEDS 25                         // Number of LEDs in matrix
 #define MAX_PHRASE_LENGTH 50                // Maximum characters in phrase  
 #define MAX_COLUMNS (5 * MAX_PHRASE_LENGTH) // Maximum columns in concatenated phrase
 
 /**
  * @brief RGB color structure
  */
 typedef struct {
     double r; // Red (0.0 to 1.0)
     double g; // Green (0.0 to 1.0)
     double b; // Blue (0.0 to 1.0)
 } RGBColor;
 
 /**
  * @brief Character enumeration for font mapping
  */
 typedef enum {
     CHAR_A, CHAR_B, CHAR_C, CHAR_D, CHAR_E, CHAR_F, CHAR_G, CHAR_H, CHAR_I, CHAR_J,
     CHAR_K, CHAR_L, CHAR_M, CHAR_N, CHAR_O, CHAR_P, CHAR_Q, CHAR_R, CHAR_S, CHAR_T,
     CHAR_U, CHAR_V, CHAR_W, CHAR_X, CHAR_Y, CHAR_Z, CHAR_SPACE, CHAR_EXCLAMATION, CHAR_PERIOD
 } Character;
 
 // Function declarations
 bool init_system(PIO *pio, uint *sm, uint *offset);
 uint32_t encode_rgb(double b, double r, double g);
 void convert_color_to_scale(RGBColor *color);
 int map_logical_to_physical(int index);
 void set_led(int index, RGBColor color, PIO pio, uint sm);
 double **create_phrase_frames(const char *text, int matrix_size);
 void display_joystick_frame(JoystickState state, RGBColor color, PIO pio, uint sm, double intensity);
 void display_frame(double *frame, RGBColor color, PIO pio, uint sm, double intensity);
 void concatenate_phrase(double *phrase[], int phrase_length, double concatenated_phrase[5][MAX_COLUMNS]);
 void scroll_text(const char *text, RGBColor color, PIO pio, uint sm, double intensity, int speed, int matrix_size);
 
 #endif
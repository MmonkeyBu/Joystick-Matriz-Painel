/**
 * @file joystick.h
 * @brief Joystick driver for RP2040 (Pico SDK)
 * 
 * Provides joystick position, direction, and intensity level detection
 */

 #ifndef JOYSTICK_H
 #define JOYSTICK_H
 
 #include <stdlib.h>
 #include <stdint.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 #include "timers_callback.h"
 #include <math.h>
 
 // Hardware pin definitions
 #define JSTYCK_BTN 22  ///< Joystick button GPIO pin
 #define VRY 26         ///< Vertical axis (Y) ADC input pin
 #define VRX 27         ///< Horizontal axis (X) ADC input pin
 
 // ADC calibration constants
 #define ADC_CENTER 2096  ///< Neutral position ADC value
 #define ADC_MIN 11       ///< Minimum ADC value at full deflection 
 #define ADC_MAX 4077     ///< Maximum ADC value at full deflection
 #define DEADZONE_PERCENT 10  ///< Deadzone percentage around center
 
 /**
  * @brief Joystick direction enumeration
  */
 typedef enum {
     CENTER,      ///< Neutral position
     UP,          ///< Up direction
     DOWN,        ///< Down direction
     LEFT,        ///< Left direction
     RIGHT,       ///< Right direction
     UP_LEFT,     ///< Up-left diagonal
     UP_RIGHT,    ///< Up-right diagonal
     DOWN_LEFT,   ///< Down-left diagonal
     DOWN_RIGHT   ///< Down-right diagonal
 } JoystickDirection;
 
 /**
  * @brief Intensity level enumeration
  */
 typedef enum {
     NONE,        ///< No significant deflection
     LOW,         ///< Moderate deflection
     HIGH         ///< Full deflection
 } JoystickLevel;
 
 /**
  * @brief Joystick state structure
  */
 typedef struct {
     uint16_t x_raw;          ///< Raw X-axis ADC value
     uint16_t y_raw;          ///< Raw Y-axis ADC value
     int16_t x_calibrated;    ///< Calibrated X position (-100% to +100%)
     int16_t y_calibrated;    ///< Calibrated Y position (-100% to +100%)
     JoystickDirection direction;  ///< Current direction
     JoystickLevel x_level;   ///< X-axis intensity level
     JoystickLevel y_level;   ///< Y-axis intensity level
 } JoystickState;
 
 // Function prototypes
 void joystick_init(void);
 void joystick_update(JoystickState *state);
 int16_t joystick_calibrate_value(uint16_t raw, uint16_t center, uint16_t min, uint16_t max);
 JoystickDirection joystick_get_direction(JoystickState *state);
 void joystick_get_levels(JoystickState *state);
 
 #endif // JOYSTICK_H
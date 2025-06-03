    /**
     * @file joystick.c
     * @brief Joystick driver implementation
     */

    #include "inc/joystick.h"

    /**
     * @brief Initialize joystick hardware
     * 
     * Configures ADC channels for analog inputs and GPIO for digital button
     */
    void joystick_init(void) {
        adc_init();
        adc_gpio_init(VRX);
        adc_gpio_init(VRY);
        
        // Configure button pin with pull-up
        gpio_pin_init(JSTYCK_BTN);
    }
    
    /**
     * @brief Update joystick state
     * @param state Pointer to JoystickState structure to populate
     */
    void joystick_update(JoystickState *state) {
        // Read raw ADC values
        adc_select_input(0);
        state->y_raw = adc_read();  // Y-axis
        adc_select_input(1);
        state->x_raw = adc_read();  // X-axis
        
        // Convert raw values to calibrated percentages
        state->x_calibrated = joystick_calibrate_value(state->x_raw, ADC_CENTER, ADC_MIN, ADC_MAX);
        state->y_calibrated = joystick_calibrate_value(state->y_raw, ADC_CENTER, ADC_MIN, ADC_MAX);
    
        // Update derived state
        state->direction = joystick_get_direction(state);
        joystick_get_levels(state);
    }
    
    /**
     * @brief Convert raw ADC value to calibrated percentage
     * @param raw ADC reading
     * @param center Neutral position value
     * @param min Minimum value at full deflection
     * @param max Maximum value at full deflection
     * @return Calibrated value (-100% to +100%)
     */
    int16_t joystick_calibrate_value(uint16_t raw, uint16_t center, uint16_t min, uint16_t max) {
        if (raw < center) {
            return -100 * (center - raw) / (center - min);
        } else {
            return 100 * (raw - center) / (max - center);
        }
    }
    
    /**
     * @brief Determine joystick direction
     * @param state Pointer to current joystick state
     * @return Detected JoystickDirection
     * 
     * @note Uses 50% threshold for direction detection
     */
    JoystickDirection joystick_get_direction(JoystickState *state) {
        const int16_t threshold = 30; // 50% deflection threshold
        
        // Axis deflection states
        bool up = state->y_calibrated > threshold;
        bool down = state->y_calibrated < -threshold;
        bool left = state->x_calibrated < -threshold;
        bool right = state->x_calibrated > threshold;
    
        // Check diagonal combinations first
        if (up && left) return UP_LEFT;
        if (up && right) return UP_RIGHT;
        if (down && left) return DOWN_LEFT;
        if (down && right) return DOWN_RIGHT;
        
        // Check primary directions
        if (up) return UP;
        if (down) return DOWN;
        if (left) return LEFT;
        if (right) return RIGHT;
        
        return CENTER;
    }
    
    /**
     * @brief Determine joystick intensity levels
     * @param state Pointer to joystick state (updates x_level/y_level)
     * 
     * @note Uses 5% (LOW) and 99% (HIGH) thresholds
     */
    void joystick_get_levels(JoystickState *state) {
        // Intensity thresholds
        const int16_t low_thresh = 2;   // 5% deflection for LOW level
        const int16_t high_thresh = 90; // 99% deflection for HIGH level
        
        // X-axis level detection
        int16_t x_abs = abs(state->x_calibrated);
        state->x_level = (x_abs > high_thresh) ? HIGH : 
                        (x_abs > low_thresh) ? LOW : NONE;
        
        // Y-axis level detection
        int16_t y_abs = abs(state->y_calibrated);
        state->y_level = (y_abs > high_thresh) ? HIGH : 
                        (y_abs > low_thresh) ? LOW : NONE;
    }
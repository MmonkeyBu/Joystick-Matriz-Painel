/**
 * @file led_functions.c
 * @brief LED Matrix Control Functions Implementation
 *
 * Contains complete implementation for controlling RGB LED matrices using PIO
 */

#include "inc/led_functions.h"

/**
 * @brief Initializes system clock and PIO
 * @param pio Pointer to PIO instance
 * @param sm Pointer to state machine index
 * @param offset Pointer to program offset
 * @return true if successful, false otherwise
 */
bool init_system(PIO *pio, uint *sm, uint *offset)
{
    // Configure system clock to 128 MHz
    if (!set_sys_clock_khz(128000, false))
    {
        printf("Error configuring system clock!\n");
        return false;
    }

    // PIO configuration
    *pio = pio0;
    *offset = pio_add_program(*pio, &main_program);
    *sm = pio_claim_unused_sm(*pio, true);

    if (*offset == -1 || *sm == -1)
    {
        printf("Error configuring PIO!\n");
        return false;
    }

    // Initialize PIO program
    main_program_init(*pio, *sm, *offset, OUT_PIN);
    return true;
}

/**
 * @brief Encodes RGB color into 32-bit format (G | R | B)
 * @param b Blue component (0.0 to 1.0)
 * @param r Red component (0.0 to 1.0)
 * @param g Green component (0.0 to 1.0)
 * @return Encoded 32-bit color value
 */
uint32_t encode_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255; // Convert to 0-255
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8); // G | R | B format
}

/**
 * @brief Converts color from 0-255 range to 0.0-1.0 scale
 * @param color Pointer to RGBColor structure
 */
void convert_color_to_scale(RGBColor *color)
{
    // Clamp values to 0-255 range
    color->r = (color->r > 255) ? 255 : (color->r < 0) ? 0
                                                       : color->r;
    color->g = (color->g > 255) ? 255 : (color->g < 0) ? 0
                                                       : color->g;
    color->b = (color->b > 255) ? 255 : (color->b < 0) ? 0
                                                       : color->b;

    // Convert to 0.0-1.0 scale
    color->r /= 255.0;
    color->g /= 255.0;
    color->b /= 255.0;
}

/**
 * @brief Maps logical LED index to physical position
 * @param index Logical LED index
 * @return Physical LED position
 */
int map_logical_to_physical(int index)
{
    int row = 4 - (index / 5); // Row (0-4, bottom to top)
    int col = 4 - (index % 5); // Column (0-4, mirrored)

    // Reverse column for odd rows (1 and 3)
    if (row == 1 || row == 3)
    {
        col = 4 - col;
    }

    return (row * 5) + col;
}

/**
 * @brief Sets an individual LED color
 * @param index LED index (0-24)
 * @param color RGB color structure
 * @param pio PIO instance
 * @param sm State machine index
 */
void set_led(int index, RGBColor color, PIO pio, uint sm)
{
    uint32_t led_value = encode_rgb(color.b, color.r, color.g);
    pio_sm_put_blocking(pio, sm, led_value);
}

/**
 * @brief Creates frame array from input string
 * @param text Input text string
 * @param matrix_size Matrix dimension (3 or 5)
 * @return Pointer to frame array
 */
double **create_phrase_frames(const char *text, int matrix_size)
{
    int max_chars = strlen(text);
    double **phrase = (double **)malloc((max_chars + 1) * sizeof(double *));

    double **font = (matrix_size == 3) ? letras_3x5 : letras_5x5;
    double *space = (matrix_size == 3) ? f_01_fino : f_01;

    for (int i = 0; i < max_chars; i++)
    {
        char c = toupper(text[i]);

        switch (c)
        {
        case 'A':
            phrase[i] = font[CHAR_A];
            break;
        // ... all other character cases ...
        default:
            phrase[i] = font[CHAR_SPACE];
            break;
        }
    }

    phrase[max_chars] = font[CHAR_SPACE];
    return phrase;
}

/**
 * @brief Displays the LED frame corresponding to joystick position/intensity
 * @param state Joystick state (direction + intensity)
 * @param color LED color
 * @param pio PIO instance
 * @param sm State machine
 * @param intensity Brightness level (0.0-1.0)
 */
void display_joystick_frame(JoystickState state, RGBColor color, PIO pio, uint sm, double intensity)
{
    double *frame = note_01; // default frame (neutral)

    switch (state.direction)
    {
    case UP:
        frame = (state.y_level == HIGH) ? note_03 : note_02;
        break;
    case UP_RIGHT:
        frame = (state.x_level == HIGH) ? note_05 : note_04;
        break;
    case RIGHT:
        frame = (state.x_level == HIGH) ? note_07 : note_06;
        break;
    case DOWN_RIGHT:
        frame = (state.y_level == HIGH) ? note_09 : note_08;
        break;
    case DOWN_LEFT:
        frame = (state.y_level == HIGH) ? note_11 : note_10;
        break;
    case LEFT:
        frame = (state.x_level == HIGH) ? note_13 : note_12;
        break;
    case UP_LEFT:
        frame = (state.x_level == HIGH) ? note_15 : note_14;
        break;

    default: // CENTER
        frame = note_01;
    }

    display_frame(frame, color, pio, sm, intensity);
}

/**
 * @brief Displays a single frame on LED matrix
 * @param frame Frame data array
 * @param color Display color
 * @param pio PIO instance
 * @param sm State machine index
 * @param intensity Brightness intensity (0.0-1.0)
 */
void display_frame(double *frame, RGBColor color, PIO pio, uint sm, double intensity)
{
    intensity = (intensity < 0.0) ? 0.0 : (intensity > 1.0) ? 1.0
                                                            : intensity;
    convert_color_to_scale(&color);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        int physical_pos = map_logical_to_physical(i);
        RGBColor led_color = {
            color.r * frame[physical_pos] * intensity,
            color.g * frame[physical_pos] * intensity,
            color.b * frame[physical_pos] * intensity};
        set_led(i, led_color, pio, sm);
    }
}

/**
 * @brief Concatenates character frames into single phrase
 * @param phrase Array of character frames
 * @param phrase_length Number of characters
 * @param concatenated_phrase Output matrix for concatenation
 */
void concatenate_phrase(double *phrase[], int phrase_length,
                        double concatenated_phrase[5][MAX_COLUMNS])
{
    int cols_per_char = (phrase[0] == letras_3x5[0]) ? 3 : 5;

    for (int i = 0; i < phrase_length; i++)
    {
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < cols_per_char; col++)
            {
                concatenated_phrase[row][(i * cols_per_char) + col] =
                    phrase[i][row * cols_per_char + col];
            }
        }
    }
}

/**
 * @brief Displays scrolling text on LED matrix
 * @param text Text to display
 * @param color Text color
 * @param pio PIO instance
 * @param sm State machine index
 * @param intensity Display intensity
 * @param speed Scrolling speed (ms per frame)
 * @param matrix_size Matrix dimension (3 or 5)
 */
void scroll_text(const char *text, RGBColor color, PIO pio, uint sm,
                 double intensity, int speed, int matrix_size)
{
    if (!text)
    {
        printf("Error: Invalid text input!\n");
        return;
    }

    double **phrase = create_phrase_frames(text, matrix_size);
    if (!phrase)
    {
        printf("Error creating phrase frames!\n");
        return;
    }

    int phrase_length = strlen(text) + 1;
    int cols_per_char = (matrix_size == 3) ? 3 : 5;
    int total_width = cols_per_char * phrase_length;

    if (total_width > MAX_COLUMNS)
    {
        printf("Error: Phrase too long!\n");
        free(phrase);
        return;
    }

    double concatenated[5][total_width];
    memset(concatenated, 0, sizeof(double) * 5 * total_width);

    concatenate_phrase(phrase, phrase_length, concatenated);

    for (int start_pos = -5; start_pos < total_width; start_pos++)
    {
        double current_frame[5][5] = {0};

        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 5; col++)
            {
                int src_col = start_pos + col;
                if (src_col >= 0 && src_col < total_width)
                {
                    current_frame[row][col] = concatenated[row][src_col];
                }
            }
        }

        display_frame(&current_frame[0][0], color, pio, sm, intensity);

        if (!gpio_get(JSTICK))
        {
            reset_usb_boot(0, 0);
        }

        sleep_ms(speed);
    }

    free(phrase);
}
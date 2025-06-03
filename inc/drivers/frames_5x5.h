/**
 * @file frames_5x5.h
 * @brief LED Matrix Frame Definitions for 5x5 Displays
 * 
 * Contains predefined patterns for LED matrices including:
 * - Blank/Full frames
 * - Heart animations
 * - Musical note representations
 */

 #ifndef FRAMES_5X5_H
 #define FRAMES_5X5_H
 
 #define NUM_LEDS 25  // 5x5 matrix
 
 // Basic frames
 extern double f_01[NUM_LEDS];  // All LEDs off
 extern double f_02[NUM_LEDS];  // All LEDs on
 
 // Heart animations
 extern double heart_01[NUM_LEDS];  // Small heart
 extern double heart_02[NUM_LEDS];  // Large heart
 
 // Musical notes (DO, RE, MI, etc.)
 extern double note_01[NUM_LEDS];  // None/rest
 extern double note_02[NUM_LEDS];  // DO
 extern double note_03[NUM_LEDS];  // do (higher octave)
 extern double note_04[NUM_LEDS];  // RE
 extern double note_05[NUM_LEDS];  // re
 extern double note_06[NUM_LEDS];  // MI
 extern double note_07[NUM_LEDS];  // mi
 extern double note_08[NUM_LEDS];  // FA
 extern double note_09[NUM_LEDS];  // fa
 extern double note_10[NUM_LEDS];  // SOL
 extern double note_11[NUM_LEDS];  // sol
 extern double note_12[NUM_LEDS];  // LA
 extern double note_13[NUM_LEDS];  // la
 extern double note_14[NUM_LEDS];  // SI
 extern double note_15[NUM_LEDS];  // si
 
 // Array of all note frames
 extern double *all_notes[15];
 
 #endif // FRAMES_5X5_H
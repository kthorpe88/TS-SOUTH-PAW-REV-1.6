#ifndef RGB_EFFECTS_H
#define RGB_EFFECTS_H

#include QMK_KEYBOARD_H

// LED indices for Caps Lock, Num Lock, and ripple effects
#define CAPS_LOCK_LED 60
#define NUM_LOCK_LED 3

// Function declarations for handling RGB effects
void handle_caps_lock_rgb(void); // Handle Caps Lock RGB effect
void handle_num_lock_rgb(void); // Handle Num Lock RGB effect
void handle_esc_ripple_effect(void); // Handle ESC ripple effect
void start_esc_ripple_effect(void); // Start ESC ripple effect
void stop_esc_ripple_effect(void); // Stop ESC ripple effect
uint8_t read_battery_level(void); // Function declaration for reading battery level

#endif // RGB_EFFECTS_H

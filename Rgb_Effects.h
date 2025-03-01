#ifndef RGB_EFFECTS_H
#define RGB_EFFECTS_H

#include QMK_KEYBOARD_H

// LED indices for Caps Lock, Num Lock, and ripple effects
#define CAPS_LOCK_LED 60
#define NUM_LOCK_LED 3

// Function declarations for handling RGB effects
void handle_caps_lock_rgb(void);
void handle_num_lock_rgb(void);
void handle_esc_ripple_effect(void);
void start_esc_ripple_effect(void);
void stop_esc_ripple_effect(void);

#endif // RGB_EFFECTS_H

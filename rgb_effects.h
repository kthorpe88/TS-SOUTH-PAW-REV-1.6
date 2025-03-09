// Header guard to prevent multiple inclusions of this header file
#ifndef RGB_EFFECTS_H
#define RGB_EFFECTS_H

// Include the QMK keyboard library
#include QMK_KEYBOARD_H

// External variable declarations
// Flags for pulse effects on Caps Lock and Num Lock
extern bool pulse_active_caps;
extern bool pulse_active_num;

// Previous RGB modes for Caps Lock and Num Lock
extern uint8_t prev_rgb_mode_caps;
extern uint8_t prev_rgb_mode_num;

// Flags for ESC key and ripple effect
extern bool is_esc_active;
extern bool is_esc_ripple_active;

// USB connection status
extern bool is_usb_connected;

// LED indices for Num Lock and ripple effects
#define NUM_LOCK_LED 3 // LED index for Num Lock

// Function declarations for handling RGB effects
void handle_caps_lock_rgb(void); // Handle Caps Lock RGB effect
void handle_num_lock_rgb(void); // Handle Num Lock RGB effect
void handle_esc_ripple_effect(void); // Handle ESC ripple effect
void start_esc_ripple_effect(void); // Start ESC ripple effect
void stop_esc_ripple_effect(void); // Stop ESC ripple effect

#endif // RGB_EFFECTS_H

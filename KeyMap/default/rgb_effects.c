#include "rgb_effects.h"

// RGB values for custom lighting
#define CAPS_LOCK_COLOR_RED 255
#define CAPS_LOCK_COLOR_GREEN 60
#define CAPS_LOCK_COLOR_BLUE 0 // Orange for Caps Lock

#define NUM_LOCK_COLOR_RED 255
#define NUM_LOCK_COLOR_GREEN 60
#define NUM_LOCK_COLOR_BLUE 0 // Orange for Num Lock

#define ESC_RIPPLE_COLOR_RED 255
#define ESC_RIPPLE_COLOR_GREEN 0
#define ESC_RIPPLE_COLOR_BLUE 0 // Red for ESC ripple effect

// Ripple effect variables for ESC
static bool is_esc_active = false;
static uint8_t ripple_step = 0;
static uint16_t ripple_timer = 0;
static bool ripple_expanding = true;

// Caps Lock and Num Lock pulsing variables
static uint8_t prev_rgb_mode_caps = 0;
static uint8_t prev_rgb_mode_num = 0;
static bool pulse_active_caps = false;
static bool pulse_active_num = false;

// Ripple configuration for ESC
const uint8_t esc_splash_ripple[][10] = {
    {0},
	{1, 19}, 
	{38, 39, 6, 7}, 
	{42, 43, 25, 48, 49},
    {80, 81, 67, 68, 75, 76}, 
	{102, 103, 71, 36, 18, 93}
};
const uint8_t ripple_layers = sizeof(esc_splash_ripple) / sizeof(esc_splash_ripple[0]);

void handle_caps_lock_rgb(void) {
    if (host_keyboard_led_state().caps_lock) {
        if (!pulse_active_caps) {
            prev_rgb_mode_caps = rgb_matrix_get_mode();
            pulse_active_caps = true;
        }
        uint8_t pulse_brightness = abs((timer_read() / 2) % 510 - 255);
        rgb_matrix_set_color(CAPS_LOCK_LED,
                             (CAPS_LOCK_COLOR_RED * pulse_brightness) / 255,
                             (CAPS_LOCK_COLOR_GREEN * pulse_brightness) / 255,
                             (CAPS_LOCK_COLOR_BLUE * pulse_brightness) / 255);
    } else if (pulse_active_caps) {
        pulse_active_caps = false;
        rgb_matrix_mode_noeeprom(prev_rgb_mode_caps);
    }
}

void handle_num_lock_rgb(void) {
    if (!host_keyboard_led_state().num_lock) {
        if (!pulse_active_num) {
            prev_rgb_mode_num = rgb_matrix_get_mode();
            pulse_active_num = true;
        }
        uint8_t pulse_brightness = abs((timer_read() / 2) % 510 - 255);
        rgb_matrix_set_color(NUM_LOCK_LED,
                             (NUM_LOCK_COLOR_RED * pulse_brightness) / 255,
                             (NUM_LOCK_COLOR_GREEN * pulse_brightness) / 255,
                             (NUM_LOCK_COLOR_BLUE * pulse_brightness) / 255);
    } else if (pulse_active_num) {
        pulse_active_num = false;
        rgb_matrix_mode_noeeprom(prev_rgb_mode_num);
    }
}

void start_esc_ripple_effect(void) {
    is_esc_active = true;
    ripple_step = 0;
    ripple_timer = timer_read();
    ripple_expanding = true;
}

void stop_esc_ripple_effect(void) {
    is_esc_active = false;
}

void handle_esc_ripple_effect(void) {
    if (is_esc_active) {
        if (timer_elapsed(ripple_timer) > 15) {
            ripple_timer = timer_read();
            ripple_step = ripple_expanding ? ripple_step + 1 : ripple_step - 1;
            if (ripple_step >= ripple_layers) {
                ripple_step = ripple_layers - 1;
                ripple_expanding = false;
            } else if (ripple_step == 0) {
                ripple_expanding = true;
            }
        }

        for (uint8_t layer = 0; layer < ripple_layers; layer++) {
            for (uint8_t i = 0; i < sizeof(esc_splash_ripple[layer]) / sizeof(esc_splash_ripple[layer][0]); i++) {
                if (layer == ripple_step) {
                    rgb_matrix_set_color(esc_splash_ripple[layer][i],
                                         ESC_RIPPLE_COLOR_RED,
                                         ESC_RIPPLE_COLOR_GREEN,
                                         ESC_RIPPLE_COLOR_BLUE);
                } else {
                    rgb_matrix_set_color(esc_splash_ripple[layer][i], 0, 0, 0);
                }
            }
        }
    }
}

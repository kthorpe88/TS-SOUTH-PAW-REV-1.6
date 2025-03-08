#include "rgb_effects.h" // Include custom RGB effects header

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

#define RGB_MATRIX_TIMEOUT 300000 // 5 minutes
#define RGB_MATRIX_SLEEP // Enable sleep mode
#define RGB_LOW_BATTERY_THRESHOLD 20 // Percentage

// Ripple effect variables for ESC
static bool is_esc_active = false; // ESC ripple effect active flag
static uint8_t ripple_step = 0; // Current ripple step
static uint16_t ripple_timer = 0; // Timer for ripple effect
static bool ripple_expanding = true; // Ripple expanding flag

// Caps Lock and Num Lock pulsing variables
static uint8_t prev_rgb_mode_caps = 0; // Previous RGB mode for Caps Lock
static uint8_t prev_rgb_mode_num = 0; // Previous RGB mode for Num Lock
static bool pulse_active_caps = false; // Caps Lock pulsing active flag
static bool pulse_active_num = false; // Num Lock pulsing active flag

// Ripple configuration for ESC
const uint8_t esc_splash_ripple[][10] = {
    {0},
    {1, 19}, 
    {38, 39, 6, 7}, 
    {42, 43, 25, 48, 49},
    {80, 81, 67, 68, 75, 76}, 
    {102, 103, 71, 36, 18, 93}
};
const uint8_t ripple_layers = sizeof(esc_splash_ripple) / sizeof(esc_splash_ripple[0]); // Number of ripple layers

#ifdef RP2040
    // RP2040 specific RGB configurations
#endif

#ifdef BLUETOOTH_ENABLE
    // Bluetooth specific RGB configurations
#endif

#ifdef BATTERY_CHARGING_ENABLE
    // Battery charging specific RGB configurations
#endif

// Function to handle Caps Lock RGB lighting
void handle_caps_lock_rgb(void) {
    if (host_keyboard_led_state().caps_lock) { // Check if Caps Lock is active
        if (!pulse_active_caps) { // If pulsing is not active
            prev_rgb_mode_caps = rgb_matrix_get_mode(); // Save current RGB mode
            pulse_active_caps = true; // Set pulsing active flag
        }
        uint8_t pulse_brightness = abs((timer_read() / 2) % 510 - 255); // Calculate pulsing brightness
        rgb_matrix_set_color(CAPS_LOCK_LED,
                             (CAPS_LOCK_COLOR_RED * pulse_brightness) / 255,
                             (CAPS_LOCK_COLOR_GREEN * pulse_brightness) / 255,
                             (CAPS_LOCK_COLOR_BLUE * pulse_brightness) / 255); // Set Caps Lock LED color
    } else if (pulse_active_caps) { // If Caps Lock is not active and pulsing was active
        pulse_active_caps = false; // Reset pulsing active flag
        rgb_matrix_mode_noeeprom(prev_rgb_mode_caps); // Restore previous RGB mode
    }
}

// Function to handle Num Lock RGB lighting
void handle_num_lock_rgb(void) {
    if (host_keyboard_led_state().num_lock) { // Check if Num Lock is active
        if (!pulse_active_num) { // If pulsing is not active
            prev_rgb_mode_num = rgb_matrix_get_mode(); // Save current RGB mode
            pulse_active_num = true; // Set pulsing active flag
        }
        uint8_t pulse_brightness = abs((timer_read() / 2) % 510 - 255); // Calculate pulsing brightness
        rgb_matrix_set_color(NUM_LOCK_LED,
                             (NUM_LOCK_COLOR_RED * pulse_brightness) / 255,
                             (NUM_LOCK_COLOR_GREEN * pulse_brightness) / 255,
                             (NUM_LOCK_COLOR_BLUE * pulse_brightness) / 255); // Set Num Lock LED color
    } else if (pulse_active_num) { // If Num Lock is not active and pulsing was active
        pulse_active_num = false; // Reset pulsing active flag
        rgb_matrix_mode_noeeprom(prev_rgb_mode_num); // Restore previous RGB mode
    }
}

// Function to start the ESC ripple effect
void start_esc_ripple_effect(void) {
    is_esc_active = true; // Set ESC ripple effect active flag
    ripple_step = 0; // Reset ripple step
    ripple_timer = timer_read(); // Reset ripple timer
    ripple_expanding = true; // Set ripple expanding flag
}

// Function to stop the ESC ripple effect
void stop_esc_ripple_effect(void) {
    is_esc_active = false; // Reset ESC ripple effect active flag
}

// Function to handle the ESC ripple effect
void handle_esc_ripple_effect(void) {
    if (is_esc_active) { // Check if ESC ripple effect is active
        if (timer_elapsed(ripple_timer) > 15) { // Check if it's time to update the ripple effect
            ripple_timer = timer_read(); // Reset ripple timer
            ripple_step = ripple_expanding ? ripple_step + 1 : ripple_step - 1; // Update ripple step
            if (ripple_step >= ripple_layers) { // Check if ripple reached the maximum layer
                ripple_step = ripple_layers - 1; // Set ripple step to the last layer
                ripple_expanding = false; // Set ripple expanding flag to false
            } else if (ripple_step == 0) { // Check if ripple reached the first layer
                ripple_expanding = true; // Set ripple expanding flag to true
            }
        }

        for (uint8_t layer = 0; layer < ripple_layers; layer++) { // Iterate through ripple layers
            for (uint8_t i = 0; i < sizeof(esc_splash_ripple[layer]) / sizeof(esc_splash_ripple[layer][0]); i++) { // Iterate through LEDs in the current layer
                if (layer == ripple_step) { // Check if the current layer is the active ripple step
                    rgb_matrix_set_color(esc_splash_ripple[layer][i],
                                         ESC_RIPPLE_COLOR_RED,
                                         ESC_RIPPLE_COLOR_GREEN,
                                         ESC_RIPPLE_COLOR_BLUE); // Set LED color for the active ripple step
                } else {
                    rgb_matrix_set_color(esc_splash_ripple[layer][i], 0, 0, 0); // Turn off LEDs for other layers
                }
            }
        }
    }
}

// Function to read the battery level
uint8_t read_battery_level(void) {
    uint32_t adc_reading = analogRead(BATTERY_LEVEL_PIN);
    uint8_t battery_percentage = (adc_reading * 100) / 4095; // Adjust if necessary
    return battery_percentage;
}

void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(85, 255, 128); // Default green
}

void indicate_battery_level(void) {
    uint8_t battery_level = read_battery_level();
    if (battery_level <= RGB_LOW_BATTERY_THRESHOLD) {
        rgb_matrix_set_val(60); // Dim to 60/255 when battery low
    }
}

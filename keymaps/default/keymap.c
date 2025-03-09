// Copyright 2025 TS Design Works LLC

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "tssouthpaw/rgb_effects.h" // Include custom RGB effects header
#include QMK_KEYBOARD_H // Include QMK keyboard header
#include "uart.h" // Include UART header
#include "gpio.h" // Include GPIO header
#include <stdio.h> // Include standard I/O header for logging
#include "hardware/gpio.h" // Include GPIO header for RP2040

#define SLEEP_TIMEOUT 300000 // 5 minutes in milliseconds
#define UART_NUM 0 // Define UART number
#define PICO_DEFAULT_LED_PIN 25 // Define default LED pin for RP2040
#define GPIO_OUT 1 // Define GPIO output mode
#define portTICK_PERIOD_MS 1 // Define portTICK_PERIOD_MS macro
#define pdMS_TO_TICKS(ms) ((ms) / portTICK_PERIOD_MS) // Define pdMS_TO_TICKS macro

static uint32_t sleep_timer = 0; // Timer to track inactivity

led_config_t g_led_config = LED_CONFIG; // LED configuration

enum layers {
    BASE, // Base layer
    FN,   // Function layer
};

enum custom_keycodes {
    KC_BATT_LVL = SAFE_RANGE, // Define custom keycode for battery level
    // ...other custom keycodes...
};

extern bool pulse_active_caps;
extern bool pulse_active_num;
extern uint8_t prev_rgb_mode_caps;
extern uint8_t prev_rgb_mode_num;
extern bool is_esc_active;
extern bool is_esc_ripple_active;
extern bool is_usb_connected;

void uart_send_keycode(char command);
int uart_read_bytes(int uart_num, uint8_t *data, int length, int ticks_to_wait);
bool matrix_is_modified(void);
void uart_init(uint32_t baud); // Update to match the correct type

// Define the missing functions
void uart_send_keycode(char command) {
    // Implementation for sending keycode via UART
}

int uart_read_bytes(int uart_num, uint8_t *data, int length, int ticks_to_wait) {
    // Implementation for reading bytes from UART
    return 0; // Placeholder return value
}

bool matrix_is_modified(void) {
    // Implementation for checking if the matrix is modified
    return false; // Placeholder return value
}

void uart_init(uint32_t baud) {
    // Implementation for initializing UART
}

// Function to send commands to ESP32-C6 via UART
void send_command_to_esp32(char command) {
    uart_send_keycode(command);
}

// Function to handle UART communication with ESP32-C6
void handle_uart_communication(void) {
    uint8_t data;
    int len = uart_read_bytes(UART_NUM, &data, 1, pdMS_TO_TICKS(100));
    if (len > 0) {
        // Process received data from ESP32-C6
        switch (data) {
            case 'W': // Wired mode
                rgb_matrix_set_color_all(0, 0, 255); // Set RGB to blue for wired mode
                break;
            case 'B': // Wireless mode
                rgb_matrix_set_color_all(0, 255, 0); // Set RGB to green for wireless mode
                break;
            default:
                printf("RP2040: Received unknown command: 0x%02X\n", data); // Log unknown command
                break;
        }
    }
}

// Function to scan the matrix and handle sleep mode and USB connection
void matrix_scan_user(void) {
    // Check if the sleep timeout has been reached
    if (timer_elapsed32(sleep_timer) > SLEEP_TIMEOUT) {
        rgb_matrix_disable(); // Turn off LEDs
        suspend_power_down(); // QMK suspend function
    }
    // Reset the sleep timer on any keypress
    if (matrix_is_modified()) { // Any keypress resets timer
        sleep_timer = timer_read32();
        rgb_matrix_enable(); // Wake LEDs
        suspend_wakeup_init(); // Wake from suspend
    }
    handle_uart_communication(); // Handle UART communication with ESP32-C6
}

// Function to process custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uart_send_keycode(keycode); // Send keycode via UART

        switch (keycode) {
            case KC_ESC:
                start_esc_ripple_effect(); // Start ESC ripple effect
                tap_code16(KC_ESC); // Send ESC keycode
                return false;

            case KC_MUTE:
                tap_code(KC_MUTE); // Mute audio
                tap_code(KC_MPLY); // Play/pause media
                return false;

            case KC_BATT_LVL:
                // Remove battery level indication
                return false;

            case MO(FN):
                // Remove battery level indication when switching to FN layer
                break;

            default:
                break;
        }
    } else {
        if (keycode == KC_ESC) {
            stop_esc_ripple_effect(); // Stop ESC ripple effect
        }
    }
    return true;
}

// Function to handle RGB matrix indicators
bool rgb_matrix_indicators_user(void) {
    handle_caps_lock_rgb(); // Handle Caps Lock RGB indicator
    handle_num_lock_rgb(); // Handle Num Lock RGB indicator
    handle_esc_ripple_effect(); // Handle ESC ripple effect
    return false; // Allow default QMK handling
}

#ifdef RP2040
void rp2040_specific_init(void) {
    // Add RP2040-specific initialization code here
    // Example: Initialize additional GPIO pins
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1); // Turn on the LED
}
#endif

// Function to initialize the keyboard after startup
void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR); // Set RGB matrix mode to solid color
    rgb_matrix_sethsv(85, 255, 128); // Set RGB color to green, full saturation, 50% brightness
    uart_init(115200); // Initialize UART with baud rate

    #ifdef RP2040
    rp2040_specific_init(); // Call RP2040-specific initialization
    #endif
}

// Keymap definitions
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_ESC, KC_DEL, KC_NUM_LOCK, KC_KB_MUTE, KC_NO, KC_NO, KC_MUTE, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, LGUI(KC_L), 
        KC_BSPC, KC_PSLS, KC_PAST, KC_PMNS, DM_PLY1, DM_PLY2, KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS,KC_EQL, KC_BSPC,       
        KC_P7, KC_P8, KC_P9, KC_PPLS, DM_PLY1, DM_PLY2, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,      
        KC_P4, KC_P5, KC_P6, KC_NO, KC_CALC, KC_PSCR, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NO, KC_ENT,                 
        KC_P1, KC_P2, KC_P3, KC_PENT, KC_NO, KC_UP, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_NO, KC_NO, KC_RSFT,                        
        KC_NO, KC_P0, KC_PDOT, KC_NO, KC_LEFT, KC_DOWN, KC_RIGHT, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, KC_NO, KC_RALT, KC_RGUI, MO(FN), KC_RCTL
    ),
    [FN] = LAYOUT(
        _______, _______, _______, _______, KC_NO  , KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BATT_LVL,
        _______, _______, _______, _______, DM_REC1, DM_REC2, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, DM_REC1, DM_REC2, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO  , _______,
        _______, _______, _______, _______, KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO  , KC_NO  , _______,
        KC_NO,   _______, _______, KC_NO  , _______, _______, _______, _______, _______, KC_NO  , KC_NO  , KC_NO  , _______, KC_NO  , KC_NO  , KC_NO  , _______, _______, _______, _______                      
    ),
};

#ifdef RP2040
    // RP2040 specific keymap configurations
#endif

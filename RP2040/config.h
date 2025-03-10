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

#pragma once

// Set 0 if debouncing isn't needed
#define DEBOUNCE 3

// Dynamic macro configuration
#define DYNAMIC_MACRO_EEPROM_STORAGE // Enable dynamic macro storage in EEPROM
#define DYNAMIC_MACRO_SIZE 128 // Set dynamic macro size

#ifdef RGB_MATRIX_ENABLE
    // RGB Matrix configuration for RP2040
    #define RGB_MATRIX_LED_COUNT 104 // Total number of LEDs
    #define RGB_MATRIX_KEYRELEASES // Reactive effects respond to key releases
    // #define RGB_MATRIX_TIMEOUT 0 // Number of milliseconds to wait until RGB automatically turns off
    // #define RGB_MATRIX_SLEEP // Turn off effects when suspended
    #define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5 // Limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
    #define RGB_MATRIX_LED_FLUSH_LIMIT 10 // Limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120 // Limits maximum brightness of LEDs to 120 out of 255. If not defined, maximum brightness is set to 255
    // #define RGB_MATRIX_DEFAULT_ON true // Sets the default enabled state, if none has been set
    // #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT // Sets the default mode, if none has been set
    // #define RGB_MATRIX_DEFAULT_HUE 0 // Sets the default hue value, if none has been set
    // #define RGB_MATRIX_DEFAULT_SAT 255 // Sets the default saturation value, if none has been set
    // #define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
    #define RGB_MATRIX_DEFAULT_SPD 100 // Sets the default animation speed, if none has been set
    #define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 50 // Delay of temperature decrease for typing heatmap
    #define RGB_MATRIX_TYPING_HEATMAP_SPREAD 40 // Limit the distance the effect spreads to surrounding keys
    #define RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT 16 // Limit how hot surrounding keys get from each press
    #define RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP 32 // Decreasing this value increases the number of keystrokes needed to fully heat up the key
    #define RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE // Enable gradient mode for solid reactive effects
    #define ENABLE_RGB_MATRIX_SOLID_COLOR
    #define ENABLE_RGB_MATRIX_ALPHAS_MODS
    // #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    // #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_BREATHING
    // #define ENABLE_RGB_MATRIX_CYCLE_ALL
    // #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    // #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    // #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    // #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_DUAL_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #define ENABLE_RGB_MATRIX_RAINDROPS
    #define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #define ENABLE_RGB_MATRIX_HUE_BREATHING
    #define ENABLE_RGB_MATRIX_HUE_PENDULUM
    #define ENABLE_RGB_MATRIX_HUE_WAVE
    #define ENABLE_RGB_MATRIX_PIXEL_RAIN
    #define ENABLE_RGB_MATRIX_PIXEL_FLOW
    #define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    #define ENABLE_RGB_MATRIX_TYPING_HEATMAP
    #define ENABLE_RGB_MATRIX_DIGITAL_RAIN
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    #define ENABLE_RGB_MATRIX_SPLASH
    #define ENABLE_RGB_MATRIX_MULTISPLASH
    #define ENABLE_RGB_MATRIX_SOLID_SPLASH
    #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
    #define RGB_MATRIX_EFFECT_PIXEL_RAIN
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL

    // Define LED configuration here
    #define LED_CONFIG                                                                 \
    {                                                                                  \
        {                                                                              \
            /* Row 0 */                                                                \
            {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18}, \
            /* Row 1 */                                                                \
            {19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37}, \
            /* Row 2 */                                                                \
            {38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56}, \
            /* Row 3 */                                                                \
            {57, 58, 59, NO_LED, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, NO_LED, 73}, \
            /* Row 4 */                                                                \
            {74, 75, 76, 77, NO_LED, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, NO_LED, 90}, \
            /* Row 5 */                                                                \
            {NO_LED, 92, 93, NO_LED, 94, 95, 96, 97, NO_LED, 98, 99, 100, 101, 102, 103} \
        },                                                                             \
        {                                                                              \
            /* X and Y coordinates for each LED */                                     \
            {0, 0}, {10, 0}, {20, 0}, {30, 0}, {40, 0}, {50, 0}, {60, 0}, {70, 0},      \
            {80, 0}, {90, 0}, {100, 0}, {110, 0}, {120, 0}, {130, 0}, {140, 0},         \
            {150, 0}, {160, 0}, {170, 0}, {180, 0}, {0, 10}, {10, 10}, {20, 10},        \
            {30, 10}, {40, 10}, {50, 10}, {60, 10}, {70, 10}, {80, 10}, {90, 10},       \
            {100, 10}, {110, 10}, {120, 10}, {130, 10}, {140, 10}, {150, 10},           \
            {160, 10}, {170, 10}, {0, 20}, {10, 20}, {20, 20}, {30, 20}, {40, 20},      \
            {50, 20}, {60, 20}, {70, 20}, {80, 20}, {90, 20}, {100, 20}, {110, 20},     \
            {120, 20}, {130, 20}, {140, 20}, {150, 20}, {160, 20}, {170, 20},           \
            {0, 30}, {10, 30}, {20, 30}, {30, 30}, {40, 30}, {50, 30}, {60, 30},        \
            {70, 30}, {80, 30}, {90, 30}, {100, 30}, {110, 30}, {120, 30}, {130, 30},   \
            {140, 30}, {150, 30}, {160, 30}, {170, 30}                                  \
        },                                                                             \
        {                                                                              \
            /* Brightness level for each LED */                                        \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                   \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                   \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                   \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                   \
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                   \
            1, 1, 1, 1, 1, 1, 1, 1, 1                                                   \
        }                                                                              \
    }

    // RGB-related constants
    #define CAPS_LOCK_LED 60 // LED index for Caps Lock
    #define ESC_RIPPLE_COLOR_RED 255 // Red color for ESC ripple effect
    #define ESC_RIPPLE_COLOR_GREEN 0 // Green color for ESC ripple effect
    #define ESC_RIPPLE_COLOR_BLUE 0 // Blue color for ESC ripple effect
    #define RGB_MATRIX_CAPS_LOCK_COLOR {255, 0, 0} // Red color for Caps Lock
    #define RGB_MATRIX_NUM_LOCK_COLOR {0, 255, 0} // Green color for Num Lock
    #define RGB_MATRIX_SCROLL_LOCK_COLOR {0, 0, 255} // Blue color for Scroll Lock
#endif // RGB_MATRIX_ENABLE 

// Define NOP_FUDGE for ws2812 bitbang driver
#define NOP_FUDGE 1

// UART configuration for RP2040
#define UART_TX_PIN GP0 // Define the TX pin for UART
#define UART_RX_PIN GP1 // Define the RX pin for UART


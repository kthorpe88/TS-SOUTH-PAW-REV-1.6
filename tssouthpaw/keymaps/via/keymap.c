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

#include QMK_KEYBOARD_H // Include QMK keyboard header
#include "hardware/gpio.h" // Include GPIO header for RP2040] 
#define GPIO_OUT 1 // Define GPIO output mode

enum layers {
    BASE, // Base layer
    FN,   // Function layer
};

// Define custom keycode for KC_MICMUTE
enum custom_keycodes {
    KC_MICMUTE = SAFE_RANGE, // Start custom keycodes after safe range
};

// Function to handle rotary encoder updates
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU); // Rotate right: Volume up
    } else {
        tap_code(KC_VOLD); // Rotate left: Volume down
    }
    return true;
}

// Function to handle rotary encoder presses
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // only act on key‐down events
    if (!record->event.pressed) return true;

    switch (keycode) {
        case KC_MUTE:
            tap_code(KC_MUTE);
            tap_code(KC_MPLY);
            return false;  
        case KC_MICMUTE:
            tap_code16(LCTL(LSFT(KC_M))); //TEAMS MIC MUTE 
            return false;
        default:
            return true; 
    }
}

// Keymap definitions
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_ESC,  KC_DEL,  KC_NUM_LOCK, KC_MICMUTE, KC_NO,   KC_NO,   KC_NO,    KC_MUTE, KC_F1,   KC_F2,   KC_F3, KC_F4, KC_F5,  KC_F6, KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  LGUI(KC_L), 
        KC_BSPC, KC_PSLS, KC_PAST,     KC_PMNS,    DM_PLY1, DM_PLY2, KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,  KC_5,  KC_6,   KC_7,  KC_8,  KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_NO,   KC_BSPC,       
        KC_P7,   KC_P8,   KC_P9,       KC_NO,      DM_PLY1, DM_PLY2, KC_NO,    KC_TAB,  KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,   KC_Y,  KC_U,  KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,      
        KC_P4,   KC_P5,   KC_P6,       KC_PPLS,    KC_CALC, KC_PSCR, KC_NO,    KC_CAPS, KC_A,    KC_S,    KC_D,  KC_F,  KC_G,   KC_H,  KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NO,   KC_ENT,                 
        KC_P1,   KC_P2,   KC_P3,       KC_NO,      KC_NO,   KC_UP,   KC_NO,    KC_LSFT, KC_Z,    KC_X,    KC_C,  KC_V,  KC_B,   KC_N,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_NO,   KC_RSFT, KC_NO,                        
        KC_NO,   KC_P0,   KC_PDOT,     KC_PENT,    KC_LEFT, KC_DOWN, KC_RIGHT, KC_NO,   KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO,   KC_RALT, KC_NO,   KC_RGUI, MO(FN),  KC_RCTL
    ),
    [FN] = LAYOUT(
        _______, _______, _______, _______, KC_NO  , KC_NO  , KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, DM_REC1, DM_REC2, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO  , _______,
        _______, _______, _______, KC_NO  , DM_REC1, DM_REC2, KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO  , _______,
        _______, _______, _______, KC_NO  , KC_NO  , _______, KC_NO  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO  , _______, KC_NO  ,
        KC_NO,   _______, _______, _______, _______, _______, _______, KC_NO  , _______, _______, KC_NO  , KC_NO  , KC_NO  , _______, KC_NO  , KC_NO  , _______, KC_NO  , _______, _______, _______                      
    ),
};

#ifdef RP2040
#endif

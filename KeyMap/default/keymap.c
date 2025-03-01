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

#include "rgb_effects.h"
#include QMK_KEYBOARD_H

led_config_t g_led_config = LED_CONFIG;

enum layers {
    BASE,
    FN,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_ESC) {
        if (record->event.pressed) {
            start_esc_ripple_effect();
            tap_code16(KC_ESC); // Send ESC keypress
        } else {
            stop_esc_ripple_effect();
        }
        return false; // Skip default processing
    }
    return true;
}

bool rgb_matrix_indicators_user(void) {
    handle_caps_lock_rgb();
    handle_num_lock_rgb();
    handle_esc_ripple_effect();
    return false; // Allow default QMK handling
}

void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(85, 255, 128); // Green, full saturation, 50% brightness
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR); // Write to EEPROM                                        
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_ESC,  KC_DEL,  KC_CALC, KC_NUM_LOCK, KC_PSCR,          KC_F1,   KC_F2,   KC_F3, KC_F4, KC_F5, KC_F6,  KC_F7, KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  DM_RSTP, LGUI(KC_L), 
        KC_BSPC, KC_PSLS, KC_PAST, KC_PMNS,     KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,  KC_5,  KC_6,  KC_7,   KC_8,  KC_9,    KC_0,   KC_MINS, KC_EQL,           KC_BSPC,    DM_REC1, 
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,     KC_TAB,           KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,  KC_Y,   KC_U,  KC_I,    KC_O,   KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    DM_REC2, 
        KC_P4,   KC_P5,   KC_P6,                KC_CAPS,          KC_A,    KC_S,    KC_D,  KC_F,  KC_G,  KC_H,   KC_J,  KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,              DM_PLY1, 
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,              KC_LSFT, KC_Z,    KC_X,    KC_C,  KC_V,  KC_B,  KC_N,   KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,          KC_UP,      DM_PLY2, 
                 KC_P0,   KC_PDOT,              KC_LCTL, MO(FN),  KC_LGUI, KC_LOPT,                      KC_SPC,                         KC_ROPT, KC_RCTL, KC_LEFT, KC_DOWN,    KC_RGHT
    ),
    [FN] = LAYOUT(
        RESET,   _______, _______, _______, QK_BOOT,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_TOGG, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RM_VALU, 
        _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_VALD, 
        _______, _______, _______,          _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RM_SATU, 
        _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RM_HUEU, RM_SATD, 
                 _______, _______,          _______, _______, _______, _______,                            _______,                            _______, _______, RM_PREV, RM_HUED, RM_NEXT
    ),
};

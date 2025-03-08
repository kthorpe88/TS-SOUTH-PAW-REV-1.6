// Header guard to prevent multiple inclusions of this header file
#ifndef RGB_EFFECTS_H
#define RGB_EFFECTS_H

// Include the QMK keyboard library
#include "qmk_keyboard.h"

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

// Battery status variables
extern uint8_t battery_level;
extern bool is_battery_low;
extern bool is_battery_critical;
extern bool is_battery_charging;

// USB connection status
extern bool is_usb_connected;

// Bluetooth status variables
extern bool is_bt_enabled;
extern bool is_bt_connected;
extern bool is_bt_pairing;
extern bool is_bt_disconnected;
extern bool is_bt_disabled;
extern bool is_bt_unpairing;
extern bool is_bt_pairing_failed;
extern bool is_bt_unpairing_failed;
extern bool is_bt_disconnection_failed;
extern bool is_bt_connection_failed;
extern bool is_bt_enable_failed;
extern bool is_bt_disable_failed;
extern bool is_bt_init_failed;
extern bool is_bt_controller_enable_failed;
extern bool is_bt_controller_disable_failed;
extern bool is_bluedroid_enable_failed;
extern bool is_bluedroid_disable_failed;
extern bool is_hid_device_init_failed;
extern bool is_hid_device_register_callbacks_failed;
extern bool is_hid_device_set_device_name_failed;
extern bool is_hid_device_start_failed;
extern bool is_hid_device_stop_failed;
extern bool is_hid_device_send_report_failed;

// LED indices for Caps Lock, Num Lock, and ripple effects
#define CAPS_LOCK_LED 60 // LED index for Caps Lock
#define NUM_LOCK_LED 3 // LED index for Num Lock

// Function declarations for handling RGB effects
void handle_caps_lock_rgb(void); // Handle Caps Lock RGB effect
void handle_num_lock_rgb(void); // Handle Num Lock RGB effect
void handle_esc_ripple_effect(void); // Handle ESC ripple effect
void start_esc_ripple_effect(void); // Start ESC ripple effect
void stop_esc_ripple_effect(void); // Stop ESC ripple effect
uint8_t read_battery_level(void); // Function declaration for reading battery level

#endif // RGB_EFFECTS_H

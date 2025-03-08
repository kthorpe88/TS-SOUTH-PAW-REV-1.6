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

// ESP32-C6 specific configurations
#ifdef ESP32_C6
    // Bootloader configuration for ESP32-C6
    #define ESP32_C6_BOOTLOADER
    // WiFi and BLE configuration for ESP32-C6
    #define ESP32_C6_WIFI_ENABLE
    #define ESP32_C6_BLE_ENABLE
    // Battery charging configuration for ESP32-C6
    #define ESP32_C6_BATTERY_CHARGING_ENABLE
    #define ESP32_C6_BATTERY_PIN 35 // Example pin for battery voltage monitoring
    // Encoder configuration for ESP32-C6
    #define ENCODERS_PAD_A { GPIO11 } // Updated to match the provided mapping
    #define ENCODERS_PAD_B { GPIO10 } // Updated to match the provided mapping
    // USB configuration for ESP32-C6
    #define ESP32_C6_USBN_PIN GPIO12 // Updated to match the provided mapping
    #define ESP32_C6_USBP_PIN GPIO13 // Updated to match the provided mapping
    // Serial configuration for ESP32-C6
    #define SERIAL_TX_PIN GPIO29 // Updated to match the provided mapping
    #define SERIAL_RX_PIN GPIO30 // Updated to match the provided mapping
#endif

// Bluetooth configurations
#ifdef BLUETOOTH_ENABLE
    // Bluetooth configuration for ESP32-C6
    #define BLE_ENABLE
    #define BLE_HID_ENABLE
    #define BLE_HID_PROFILE 0 // 0 for keyboard, 1 for mouse
    #define BLE_HID_DEVICE_NAME "TS-South-Paw"
    #define BLE_HID_MANUFACTURER "TS Design Works LLC"
    #define BLE_HID_SERIAL_NUMBER "1234567890"
#endif

// Battery charging configurations
#ifdef BATTERY_CHARGING_ENABLE
    // Battery charging configuration for ESP32-C6
    #define BATTERY_CHARGING_PIN GPIO35 // Example pin for battery voltage monitoring
    #define BATTERY_CHARGING_THRESHOLD 3.7 // Voltage threshold for charging
#endif // BATTERY_CHARGING_ENABLE

// Battery level indication configuration for ESP32-C6
#define BATTERY_LEVEL_PIN 35 // Define the pin for battery level monitoring
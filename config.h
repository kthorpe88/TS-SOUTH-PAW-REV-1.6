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
#undef DYNAMIC_MACRO_EEPROM_STORAGE // Disable dynamic macro storage in EEPROM
#define DYNAMIC_MACRO_SIZE 128 // Set dynamic macro size

// Rotary encoder configuration
#define ENCODERS_PAD_A { GP1 }
#define ENCODERS_PAD_B { GP0 }
#define ENCODER_RESOLUTION 4

// Vial dynamic keymap support
#define VIAL_ENABLE
#define VIAL_KEYBOARD_UID {0xB1, 0x22, 0xC3, 0xD4, 0xE5, 0xF6, 0x17, 0x28, 0x39, 0x4A, 0x5B, 0x6C, 0x7D, 0x8E, 0x9F, 0x00}
#define DYNAMIC_KEYMAP_ENABLE
#define DYNAMIC_KEYMAP_LAYER_COUNT 2  // BASE + FN layers
#define RAW_ENABLE
#define RAW_HID_ENABLE

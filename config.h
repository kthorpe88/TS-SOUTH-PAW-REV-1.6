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

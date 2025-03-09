# TS-SOUTH-PAW-REV-1.6

## Description of the keyboard
Fullsize South Paw Mechanical Keyboard,
104 hot swap switches,
Rotary Knob,
RGB key switch lighting,  
Dual MCU - RP-2040 & ESP-32-C6,
Wireless/Bluetooth,
USB-C,
ON-Board LIPO,
Power Management,
Battery Charging.

## Maintainer and hardware information
* Keyboard Maintainer: (TS Design Works LLC)
* Hardware Supported: TS-SOUTH-PAW-REV-1.6 PCB
* Hardware Availability: US

## Features
- The ESP32-C6 is in control of the encoder, wireless/Bluetooth, power management, and battery charging.
- The RP2040 handles the RGB lighting, keyboard matrix, and keymap.

## QMK compilation example
* QMK example for this keyboard (after setting up your build environment):

      qmk compile -kb tssouthpaw/ts-south-paw-rev-1.6 -km default

## Bootloader section
* Must enter Bootloader mode to flash firmware.

## Instructions to enter the bootloader
* Enter the bootloader using one of the following methods:

   **Physical reset button**: Briefly press the button on the back of the PCB.

## Flashing Instructions
### Flashing the RP2040
* QMK example for flashing the RP2040:

      qmk flash -kb tssouthpaw/ts-south-paw-rev-1.6 -km default

### Flashing the ESP32-C6
* Use the ESP-IDF toolchain to flash the ESP32-C6 firmware.
* Example command to flash the ESP32-C6:

      idf.py -p (PORT) flash

Replace `(PORT)` with the appropriate serial port for your ESP32-C6.

## Additional resources and guides
See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

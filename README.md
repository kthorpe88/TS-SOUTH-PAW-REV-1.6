# TS-SOUTH-PAW-REV-1.6

## Description of the keyboard
- Fullsize South Paw Mechanical Keyboard,
- 104 hot swap switches,
- Rotary volume encoder on GP1/GP0  
- MCU: RP2040 (USB‑C)

## Maintainer and hardware information
* Keyboard Maintainer: (TS Design Works LLC)
* Hardware Supported: TS-SOUTH-PAW-REV-1.6 PCB
* Hardware Availability: US

## QMK compilation example
* QMK example for this keyboard (after setting up your build environment):

      qmk compile -kb tssouthpaw -km default

## Bootloader section
* Must enter Bootloader mode to flash firmware.

## Instructions to enter the bootloader
* Enter the bootloader using one of the following methods:

   **Physical reset button**: Briefly press the button on the back of the PCB.

## Flashing Instructions
### Flashing the RP2040
* QMK example for flashing the RP2040:

      qmk flash -kb tssouthpaw -km default

## Additional resources and guides
See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

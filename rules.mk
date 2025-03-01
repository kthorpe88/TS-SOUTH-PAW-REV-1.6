# Microcontroller and clock speed
MCU = STM32F072
F_CPU = 48000000

# Bootloader and platform
BOOTLOADER = stm32-dfu
PLATFORM = chibios

# Manufacturer information
MANUFACTURER = TS Design Works LLC

# RGB configuration
RGB_MATRIX_ENABLE = yes       # Enable RGB Matrix
RGBLIGHT_ENABLE = no          # Disable RGB Light to avoid conflicts
RGB_MATRIX_DRIVER = ws2812    # Specify the driver

# Additional features
DYNAMIC_MACRO_ENABLE = yes
RGB_MATRIX_FRAMEBUFFER_EFFECTS = yes
RGB_MATRIX_KEYPRESSES = yes
ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE = yes

# Source files
SRC += rgb_effects.c

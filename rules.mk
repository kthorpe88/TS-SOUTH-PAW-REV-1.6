# Microcontroller and clock speed
MCU = RP2040
F_CPU = 125000000

# Bootloader and platform
BOOTLOADER = rp2040
PLATFORM = chibios

# Manufacturer information
MANUFACTURER = TS Design Works LLC

# Additional features
DYNAMIC_MACRO_ENABLE = yes

# Enable the use of the onboard ADC
DISABLE_ADC = yes

# Enable the use of the onboard Rotary Encoder
ENCODER_ENABLE = yes

# Enable Vial dynamic keymap support
VIAL_ENABLE = yes

# Include paths for QMK firmware headers
EXTRAFLAGS += -I"$(QUANTUM_DIR)" -I"$(PLATFORM_DIR)" -I"$(CHIBIOS_DIR)/os/license" -I"$(CHIBIOS_DIR)/os/hal/include" -I"$(CHIBIOS_DIR)/os/hal/ports/$(MCU)" -I"$(CHIBIOS_DIR)/os/hal/ports/$(MCU)/LLD" -I"$(CHIBIOS_DIR)/os/hal/lib/streams" -I"$(CHIBIOS_DIR)/os/kernel/include" -I"$(CHIBIOS_DIR)/os/various"

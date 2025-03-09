#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "usb/usb_serial_jtag.h"
#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"
#include "esp_hidd.h"
#include "esp_hid_gap.h"
#include "esp_hid_device.h"

#define UART_NUM UART_NUM_0
#define TXD_PIN GPIO1  // Updated TXD pin for RP2040
#define RXD_PIN GPIO0  // Updated RXD pin for RP2040
#define BAUD_RATE 115200
#define USB_DETECT_PIN GPIO19 // Define the pin to detect USB connection
#define ENC_A_PIN GPIO11 // Encoder A pin
#define ENC_B_PIN GPIO10 // Encoder B pin
#define SLEEP_TIMEOUT_MS 300000 // 5 minutes
#define BATTERY_CHARGING_PIN GPIO35 // Example pin for battery voltage monitoring
#define BATTERY_CHARGING_THRESHOLD 3.7 // Voltage threshold for charging
static uint64_t last_activity = 0; // Timestamp of the last activity
static esp_hidd_dev_t *hid_dev; // Global HID device handle

static const char *TAG = "ESP32-C6";

// Function to configure UART
void configure_uart() {
    // Configure UART parameters
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    // Apply UART configuration
    if (uart_param_config(UART_NUM, &uart_config) != ESP_OK) {
        ESP_LOGE(TAG, "UART configuration failed");
        return;
    }
    // Set UART pins
    if (uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) {
        ESP_LOGE(TAG, "UART pin setup failed");
        return;
    }
    // Install UART driver
    if (uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "UART driver installation failed");
        return;
    }
}

// Bluetooth event handler
void bluetooth_event_handler(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    ESP_LOGI(TAG, "Bluetooth event received: %d", event);
}

// Function to configure Bluetooth
void configure_bluetooth() {
    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    // Initialize Bluetooth controller
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller initialization failed: %d", ret);
        return;
    }
    // Enable Bluetooth
    ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth enable failed: %d", ret);
        return;
    }
    // Initialize Bluedroid stack
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid initialization failed: %d", ret);
        return;
    }
    // Enable Bluedroid stack
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid enable failed: %d", ret);
        return;
    }
    // Set Bluetooth device name
    esp_bt_dev_set_device_name("TS-South-Paw");
    // Register Bluetooth event callback
    esp_bt_gap_register_callback(bluetooth_event_handler);
    ESP_LOGI(TAG, "Bluetooth initialized successfully");
}

// HID event callback
void hid_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param) {
    switch (event) {
        case ESP_HIDD_EVENT_BLE_CONNECT:
            ESP_LOGI(TAG, "HID connected");
            break;
        case ESP_HIDD_EVENT_BLE_DISCONNECT:
            ESP_LOGI(TAG, "HID disconnected");
            break;
        default:
            break;
    }
}

// Function to configure HID
void configure_hid() {
    esp_hidd_callbacks_t callbacks = {
        .event_cb = hid_event_callback,
        .report_cb = NULL,
    };
    // Register HID callbacks
    esp_err_t ret = esp_hid_device_register_callbacks(&callbacks);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID callback registration failed: %d", ret);
        return;
    }
    // Initialize HID device
    ret = esp_hid_device_init(&hid_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID device initialization failed: %d", ret);
        return;
    }
    // Start HID services
    ret = esp_hid_device_start_services(hid_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID service start failed: %d", ret);
        return;
    }
    // Set HID device name
    esp_hid_device_set_device_name("TS-South-Paw");
    ESP_LOGI(TAG, "HID initialized successfully");
}

// Task to handle UART communication and reset activity timer on keypress
void uart_task(void *arg) {
    uint8_t data;
    while (1) {
        // Read data from UART
        int len = uart_read_bytes(UART_NUM, &data, 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            last_activity = esp_timer_get_time() / 1000; // Reset timer on keypress
            ESP_LOGI(TAG, "Received keycode: 0x%02X", data);
            // Send HID report
            uint8_t report[] = {0x00, 0x00, data, 0x00, 0x00, 0x00, 0x00, 0x00};
            esp_hid_device_send_report(hid_dev, ESP_HIDD_REPORT_ID_KEYBOARD, report, sizeof(report));
            report[2] = 0x00; // Release key
            esp_hid_device_send_report(hid_dev, ESP_HIDD_REPORT_ID_KEYBOARD, report, sizeof(report));
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Encoder task
void encoder_task(void *arg) {
    static int8_t last_a_state = 0;
    static int8_t last_b_state = 0;
    gpio_set_direction(ENC_A_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENC_B_PIN, GPIO_MODE_INPUT);
    last_a_state = gpio_get_level(ENC_A_PIN);
    last_b_state = gpio_get_level(ENC_B_PIN);

    while (1) {
        int8_t a_state = gpio_get_level(ENC_A_PIN);
        int8_t b_state = gpio_get_level(ENC_B_PIN);

        if (a_state != last_a_state) { // Detect change on A pin
            if (b_state != a_state) {  // Clockwise
                ESP_LOGI(TAG, "Encoder: Clockwise (Volume Up)");
                uint8_t report[] = {0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00}; // KC_VOLU
                esp_hid_device_send_report(hid_dev, ESP_HIDD_REPORT_ID_KEYBOARD, report, sizeof(report));
                report[2] = 0x00; // Release key
                esp_hid_device_send_report(hid_dev, ESP_HIDD_REPORT_ID_KEYBOARD, report, sizeof(report));
            } else {                   // Counterclockwise
                ESP_LOGI(TAG, "Encoder: Counterclockwise (Volume Down)");
                uint8_t report[] = {0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00}; // KC_VOLD
                esp_hid_device_send_report(hid_dev, ESP_HIDD_REPORT_ID_KEYBOARD, report, sizeof(report));
                report[2] = 0x00; // Release key
                esp_hid_device_send_report(hid_dev, ESP_HIDD_REPORT_ID_KEYBOARD, report, sizeof(report));
            }
            last_activity = esp_timer_get_time() / 1000; // Reset sleep timer
        }
        last_a_state = a_state;
        last_b_state = b_state;
        vTaskDelay(pdMS_TO_TICKS(10)); // Debounce delay
    }
}

// Task to detect USB connection and manage Bluetooth state
void usb_detect_task(void *arg) {
    gpio_set_direction(USB_DETECT_PIN, GPIO_MODE_INPUT);
    int previous_state = gpio_get_level(USB_DETECT_PIN);
    int stable_count = 0;

    while (1) {
        int current_state = gpio_get_level(USB_DETECT_PIN);
        if (current_state == previous_state) {
            stable_count++;
        } else {
            stable_count = 0;
        }

        if (stable_count >= 5) { // Stable for 5 checks (~2.5 seconds debounce)
            if (current_state) {
                ESP_LOGI(TAG, "USB connected, disabling Bluetooth.");
                if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_ENABLED) {
                    esp_bluedroid_disable();
                }
                if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED) {
                    esp_bt_controller_disable();
                }
                uart_write_bytes(UART_NUM, "W", 1); // Send wired mode status to RP2040
            } else {
                ESP_LOGI(TAG, "USB disconnected, enabling Bluetooth.");
                if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_IDLE) {
                    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
                }
                if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_UNINITIALIZED) {
                    esp_bluedroid_enable();
                }
                uart_write_bytes(UART_NUM, "B", 1); // Send wireless mode status to RP2040
            }
            stable_count = 0;
        }
        previous_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task to put the device into deep sleep if no activity is detected
void bluetooth_sleep_task(void *arg) {
    uint8_t buffer[5];
    while (1) {
        if (esp_timer_get_time() / 1000 - last_activity > SLEEP_TIMEOUT_MS && !gpio_get_level(USB_DETECT_PIN)) {
            ESP_LOGI(TAG, "Entering deep sleep due to timeout");
            goto sleep;
        }
        int len = uart_read_bytes(UART_NUM, buffer, 5, pdMS_TO_TICKS(100));
        if (len == 5 && memcmp(buffer, "SLEEP", 5) == 0) {
            ESP_LOGI(TAG, "Entering deep sleep due to RP2040 signal");
            goto sleep;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        continue;

    sleep:
        esp_bluedroid_disable();
        esp_bt_controller_disable();
        esp_deep_sleep_start();
    }
}

// Function to handle UART communication with RP2040
void handle_uart_communication() {
    uint8_t data;
    int len = uart_read_bytes(UART_NUM, &data, 1, pdMS_TO_TICKS(100));
    if (len > 0) {
        // Process received data from RP2040
        switch (data) {
            case 'S': // Sleep command
                ESP_LOGI(TAG, "Received sleep command from RP2040");
                esp_deep_sleep_start();
                break;
            case 'W': // Wake command
                ESP_LOGI(TAG, "Received wake command from RP2040");
                // Handle wake-up logic if needed
                break;
            default:
                ESP_LOGI(TAG, "Received unknown command: 0x%02X", data);
                break;
        }
    }
}

// Function to handle power management
void handle_power_management() {
    // Check battery level and manage power accordingly
    uint8_t battery_level = read_battery_level();
    if (battery_level < BATTERY_CHARGING_THRESHOLD) {
        ESP_LOGI(TAG, "Battery level low, enabling charging mode");
        // Enable charging mode
        gpio_set_level(BATTERY_CHARGING_PIN, 1);
    } else {
        ESP_LOGI(TAG, "Battery level sufficient, disabling charging mode");
        // Disable charging mode
        gpio_set_level(BATTERY_CHARGING_PIN, 0);
    }
}

uint8_t read_battery_level(void) {
    uint32_t adc_reading = adc1_get_raw(ADC1_CHANNEL_0); // Adjust if necessary
    uint8_t battery_percentage = (adc_reading * 100) / 4095; // Adjust if necessary
    return battery_percentage;
}

// Main application entry point
void app_main() {
    ESP_LOGI(TAG, "Starting ESP32-C6 Firmware");

    configure_uart(); // Initialize UART
    configure_bluetooth(); // Initialize Bluetooth
    configure_hid(); // Initialize HID
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL); // Create UART task
    xTaskCreate(encoder_task, "encoder_task", 2048, NULL, 10, NULL); // Create encoder task
    xTaskCreate(usb_detect_task, "usb_detect_task", 2048, NULL, 10, NULL); // Create USB detection task
    xTaskCreate(bluetooth_sleep_task, "bluetooth_sleep_task", 2048, NULL, 5, NULL); // Create sleep task
    esp_sleep_enable_uart_wakeup(UART_NUM); // Enable wakeup on UART activity
    esp_sleep_enable_ext0_wakeup(USB_DETECT_PIN, 1); // Wake on USB connect

    while (1) {
        ESP_LOGI(TAG, "ESP32-C6 Running...");
        handle_power_management(); // Handle power management
        handle_uart_communication(); // Handle UART communication with RP2040
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

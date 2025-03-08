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
#define ENC_A_PIN GPIO11 // Updated encoder A pin
#define ENC_B_PIN GPIO10 // Updated encoder B pin

static const char *TAG = "ESP32-C6";

// Function to configure UART
void configure_uart() {
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    if (uart_param_config(UART_NUM, &uart_config) != ESP_OK) {
        ESP_LOGE(TAG, "UART configuration failed");
        return;
    }
    if (uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) {
        ESP_LOGE(TAG, "UART pin setup failed");
        return;
    }
    if (uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "UART driver installation failed");
        return;
    }
}

// Bluetooth event handler
void bluetooth_event_handler(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    ESP_LOGI(TAG, "Bluetooth event received: %d", event);
}

// Function to configure Bluetooth
void configure_bluetooth()
{
    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller initialization failed: %d", ret);
        return;
    }
    ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth enable failed: %d", ret);
        return;
    }
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid initialization failed: %d", ret);
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid enable failed: %d", ret);
        return;
    }
    esp_bt_dev_set_device_name("ESP32-C6 Keyboard");
    esp_bt_gap_register_callback(bluetooth_event_handler);
    ESP_LOGI(TAG, "Bluetooth initialized successfully");
}

// HID event callback
void hid_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param)
{
    switch (event)
    {
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
void configure_hid()
{
    esp_hidd_dev_t *hid_dev;
    esp_hidd_callbacks_t callbacks = {
        .event_cb = hid_event_callback,
        .report_cb = NULL,
    };

    esp_err_t ret = esp_hid_device_register_callbacks(&callbacks);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID callback registration failed: %d", ret);
        return;
    }

    ret = esp_hid_device_init(&hid_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID device initialization failed: %d", ret);
        return;
    }

    ret = esp_hid_device_start_services(hid_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID service start failed: %d", ret);
        return;
    }

    esp_hid_device_set_device_name("ESP32-C6 Keyboard");
    ESP_LOGI(TAG, "HID initialized successfully");
}

// Task to handle UART communication
void uart_task(void *arg)
{
    uint8_t data[128];
    while (1)
    {
        int len = uart_read_bytes(UART_NUM, data, sizeof(data) - 1, pdMS_TO_TICKS(100));
        if (len > 0)
        {
            data[len] = '\0'; // Null-terminate received data
            ESP_LOGI(TAG, "Received from RP2040: %s", data);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Function to send keypress to Bluetooth
void send_keypress_to_bt(uint8_t keycode)
{
    char message[16];
    snprintf(message, sizeof(message), "KEY:%02X", keycode);
    uart_write_bytes(UART_NUM, message, strlen(message));
    ESP_LOGI(TAG, "Sent keypress: %s", message);
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

        if (stable_count >= 5) { // stable for 5 checks (~2.5 seconds debounce)
            if (current_state) {
                ESP_LOGI(TAG, "USB connected, disabling Bluetooth.");
                if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_ENABLED) {
                    esp_bluedroid_disable();
                }
                if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED) {
                    esp_bt_controller_disable();
                }
            } else {
                ESP_LOGI(TAG, "USB disconnected, enabling Bluetooth.");
                if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_IDLE) {
                    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
                }
                if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_UNINITIALIZED) {
                    esp_bluedroid_enable();
                }
            }
            stable_count = 0;
        }
        previous_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Main application entry point
void app_main()
{
    ESP_LOGI(TAG, "Starting ESP32-C6 Firmware");

    configure_uart();
    configure_bluetooth();
    configure_hid();
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);
    xTaskCreate(usb_detect_task, "usb_detect_task", 2048, NULL, 10, NULL);

    // Configure encoder pins as input
    gpio_set_direction(ENC_A_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENC_B_PIN, GPIO_MODE_INPUT);

    while (1)
    {
        ESP_LOGI(TAG, "ESP32-C6 Running...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

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

// Define UART parameters
#define UART_NUM UART_NUM_0
#define TXD_PIN GPIO17
#define RXD_PIN GPIO16
#define BAUD_RATE 115200

static const char *TAG = "ESP32-C6";

// Function to configure UART settings
void configure_uart()
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    
    // Configure UART parameters
    if (uart_param_config(UART_NUM, &uart_config) != ESP_OK) {
        ESP_LOGE(TAG, "UART parameter configuration failed");
        return;
    }
    
    // Set UART pins
    if (uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) {
        ESP_LOGE(TAG, "UART set pin failed");
        return;
    }
    
    // Install UART driver
    if (uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "UART driver install failed");
        return;
    }
}

// Bluetooth event handler function
void bluetooth_event_handler(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    ESP_LOGI(TAG, "Bluetooth event received: %d", event);
}

// Function to configure Bluetooth settings
void configure_bluetooth()
{
    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    
    // Initialize Bluetooth controller
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller initialization failed: %d", ret);
        return;
    }
    
    // Enable Bluetooth controller
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
    esp_bt_dev_set_device_name("ESP32-C6 Keyboard");
    
    // Register Bluetooth GAP callback
    esp_bt_gap_register_callback(bluetooth_event_handler);
    ESP_LOGI(TAG, "Bluetooth initialized successfully");
}

// Main application entry point
void app_main()
{
    ESP_LOGI(TAG, "Starting ESP32-C6 Firmware");

    // Configure UART
    configure_uart();
    
    // Configure Bluetooth
    configure_bluetooth();

    // Main loop
    while (1)
    {
        // Log running status every second
        ESP_LOGI(TAG, "ESP32-C6 Running...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

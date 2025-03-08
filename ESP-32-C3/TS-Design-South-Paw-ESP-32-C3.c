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
#define TXD_PIN GPIO17
#define RXD_PIN GPIO16
#define BAUD_RATE 115200
#define USB_DETECT_PIN GPIO19 // Define the pin to detect USB connection

static const char *TAG = "ESP32-C6";

void configure_uart()
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    if (uart_param_config(UART_NUM, &uart_config) != ESP_OK) {
        ESP_LOGE(TAG, "UART parameter configuration failed");
        return;
    }
    if (uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) {
        ESP_LOGE(TAG, "UART set pin failed");
        return;
    }
    if (uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "UART driver install failed");
        return;
    }
}

void bluetooth_event_handler(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    ESP_LOGI(TAG, "Bluetooth event received: %d", event);
}

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

void configure_hid()
{
    esp_hidd_dev_t *hid_dev;
    esp_hidd_callbacks_t callbacks = {
        .event_cb = hid_event_callback,
        .report_cb = NULL,
    };

    esp_hid_device_register_callbacks(&callbacks);
    esp_hid_device_init(&hid_dev);
    esp_hid_device_start_services(hid_dev);
    esp_hid_device_set_device_name("ESP32-C6 Keyboard");
    ESP_LOGI(TAG, "HID initialized successfully");
}

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

void send_keypress_to_bt(uint8_t keycode)
{
    char message[16];
    snprintf(message, sizeof(message), "KEY:%02X", keycode);
    uart_write_bytes(UART_NUM, message, strlen(message));
    ESP_LOGI(TAG, "Sent keypress: %s", message);
}

void usb_detect_task(void *arg)
{
    gpio_set_direction(USB_DETECT_PIN, GPIO_MODE_INPUT);
    while (1)
    {
        int usb_connected = gpio_get_level(USB_DETECT_PIN);
        if (usb_connected)
        {
            ESP_LOGI(TAG, "USB connected, switching to wired mode");
            // Disable Bluetooth and HID
            esp_bluedroid_disable();
            esp_bt_controller_disable();
        }
        else
        {
            ESP_LOGI(TAG, "USB disconnected, switching to wireless mode");
            // Enable Bluetooth and HID
            esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
            esp_bluedroid_enable();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main()
{
    ESP_LOGI(TAG, "Starting ESP32-C6 Firmware");

    configure_uart();
    configure_bluetooth();
    configure_hid();
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);
    xTaskCreate(usb_detect_task, "usb_detect_task", 2048, NULL, 10, NULL);

    while (1)
    {
        ESP_LOGI(TAG, "ESP32-C6 Running...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

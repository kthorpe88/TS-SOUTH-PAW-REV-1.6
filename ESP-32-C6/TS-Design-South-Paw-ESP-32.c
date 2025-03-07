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

#define UART_NUM UART_NUM_0
#define TXD_PIN GPIO17
#define RXD_PIN GPIO16
#define BAUD_RATE 115200

static const char *TAG = "ESP32-C6";

void configure_uart()
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
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

void app_main()
{
    ESP_LOGI(TAG, "Starting ESP32-C6 Firmware");

    configure_uart();
    configure_bluetooth();

    while (1)
    {
        ESP_LOGI(TAG, "ESP32-C6 Running...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_hidd_prf_api.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_log.h"

#define HID_DEMO_TAG "HID_MOUSE"

// HID Device Name
#define HIDD_DEVICE_NAME "ESP32 Mouse"

// Connection and security status
static uint16_t hid_conn_id = 0;
static bool sec_conn = false;

// HID UUID for the mouse service
static uint8_t hidd_service_uuid128[] = {
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0x18, 0x00, 0x00,
};

// BLE Advertising Data
static esp_ble_adv_data_t hidd_adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, // Minimum interval (7.5 ms)
    .max_interval = 0x0010, // Maximum interval (20 ms)
    .appearance = 0x03c2,   // Appearance for a generic mouse
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(hidd_service_uuid128),
    .p_service_uuid = hidd_service_uuid128,
    .flag = ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT,
};

// BLE Advertising Parameters
static esp_ble_adv_params_t hidd_adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x30,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

// HID Callback for events like connection and disconnection
static void hidd_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param) {
    switch (event) {
        case ESP_HIDD_EVENT_REG_FINISH:
            if (param->init_finish.state == ESP_HIDD_INIT_OK) {
                esp_ble_gap_set_device_name(HIDD_DEVICE_NAME);
                esp_ble_gap_config_adv_data(&hidd_adv_data);
            }
            break;
        case ESP_HIDD_EVENT_BLE_CONNECT:
            ESP_LOGI(HID_DEMO_TAG, "Connected to client");
            hid_conn_id = param->connect.conn_id;
            sec_conn = true;
            break;
        case ESP_HIDD_EVENT_BLE_DISCONNECT:
            ESP_LOGI(HID_DEMO_TAG, "Disconnected from client");
            sec_conn = false;
            esp_ble_gap_start_advertising(&hidd_adv_params);
            break;
        default:
            break;
    }
}

// GAP Event Handler
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    if (event == ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT) {
        esp_ble_gap_start_advertising(&hidd_adv_params);
    }
}

// Simulate Mouse Left-Right Movement
void hid_demo_task(void *pvParameters) {
    int x_movement = 100;  // Define X-axis movement increment
    while (1) {
        if (sec_conn) {
            // Move mouse right
            ESP_LOGI(HID_DEMO_TAG, "Moving mouse right");
            esp_hidd_send_mouse_value(hid_conn_id, 0, x_movement, 0);
            vTaskDelay(pdMS_TO_TICKS(500));  // Wait 500 ms

            // Move mouse left
            ESP_LOGI(HID_DEMO_TAG, "Moving mouse left");
            esp_hidd_send_mouse_value(hid_conn_id, 0, -x_movement, 0);
            vTaskDelay(pdMS_TO_TICKS(5000));  // Pause for 5 seconds
        } else {
            vTaskDelay(pdMS_TO_TICKS(1000));  // Check connection every 1 second if disconnected
        }
    }
}

void app_main(void) {
    // Initialize NVS and Bluetooth
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Release Classic Bluetooth to save memory
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    // Initialize the Bluetooth controller and enable BLE
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    // Initialize the HID profile and set up callbacks
    ESP_ERROR_CHECK(esp_hidd_profile_init());
    esp_ble_gap_register_callback(gap_event_handler);
    esp_hidd_register_callbacks(hidd_event_callback);

    // Security and bonding configuration
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND;      // Bonding
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;            // No input/output capability
    uint8_t key_size = 16;                               // Key size
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

    // Start the HID demo task
    xTaskCreate(&hid_demo_task, "hid_task", 2048, NULL, 5, NULL);
}

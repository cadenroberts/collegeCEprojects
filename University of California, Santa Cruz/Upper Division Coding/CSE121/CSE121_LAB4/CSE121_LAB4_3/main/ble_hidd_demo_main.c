#include "driver/i2c.h"
#include "esp_log.h"
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
#include "driver/gpio.h"
#include "hid_dev.h"
#define HID_DEMO_TAG "HID_MOUSE"

// HID Device Name
#define HIDD_DEVICE_NAME "ESP32 Mouse"

#define TAG "ICM42670"
#define I2C_MASTER_SCL_IO 8          // Set the GPIO number for SCL
#define I2C_MASTER_SDA_IO 10         // Set the GPIO number for SDA
#define I2C_MASTER_NUM I2C_NUM_0     // I2C port number for master
#define I2C_MASTER_FREQ_HZ 400000    // I2C master clock frequency
#define ICM42670_ADDR 0x68           // I2C address of the ICM-42670-P

#define WHO_AM_I_REG 0x75            // WHO_AM_I register for device ID check
#define ACCEL_CONFIG 0x33            // Register to configure accelerometer
#define PWR_MGMT_1 0x1F              // Power management register
#define ACCEL_XOUT_H 0x0B            // High byte of X-axis acceleration data
#define ACCEL_YOUT_H 0x0D            // High byte of Y-axis acceleration data


// Define GPIO for the boot button
#define BOOT_BUTTON_GPIO GPIO_NUM_9


void setup_button() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,     // Disable interrupts
        .mode = GPIO_MODE_INPUT,            // Set as input mode
        .pin_bit_mask = (1ULL << BOOT_BUTTON_GPIO), // Select GPIO0
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,   // Enable pull-up resistor
    };
    gpio_config(&io_conf);
}
// Initialize the I2C driver
esp_err_t i2c_master_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = I2C_MASTER_FREQ_HZ,
        },
        .clk_flags = 0,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

// Write a byte to a register
esp_err_t i2c_write_register(uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ICM42670_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Read two bytes (high and low) from a register
int16_t i2c_read_register_16(uint8_t reg) {
    uint8_t data[2];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ICM42670_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);  // Restart for reading
    i2c_master_write_byte(cmd, (ICM42670_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 2, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read register 0x%02X", reg);
        return 0;
    }

    return (int16_t)((data[0] << 8) | data[1]);  // Combine high and low bytes
}

// Initialize ICM-42670-P for accelerometer readings
void init_icm42670() {
    ESP_ERROR_CHECK(i2c_write_register(PWR_MGMT_1, 0x0F)); // Power up accelerometer and gyroscope
    ESP_ERROR_CHECK(i2c_write_register(ACCEL_CONFIG, 0x10)); // Set accelerometer configuration (e.g., ±4g)
}

// Read X and Y accelerometer values
void read_accel_data(int16_t *x, int16_t *y) {
    *x = i2c_read_register_16(ACCEL_XOUT_H);     // Read X-axis
    *y = i2c_read_register_16(ACCEL_YOUT_H);     // Read Y-axis
}

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

//


//


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

    ESP_ERROR_CHECK(i2c_master_init());
    setup_button();
    init_icm42670();
    int y_movement, x_movement;
    bool left_click_state = false;  // Track left-click state (Reset button)
    uint8_t yes = 0, i = 0;
    while (true) {
        int16_t x, y;
        read_accel_data(&x, &y);

        // ESP_LOGI(TAG, "X: %d, Y: %d %s %s", x, y, x>0 ? "LEFT" : "RIGHT", y>0 ? "UP" : "DOWN");  // Print raw accelerometer data
          // Define X-axis movement increment
        int negy = 0, negx = 0;
        if (y < 0) {
            negy = 1;
            y = -y;
        }
        if (x < 0){
            negx = 1;
            x = -x;
        }
        y_movement = (y > 600 ? 15 : (y > 400 ? 10 : ( y > 200 ? 5 : (y > 100 ? 1 : 0))));
        x_movement = (x > 600 ? 15 : (x > 400 ? 10 : ( x > 200 ? 5 : (x > 100 ? 1 : 0))));
        if (negy) {
            y_movement = -y_movement;
        }
        if (negx) {
            x_movement = -x_movement;
        }
        if (y_movement==0&&x_movement==0) yes++;
        if (yes == 30) {
            printf("%d\n", i);
            yes=0;
            esp_hidd_send_mouse_value(hid_conn_id, yes,0,0);
            vTaskDelay(200 / portTICK_PERIOD_MS);
            esp_hidd_send_mouse_value(hid_conn_id, 0,0,0);
        }

        esp_hidd_send_mouse_value(hid_conn_id, 0, x_movement, y_movement);


        if (gpio_get_level(BOOT_BUTTON_GPIO) == 0 && !left_click_state) {
            esp_hidd_send_mouse_value(hid_conn_id, 1, 0, 0);
            vTaskDelay(200 / portTICK_PERIOD_MS);
            esp_hidd_send_mouse_value(hid_conn_id, 0, 0, 0); // Release left-click
            left_click_state = true;
            printf("Yes\n");
        } else if (gpio_get_level(BOOT_BUTTON_GPIO) == 1) {
            left_click_state = false; // Reset click state when button is released
        } 

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }


}
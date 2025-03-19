#include "driver/i2c.h"
#include "esp_log.h"

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

extern "C" void app_main() {
    ESP_ERROR_CHECK(i2c_master_init());

    init_icm42670();

    while (true) {
        int16_t x, y;
        read_accel_data(&x, &y);

        ESP_LOGI(TAG, "X: %d, Y: %d %s %s", x, y, x>0 ? "RIGHT" : "LEFT", y>0 ? "DOWN" : "UP");  // Print raw accelerometer data
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/task.h"

// I2C configuration
#define I2C_MASTER_SCL_IO 8          // GPIO for SCL
#define I2C_MASTER_SDA_IO 10         // GPIO for SDA
#define I2C_MASTER_NUM I2C_NUM_0     // I2C port number for master
#define I2C_MASTER_FREQ_HZ 100000    // I2C clock frequency
#define SHTC3_SENSOR_ADDR 0x70       // SHTC3 I2C address

// SHTC3 command codes
#define SHTC3_CMD_WAKEUP 0x3517
#define SHTC3_CMD_SLEEP 0xB098
#define SHTC3_CMD_MEASURE 0x7CA2     // Measure temp and humidity with clock stretching

// Initialize I2C master
static void i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

// Send command to SHTC3 sensor
static esp_err_t shtc3_write_cmd(uint16_t cmd) {
    uint8_t cmd_buf[2] = { (cmd >> 8) & 0xFF, cmd & 0xFF };
    return i2c_master_write_to_device(I2C_MASTER_NUM, SHTC3_SENSOR_ADDR, cmd_buf, sizeof(cmd_buf), pdMS_TO_TICKS(1000));
}

// Read temperature and humidity data (2 bytes each)
static esp_err_t shtc3_read_data(uint8_t *data, size_t len) {
    return i2c_master_read_from_device(I2C_MASTER_NUM, SHTC3_SENSOR_ADDR, data, len, pdMS_TO_TICKS(1000));
}

// Convert raw sensor data to temperature (Celsius)
static float get_temperature(uint16_t raw_temp) {
    return -45 + 175 * (raw_temp / 65535.0);
}

// Convert raw sensor data to humidity (percentage)
static float get_humidity(uint16_t raw_hum) {
    return 100 * (raw_hum / 65535.0);
}

// Read and print temperature and humidity
static void read_and_print_data(void) {
    uint8_t data[6];

    // Power up the sensor
    shtc3_write_cmd(SHTC3_CMD_WAKEUP);
    vTaskDelay(pdMS_TO_TICKS(10));

    // Start measurement
    shtc3_write_cmd(SHTC3_CMD_MEASURE);
    vTaskDelay(pdMS_TO_TICKS(15));

    // Read 6 bytes: temperature (2 bytes), humidity (2 bytes), and 2 CRC bytes
    shtc3_read_data(data, 6);

    // Convert raw data to temperature and humidity
    uint16_t raw_temp = (data[0] << 8) | data[1];
    uint16_t raw_hum = (data[3] << 8) | data[4];
    float temperature = get_temperature(raw_temp);
    float humidity = get_humidity(raw_hum);

    // Print the results
    printf("Temperature is %.1f°C (or %.1f°F) with a %.1f%% Humidity\n", temperature, temperature * 1.8 + 32, humidity);

    // Power down the sensor
    shtc3_write_cmd(SHTC3_CMD_SLEEP);
}

void app_main(void) {
    i2c_master_init();
    
    while (1) {
        read_and_print_data();
        vTaskDelay(pdMS_TO_TICKS(2000));  // Wait for 2 seconds
    }
}

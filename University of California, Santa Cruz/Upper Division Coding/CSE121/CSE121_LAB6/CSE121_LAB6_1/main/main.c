#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/task.h"
//
#include "driver/gpio.h"
#include "esp_timer.h"
//
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


//
#define TRIG_PIN 5
#define ECHO_PIN 6
//
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

//

// Measure distance with SR04
static float measure_distance(float temperature) {
    const int timeout_us = 25000; // 25ms timeout for ECHO response
    int64_t start_time, end_time;

    // Generate TRIG pulse
    gpio_set_level(TRIG_PIN, 0);
    esp_rom_delay_us(2);
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_PIN, 0);

    // Wait for ECHO pin to go HIGH (start of pulse)
    start_time = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 0) {
        if (esp_timer_get_time() - start_time > timeout_us) {
            printf("ECHO timeout waiting for pulse start.\n");
            return -1; // Indicate timeout
        }
    }

    // Measure duration of HIGH pulse on ECHO pin
    start_time = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 1) {
        if (esp_timer_get_time() - start_time > timeout_us) {
            printf("ECHO timeout waiting for pulse end.\n");
            return -1; // Indicate timeout
        }
    }
    end_time = esp_timer_get_time();

    // Calculate pulse duration in microseconds
    int64_t pulse_duration = end_time - start_time;

    // Calculate speed of sound based on temperature (in cm/us)
    float speed_of_sound = 331.3 + (0.6 * temperature); // Speed in m/s
    float speed_of_sound_cm_us = speed_of_sound / 10000; // Convert to cm/μs

    // Calculate distance (round-trip time, so divide by 2)
    float distance = (pulse_duration * speed_of_sound_cm_us) / 2;
    return distance;
}
static void read_and_print_data(void) {
    uint8_t data[6];
    shtc3_write_cmd(SHTC3_CMD_WAKEUP);
    vTaskDelay(pdMS_TO_TICKS(10));
    shtc3_write_cmd(SHTC3_CMD_MEASURE);
    vTaskDelay(pdMS_TO_TICKS(15));
    shtc3_read_data(data, 6);
    uint16_t raw_temp = (data[0] << 8) | data[1];
    float temperature = get_temperature(raw_temp);
    shtc3_write_cmd(SHTC3_CMD_SLEEP);

    float distance = measure_distance(temperature);

    printf("Distance: %.2f cm at %.1f°C\n", distance, temperature);
}
//
// Read and print temperature and humidity


void app_main(void) {
    // Initialize I2C for temperature sensor
    i2c_master_init();

    // Initialize GPIO for SR04
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << TRIG_PIN) | (1ULL << ECHO_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
    };
    gpio_config(&io_conf);

    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);

    // Main loop
    while (1) {
        read_and_print_data();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
    }
}


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "esp_sntp.h"

// THIS CODE SENDS TO POSTGRES BACKEND

// WIFI CREDENTIALS ARE CONSTANT MOBILE HOTSPOT
#define WIFI_SSID "Jho"
#define WIFI_PASS "12345678"

// Only need the client UUID now:
#define CLIENT_ID "27fbb904-6ca9-4d8a-a894-38a393eb28b2"

// IMPORTANT: point at your Node.js backend on port 3010
#define SERVER_URL "http://35.215.126.127:3010"

#define FLEX_CHANNEL ADC_CHANNEL_0
#define FLEX_WIDTH ADC_WIDTH_BIT_12
#define SAMPLE_COUNT 10
#define SAMPLE_INTERVAL_MS 50
#define MAX_DURATION_MS 10000
#define MAX_SAMPLES (MAX_DURATION_MS / SAMPLE_INTERVAL_MS)

static float voltage_array[MAX_SAMPLES];
static float flex_array[MAX_SAMPLES];
static int sample_idx = 0;

void initialize_sntp(void) {
    ESP_LOGI("SNTP", "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

bool obtain_time(void) {
    initialize_sntp();

    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0, retry_count = 10;

    while (timeinfo.tm_year < (2023 - 1900) && ++retry < retry_count) {
        ESP_LOGI("SNTP", "Waiting for system time... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2023 - 1900)) {
        ESP_LOGE("SNTP", "Failed to get time over NTP.");
        return false;
    }

    ESP_LOGI("SNTP", "Time obtained successfully.");
    return true;
}

static bool wifi_init_sta(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI("WIFI", "Connecting to %s...", WIFI_SSID);
    esp_wifi_connect();

    for (int i = 0; i < 10; ++i) {
        wifi_ap_record_t ap_info;
        vTaskDelay(pdMS_TO_TICKS(500));
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            ESP_LOGI("WIFI", "Connected to AP: %s", ap_info.ssid);
            return true;
        }
    }

    ESP_LOGE("WIFI", "Failed to connect to Wi-Fi after retries");
    return false;
}

static void send_data_to_server(void) {
    time_t now;
    time(&now);
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%" PRId64, (int64_t)now);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "voltages", cJSON_CreateFloatArray(voltage_array, sample_idx));
    cJSON_AddItemToObject(root, "flex_percent", cJSON_CreateFloatArray(flex_array, sample_idx));

    char url[256];
    snprintf(url, sizeof(url), SERVER_URL "/upload/%s/%s", CLIENT_ID, timestamp);
    char *post_data = cJSON_Print(root);

    ESP_LOGI("HTTP", "Posting to: %s\n%s", url, post_data);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    // Removed X-API-KEY header entirely
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI("HTTP", "Status = %d, content_length = %" PRId64,
                 esp_http_client_get_status_code(client),
                 (int64_t)esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE("HTTP", "HTTP POST failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    cJSON_Delete(root);
    free(post_data);
}

void app_main(void) {
    nvs_flash_init();

    if (!wifi_init_sta()) {
        ESP_LOGE("MAIN", "Wi-Fi connection failed. Halting program.");
        return;
    }

    if (!obtain_time()) {
        ESP_LOGE("MAIN", "Failed to obtain time. Halting program.");
        return;
    }

    adc1_config_width(FLEX_WIDTH);
    adc1_config_channel_atten(FLEX_CHANNEL, ADC_ATTEN_DB_11);

    float readings[SAMPLE_COUNT] = {0};
    int sample_index = 0;
    int elapsed_ms = 0;

    while (elapsed_ms < MAX_DURATION_MS && sample_idx < MAX_SAMPLES) {
        int raw = adc1_get_raw(FLEX_CHANNEL);
        float voltage = (raw / 4095.0f) * 3.3f;
        readings[sample_index++] = voltage;

        if (sample_index >= SAMPLE_COUNT) {
            float sum = 0.0f;
            for (int i = 0; i < SAMPLE_COUNT; i++) sum += readings[i];
            float mean = sum / SAMPLE_COUNT;

            float variance = 0.0f;
            for (int i = 0; i < SAMPLE_COUNT; i++) variance += powf(readings[i] - mean, 2);
            float std_dev = sqrtf(variance / SAMPLE_COUNT);

            float filtered_sum = 0.0f;
            int count = 0;
            for (int i = 0; i < SAMPLE_COUNT; i++) {
                if (fabsf(readings[i] - mean) <= std_dev) {
                    filtered_sum += readings[i];
                    count++;
                }
            }

            float avg_voltage = (count > 0) ? filtered_sum / count : mean;
            float flex_percent = 0.0f;
            const float min_voltage = 0.25f, max_voltage = 1.2f;
            if (avg_voltage >= min_voltage) {
                flex_percent = ((avg_voltage - min_voltage) / (max_voltage - min_voltage)) * 100.0f;
                if (flex_percent > 100.0f) flex_percent = 100.0f;
            }

            voltage_array[sample_idx] = avg_voltage;
            flex_array[sample_idx] = flex_percent;
            sample_idx++;

            ESP_LOGI("FLEX", "Avg Voltage: %.3f V | Flex: %.1f%%", avg_voltage, flex_percent);
            sample_index = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL_MS));
        elapsed_ms += SAMPLE_INTERVAL_MS;
    }

    ESP_LOGI("FLEX", "Finished collecting. Uploading...");
    send_data_to_server();
}

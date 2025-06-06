#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>




#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"




#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "cJSON.h"




// === CONFIGURATIONS ===
#define WIFI_SSID             "Jho"
#define WIFI_PASS             "12345678"
#define RUNNER_URL            "https://therahand.info/v0/runner"
#define MAX_HTTP_OUTPUT_BUFFER 8192




// (certificate truncated for brevity)
static const char isrg_root_x1_pem[] =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";








static const char *TAG = "runner_client";
static SemaphoreHandle_t s_semph_wifi_connected;
static char http_buffer[MAX_HTTP_OUTPUT_BUFFER];
static int  http_buf_len;








// Valid HW IDs
static const char *valid_hw_ids[] = {"aliyaa_device", "blank_device"};
static const int valid_hw_count = sizeof(valid_hw_ids)/sizeof(valid_hw_ids[0]);








// === FLEX & SERVO CONFIGURATION ===
#define SENSOR_COUNT          5
#define CALIB_SAMPLES         10
#define SAMPLE_COUNT          10
#define SAMPLE_INTERVAL_MS    50
#define MIN_VOLTAGE           0.25f
#define MAX_VOLTAGE           1.2f








// how many 0.5 s samples we can store (adjust if you expect >200 samples/session)
#define MAX_RESULT_SAMPLES 200








// buffers for the recorded flex% (one entry per 0.5 s)
static float result_thumb[MAX_RESULT_SAMPLES];
static float result_index_vals[MAX_RESULT_SAMPLES];
static float result_middle[MAX_RESULT_SAMPLES];
static float result_ring[MAX_RESULT_SAMPLES];
static float result_pinky[MAX_RESULT_SAMPLES];
static float result_timestamps[MAX_RESULT_SAMPLES];
static int   result_sample_count = 0;
static bool  recording_results    = false;








// Gamma for sensitivity curve (20% -> ~60%)
#define FLEX_SENSITIVITY_GAMMA 0.317f








static const adc1_channel_t flex_channels[SENSOR_COUNT] = {
    ADC1_CHANNEL_0, ADC1_CHANNEL_1, ADC1_CHANNEL_2,
    ADC1_CHANNEL_3, ADC1_CHANNEL_4
};
float baseline[SENSOR_COUNT];
static float readings[SENSOR_COUNT][SAMPLE_COUNT];
static int sample_index = 0;








#define SERVO1_GPIO           5
#define SERVO2_GPIO           7
#define SERVO_PWM_FREQ        50
#define SERVO_PWM_RES         LEDC_TIMER_13_BIT
#define SERVO1_CHANNEL        LEDC_CHANNEL_0
#define SERVO2_CHANNEL        LEDC_CHANNEL_1
#define SERVO_STOP_MS         1.5f
#define SERVO_MIN_MS          1.45f
#define SERVO_MAX_MS          1.65f
#define BOOT_BTN_GPIO         9
#define PAUSE_BETWEEN_SETS_MS 2000








// dynamic timing globals
static float one_rot_ms, two_rot_ms, one_rev_ms, two_rev_ms;
// calibration anchors
static const float A30_1    = 6000.0f, A30_2    = 2200.0f;
static const float A30_1rev = 4500.0f, A30_2rev = 1200.0f;
static const float A60_1    = 9500.0f, A60_2    = 6000.0f;
static const float A60_1rev = 6900.0f, A60_2rev = 3900.0f;








// === FORWARD DECLARATIONS ===
static void wifi_init(void);
static void wifi_event_cb(void* arg, esp_event_base_t base, int32_t id, void* data);
static esp_err_t _http_event_handler(esp_http_client_event_t *evt);
static void http_runner_task(void *pv);
static void sample_and_log_flex(void);
void set_durations_for_value(float v);
uint32_t ms_to_duty(float ms);
void setup_servo(int gpio, ledc_channel_t ch);
void set_servo_pulse(ledc_channel_t ch, float pulse_ms);
void stop_all_servos(void);
bool boot_button_pressed(void);
void run_servo_with_interrupt(ledc_channel_t ch, float pulse_ms, int dur_ms);
void run_servo_sequence(int reps);








// === WIFI HANDLER ===
static void wifi_event_cb(void* arg, esp_event_base_t base,
                          int32_t id, void* data) {
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        xSemaphoreGive(s_semph_wifi_connected);
    }
}








static void wifi_init(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();








    esp_event_handler_instance_t h1, h2;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                       &wifi_event_cb, NULL, &h1);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                       &wifi_event_cb, NULL, &h2);








    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    wifi_config_t sta = { .sta = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
    }};
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &sta);








    s_semph_wifi_connected = xSemaphoreCreateBinary();
    esp_wifi_start();
    ESP_LOGI(TAG, "Wi-Fi started, waiting for IP...");
    if (xSemaphoreTake(s_semph_wifi_connected,
                       pdMS_TO_TICKS(10000)) == pdTRUE) {
        ESP_LOGI(TAG, "Got IP, starting HTTP task");
    } else {
        ESP_LOGE(TAG, "Failed to get IP");
    }
}








// === HTTP EVENT HANDLER ===
static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    if (evt->event_id == HTTP_EVENT_ON_DATA && evt->data_len > 0) {
        if (http_buf_len + evt->data_len < MAX_HTTP_OUTPUT_BUFFER) {
            memcpy(http_buffer + http_buf_len, evt->data, evt->data_len);
            http_buf_len += evt->data_len;
        }
    }
    return ESP_OK;
}








// === FLEX SAMPLING ===
static void sample_and_log_flex() {
    for (int s = 0; s < SENSOR_COUNT; s++) {
        int raw = adc1_get_raw(flex_channels[s]);
        float voltage = (raw / 4095.0f) * 3.3f;
        readings[s][sample_index] = voltage;
    }
    sample_index++;
    if (sample_index >= SAMPLE_COUNT) {
        float pct_arr[SENSOR_COUNT];








        for (int s = 0; s < SENSOR_COUNT; s++) {
            // 1) compute mean & var
            float sum = 0, var = 0;
            for (int j = 0; j < SAMPLE_COUNT; j++) {
                sum += readings[s][j];
            }
            float mean = sum / SAMPLE_COUNT;
            for (int j = 0; j < SAMPLE_COUNT; j++) {
                float d = readings[s][j] - mean;
                var += d * d;
            }
            float stddev = sqrtf(var / SAMPLE_COUNT);








            // 2) filter out outliers and recompute avg
            float fsum = 0; int cnt = 0;
            for (int j = 0; j < SAMPLE_COUNT; j++) {
                if (fabsf(readings[s][j] - mean) <= stddev) {
                    fsum += readings[s][j];
                    cnt++;
                }
            }
            float avg = cnt ? (fsum / cnt) : mean;








            // 3) map to percentage
            float range = MAX_VOLTAGE - baseline[s];
            float pct = (range > 0)
                       ? ((avg - baseline[s]) / range) * 100.0f
                       : 0.0f;
            pct = fmaxf(0, fminf(100, pct));
            // apply gamma curve
            pct = powf(pct/100.0f, FLEX_SENSITIVITY_GAMMA) * 100.0f;








            ESP_LOGI("FLEX", "Sensor %d | Voltage: %.3f V | Flex: %.1f%%",
                s, avg, pct);
            pct_arr[s] = pct;
        }








        // if recording, stash them + timestamp (unchanged)
        if (recording_results && result_sample_count < MAX_RESULT_SAMPLES) {
            result_thumb[    result_sample_count ] = pct_arr[0];
            result_index_vals[result_sample_count ] = pct_arr[1];
            result_middle[   result_sample_count ] = pct_arr[2];
            result_ring[     result_sample_count ] = pct_arr[3];
            result_pinky[    result_sample_count ] = pct_arr[4];
            result_timestamps[result_sample_count ] =
                result_sample_count * (SAMPLE_COUNT * SAMPLE_INTERVAL_MS) / 1000.0f;
            result_sample_count++;
        }








        sample_index = 0;
    }
}








// === TIMING INTERPOLATION ===
void set_durations_for_value(float v) {
    if (v <= 0) {
        one_rot_ms = two_rot_ms = one_rev_ms = two_rev_ms = 0;
        return;
    }
    if (v > 60) v = 60;
    if (v <= 30) {
        float f = v / 30.0f;
        one_rot_ms    = f * A30_1;
        two_rot_ms    = f * A30_2;
        one_rev_ms    = f * A30_1rev;
        two_rev_ms    = f * A30_2rev;
    } else {
        float f = (v - 30) / 30.0f;
        one_rot_ms    = A30_1    + f * (A60_1    - A30_1);
        two_rot_ms    = A30_2    + f * (A60_2    - A30_2);
        one_rev_ms    = A30_1rev + f * (A60_1rev - A30_1rev);
        two_rev_ms    = A30_2rev + f * (A60_2rev - A30_2rev);
    }
}








// === SERVO HELPERS ===
uint32_t ms_to_duty(float ms) {
    return (uint32_t)((ms / 20.0f) * ((1 << 13) - 1));
}
void setup_servo(int gpio, ledc_channel_t ch) {
    ledc_channel_config_t cfg = {
        .gpio_num   = gpio,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = ch,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&cfg);
}
void set_servo_pulse(ledc_channel_t ch, float pulse_ms) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, ch, ms_to_duty(pulse_ms));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, ch);
}
void stop_all_servos() {
    set_servo_pulse(SERVO1_CHANNEL, SERVO_STOP_MS);
    ledc_stop(LEDC_LOW_SPEED_MODE, SERVO1_CHANNEL, 0);
    set_servo_pulse(SERVO2_CHANNEL, SERVO_STOP_MS);
    ledc_stop(LEDC_LOW_SPEED_MODE, SERVO2_CHANNEL, 0);
}
bool boot_button_pressed() { return gpio_get_level(BOOT_BTN_GPIO) == 0; }








// === INTERRUPTIBLE MOTION ===
void run_servo_with_interrupt(ledc_channel_t ch, float pulse_ms, int dur_ms) {
    set_servo_pulse(ch, pulse_ms);
    int elapsed = 0, flex_timer = 0;
    while (elapsed < dur_ms) {
        if (boot_button_pressed()) { stop_all_servos(); return; }
        vTaskDelay(pdMS_TO_TICKS(10));
        elapsed += 10; flex_timer += 10;
        if (flex_timer >= SAMPLE_INTERVAL_MS) {
            sample_and_log_flex(); flex_timer = 0;
        }
    }
    ledc_stop(LEDC_LOW_SPEED_MODE, ch, 0);
}








// === EXERCISE SEQUENCE ===
void run_servo_sequence(int reps) {
    for (int rep = 0; rep < reps; rep++) {
        if (boot_button_pressed()) break;
        //Clockwise turn to reach inputted flex data
        run_servo_with_interrupt(SERVO1_CHANNEL, SERVO_MIN_MS, (int)one_rot_ms);
        //Unwind the string by running servos 1 counterclockwise
        run_servo_with_interrupt(SERVO1_CHANNEL, SERVO_MAX_MS, (int)one_rev_ms);
        //Pause a bit before winding servos 2
        vTaskDelay(pdMS_TO_TICKS(PAUSE_BETWEEN_SETS_MS)); if (boot_button_pressed()) break;
        //Clockwise servos 2 to bring finger back to resting position
        run_servo_with_interrupt(SERVO2_CHANNEL, SERVO_MIN_MS, (int)two_rot_ms);
        //Counterclockwise servos 2 to loosen up string
        run_servo_with_interrupt(SERVO2_CHANNEL, SERVO_MAX_MS, (int)two_rev_ms);
        //Pause after completed 1 rep
        vTaskDelay(pdMS_TO_TICKS(PAUSE_BETWEEN_SETS_MS));
    }
    stop_all_servos();
    ESP_LOGI(TAG, "Exercise Completed");
}








// === HTTP + EXERCISE TASK ===
static void http_runner_task(void *pv) {
    while (1) {
        // --- HTTP GET ---
        http_buf_len = 0;
        http_buffer[0] = '\0';
        esp_http_client_config_t get_cfg = {
            .url            = RUNNER_URL,
            .transport_type = HTTP_TRANSPORT_OVER_SSL,
            .cert_pem       = isrg_root_x1_pem,
            .event_handler  = _http_event_handler,
        };
        esp_http_client_handle_t client = esp_http_client_init(&get_cfg);
        esp_err_t err = esp_http_client_perform(client);








        if (err == ESP_OK) {
            http_buffer[http_buf_len] = '\0';
            ESP_LOGI(TAG, "GET status = %d, len = %d",
                     esp_http_client_get_status_code(client), http_buf_len);
            ESP_LOGI(TAG, "Response JSON: %s", http_buffer);








            cJSON *root = cJSON_Parse(http_buffer);
            if (root && cJSON_IsArray(root) && cJSON_GetArraySize(root) > 0) {
                cJSON *item = cJSON_GetArrayItem(root, 0);








                // --- STEP 1: grab nested “data” object ---
                cJSON *data = cJSON_GetObjectItemCaseSensitive(item, "data");
                if (!data || !cJSON_IsObject(data)) {
                    ESP_LOGW(TAG, "Entry missing 'data' object");
                    goto CLEANUP_AND_RETRY;
                }








                // --- STEP 2: validate hardware_id ---
                cJSON *jhw = cJSON_GetObjectItemCaseSensitive(data, "hardware_id");
                if (!jhw)  // fallback if older key used
                    jhw = cJSON_GetObjectItemCaseSensitive(data, "hw_id");
                bool hw_valid = false;
                if (jhw && cJSON_IsString(jhw) && jhw->valuestring) {
                    for (int i = 0; i < valid_hw_count; i++) {
                        if (strcmp(jhw->valuestring, valid_hw_ids[i]) == 0) {
                            hw_valid = true;
                            break;
                        }
                    }
                }
                if (!hw_valid) {
                    ESP_LOGW(TAG, "Invalid hw_id: %s",
                             jhw && jhw->valuestring ? jhw->valuestring : "(null)");








                    // IMMEDIATELY DELETE invalid entry
                    {
                        esp_http_client_cleanup(client);
                        esp_http_client_config_t del_cfg = {
                            .url            = RUNNER_URL,
                            .transport_type = HTTP_TRANSPORT_OVER_SSL,
                            .cert_pem       = isrg_root_x1_pem,
                        };
                        esp_http_client_handle_t del_client = esp_http_client_init(&del_cfg);
                        esp_http_client_set_method(del_client, HTTP_METHOD_DELETE);
                        if (esp_http_client_perform(del_client) == ESP_OK) {
                            ESP_LOGI(TAG, "DELETE(status=%d)",
                                     esp_http_client_get_status_code(del_client));
                        } else {
                            ESP_LOGE(TAG, "DELETE failed");
                        }
                        esp_http_client_cleanup(del_client);
                    }
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    continue;
                }








                // --- STEP 3: extract device_id, flexion & reps ---
                cJSON *jdevice = cJSON_GetObjectItemCaseSensitive(item, "device_id");
                cJSON *jflex   = cJSON_GetObjectItemCaseSensitive(data, "flexion");
                cJSON *jreps   = cJSON_GetObjectItemCaseSensitive(data, "reps");








                if (jdevice && cJSON_IsString(jdevice) &&
                    jflex   && cJSON_IsNumber(jflex) &&
                    jreps   && cJSON_IsNumber(jreps))
                {
                    float intensity = (float)jflex->valuedouble;
                    int   reps      =            jreps->valueint;








                    // reset & enable flex recording
                    result_sample_count = 0;
                    recording_results   = true;








                    set_durations_for_value(intensity);
                    run_servo_sequence(reps);




                    // *** extra at-rest sampling ***
                    for (int i = 0; i < SAMPLE_COUNT; i++) {
                        sample_and_log_flex();
                        vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL_MS));
                    }




                    // disable recording
                    recording_results = false;








                    // --- STEP 3.5: POST collected flex arrays to /results/{device_id} ---
                    char results_url[128];
                    snprintf(results_url, sizeof(results_url),
                             "https://therahand.info/v0/results/%s",
                             jdevice->valuestring);








                    cJSON *post_root = cJSON_CreateObject();
                    // top‑level “data” object as required by the POST schema
                    cJSON *data_obj = cJSON_CreateObject();
                    cJSON_AddItemToObject(post_root, "data", data_obj);
               
                    // 1) name array
                    cJSON *arr_name = cJSON_CreateArray();
                    cJSON_AddItemToArray(arr_name,
                        cJSON_CreateString(
                            cJSON_GetObjectItem(data, "name")->valuestring
                        ));
                    cJSON_AddItemToObject(data_obj, "name", arr_name);
               
                    // 2) five flex arrays + timestamps
                    #define ADD_ARRAY_D(field, buf)                      \
                    do {                                               \
                        cJSON *a = cJSON_CreateArray();                 \
                        for (int i = 0; i < result_sample_count; i++) { \
                        cJSON_AddItemToArray(a,                       \
                            cJSON_CreateNumber(buf[i]));                \
                        }                                                \
                        cJSON_AddItemToObject(data_obj, field, a);      \
                    } while (0)
               
                    ADD_ARRAY_D("thumb",      result_thumb);
                    ADD_ARRAY_D("index",      result_index_vals);
                    ADD_ARRAY_D("middle",     result_middle);
                    ADD_ARRAY_D("ring",       result_ring);
                    ADD_ARRAY_D("pinky",      result_pinky);
                    ADD_ARRAY_D("timestamp_s",result_timestamps);
                    #undef ADD_ARRAY_D
                   
                    // grab the original exercise parameters from the same data object
                    cJSON *jfinger = cJSON_GetObjectItemCaseSensitive(data, "finger");
                    // jreps and jflex were already fetched above:
                    //    cJSON *jreps = cJSON_GetObjectItemCaseSensitive(data, "reps");
                    //    cJSON *jflex = cJSON_GetObjectItemCaseSensitive(data, "flexion");








                    // --- NEW: append finger, reps & flexion arrays ---
                    if (jfinger && cJSON_IsString(jfinger) &&
                        jreps   && cJSON_IsNumber(jreps) &&
                        jflex   && cJSON_IsNumber(jflex))
                    {
                        // finger array
                        cJSON *arr_finger = cJSON_CreateArray();
                        cJSON_AddItemToArray(arr_finger,
                            cJSON_CreateString(jfinger->valuestring));
                        cJSON_AddItemToObject(data_obj, "finger", arr_finger);








                        // reps array
                        cJSON *arr_reps = cJSON_CreateArray();
                        cJSON_AddItemToArray(arr_reps,
                            cJSON_CreateNumber(jreps->valueint));
                        cJSON_AddItemToObject(data_obj, "reps", arr_reps);








                        // flexion array
                        cJSON *arr_flexion = cJSON_CreateArray();
                        cJSON_AddItemToArray(arr_flexion,
                            cJSON_CreateNumber(jflex->valueint));
                        cJSON_AddItemToObject(data_obj, "flexion", arr_flexion);
                    }








                    // print & POST
                    char *payload = cJSON_PrintUnformatted(post_root);








                    // add these lines to actually create post_client
                    esp_http_client_config_t post_cfg = {
                        .url            = results_url,
                        .transport_type = HTTP_TRANSPORT_OVER_SSL,
                        .cert_pem       = isrg_root_x1_pem,
                    };
                    esp_http_client_handle_t post_client = esp_http_client_init(&post_cfg);
                    esp_http_client_set_method(post_client, HTTP_METHOD_POST);








                    // now you can safely set headers and body
                    esp_http_client_set_header( post_client,
                                                "Content-Type",
                                                "application/json");
                    esp_http_client_set_post_field( post_client,
                                                    payload,
                                                    strlen(payload));








                    // perform & cleanup
                    if (esp_http_client_perform(post_client) == ESP_OK) {
                        ESP_LOGI(TAG, "POST results status = %d",
                                esp_http_client_get_status_code(post_client));
                    } else {
                        ESP_LOGE(TAG, "POST results failed: %s",
                                esp_err_to_name(esp_http_client_perform(post_client)));
                    }
                    esp_http_client_cleanup(post_client);
                    free(payload);
                    cJSON_Delete(post_root);
                }








                // --- STEP 4: now DELETE the runner assignment ---
                vTaskDelay(pdMS_TO_TICKS(5000));
                {
                    esp_http_client_config_t del_cfg = {
                        .url            = RUNNER_URL,
                        .transport_type = HTTP_TRANSPORT_OVER_SSL,
                        .cert_pem       = isrg_root_x1_pem,
                    };
                    esp_http_client_handle_t del_client = esp_http_client_init(&del_cfg);
                    esp_http_client_set_method(del_client, HTTP_METHOD_DELETE);
                    if (esp_http_client_perform(del_client) == ESP_OK) {
                        ESP_LOGI(TAG, "DELETE status = %d",
                                 esp_http_client_get_status_code(del_client));
                    } else {
                        ESP_LOGE(TAG, "DELETE failed");
                    }
                    esp_http_client_cleanup(del_client);
                }








                cJSON_Delete(root);
                esp_http_client_cleanup(client);
            } else {
                if (root) cJSON_Delete(root);
            }
        } else {
            ESP_LOGE(TAG, "GET failed: %s", esp_err_to_name(err));
        }








    CLEANUP_AND_RETRY:
        esp_http_client_cleanup(client);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
















void app_main(void) {
    // NVS init
    esp_err_t r = nvs_flash_init();
    if (r == ESP_ERR_NVS_NO_FREE_PAGES || r == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    wifi_init();
    // baseline
    adc1_config_width(ADC_WIDTH_BIT_12);
    for (int i = 0; i < SENSOR_COUNT; i++) {
        adc1_config_channel_atten(flex_channels[i], ADC_ATTEN_DB_11);
        float sum = 0;
        for (int n = 0; n < CALIB_SAMPLES; n++) {
            int raw = adc1_get_raw(flex_channels[i]);
            sum += (raw / 4095.0f) * 3.3f;
            vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL_MS));
        }
        baseline[i] = sum / CALIB_SAMPLES;
        ESP_LOGI(TAG, "Sensor %d baseline: %.3f V", i, baseline[i]);
    }
    sample_index = 0;
    // PWM setup
    ledc_timer_config_t t = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .duty_resolution = SERVO_PWM_RES,
        .timer_num       = LEDC_TIMER_0,
        .freq_hz         = SERVO_PWM_FREQ,
        .clk_cfg         = LEDC_AUTO_CLK
    };
    ledc_timer_config(&t);
    setup_servo(SERVO1_GPIO, SERVO1_CHANNEL);
    setup_servo(SERVO2_GPIO, SERVO2_CHANNEL);
    // start task
    xTaskCreate(http_runner_task, "http_runner", 16*1024, NULL, 5, NULL);
}
























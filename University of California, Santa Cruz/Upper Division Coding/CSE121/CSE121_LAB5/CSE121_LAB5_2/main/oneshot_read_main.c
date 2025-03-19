#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

// ADC Configuration
#define ADC_CHANNEL ADC_CHANNEL_2           // GPIO2
#define ADC_ATTEN ADC_ATTEN_DB_12           // 0-3.3V range
#define THRESHOLD 980                       // Signal threshold for high
#define UNIT_DURATION_MS 100               // 1 Morse unit = 1000ms (1 second)

// Morse Timing (in units)
#define DOT_UNITS 1
#define DASH_UNITS 3
#define SYMBOL_GAP_UNITS 2
#define LETTER_GAP_UNITS 5
#define WORD_GAP_UNITS 7

// Morse Code Map
const char *MORSE_CODE[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---",
    "...--", "....-", ".....", "-....", "--...", "---..", "----."
};
const char LETTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Function Prototypes
void decode_morse(const char *morse);
void reset_buffer(char *buffer, int *index);

// Main Application
void app_main(void) {
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t adc_init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&adc_init_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t channel_cfg = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &channel_cfg);

    int adc_value = 0;
    int on_counter = 0;
    int off_counter = 0;
    char morse_buffer[128] = "";
    int buffer_index = 0;
    while (1) {
        adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_value);
        if (adc_value > THRESHOLD || off_counter==7) {
            // Signal detected
	        if (adc_value > THRESHOLD) on_counter++;
		if (off_counter >= LETTER_GAP_UNITS && off_counter < WORD_GAP_UNITS) {
                    // Gap between letters
                    decode_morse(morse_buffer);
                    reset_buffer(morse_buffer, &buffer_index);
                } else if (off_counter >= WORD_GAP_UNITS) {
                    // Gap between words
                    decode_morse(morse_buffer);
                    reset_buffer(morse_buffer, &buffer_index);
	            printf("\n");
                }
                off_counter = 0;  // Reset unit counter
        } else {
            // No signal
                // Transition
		off_counter++;
		if (on_counter == DOT_UNITS) {
                    morse_buffer[buffer_index++] = '.';
                } else if (on_counter == DASH_UNITS) {
                    morse_buffer[buffer_index++] = '-';
                }
		on_counter = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(UNIT_DURATION_MS));  // Increment unit time
    }
}

// Decode Morse Code
void decode_morse(const char *morse) {
    for (int i = 0; i < sizeof(MORSE_CODE) / sizeof(MORSE_CODE[0]); i++) {
        if (strcmp(morse, MORSE_CODE[i]) == 0) {
            printf("%c", LETTERS[i]);
            return;
        }
    }
    printf("?");  // Unknown Morse sequence
}

// Reset Morse Buffer
void reset_buffer(char *buffer, int *index) {
    for (int i = 0; i<128; i++) buffer[i] = '\0';
    *index = 0;
}


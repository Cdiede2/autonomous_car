#include "setup.h"

void setup() {
    const char *TAG = "SETUP";
    ESP_LOGI(TAG, "Setting up...");

    // Setup GPIO
    unsigned long long gpio_mask = 0ULL;
    gpio_mask |= (1ULL << CLK_OUTP_PIN);  // GPIO2
    gpio_mask |= (1ULL << SI_PULSE_PIN); // GPIO11    

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,         // Disable interrupts
        .mode = GPIO_MODE_OUTPUT,               // Set as output mode
        .pin_bit_mask = gpio_mask,              // Set GPIO11 as output
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
        .pull_up_en = GPIO_PULLUP_DISABLE       // Disable pull-up
    };

    gpio_config(&io_conf); // Configure GPIO

    // Setup Timer
    esp_timer_handle_t timer_handle;
    const esp_timer_create_args_t timer_args = {
        .callback = &timer_isr,                 // Timer ISR callback
        .name = "timer"                         // Timer name
    };

    adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC width to 12 bits
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // Set attenuation

    esp_timer_create(&timer_args, &timer_handle);   // Create timer
    esp_timer_start_periodic(timer_handle, 20); 
    return;
}
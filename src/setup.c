#include "setup.h"

// Timer Configuration
#define TIMER_PERIOD_LF 4 // Timer period in milliseconds
#define TIMER_PERIOD_HF 2 // Timer period in milliseconds


void setup() {
    const char *TAG = "SETUP";
    ESP_LOGI(TAG, "Setting up...");

    //////////////////
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
    //////////////////


    //////////////////
    // Setup Low Frequency Timer
    esp_timer_handle_t lf_timer_handle;


    const esp_timer_create_args_t lf_timer_args = {
        .callback   = &lf_timer_isr,                // Timer ISR callback
        .name       = "timer"                       // Timer name
    };


    esp_timer_create(&lf_timer_args, &lf_timer_handle);   // Create timer
    esp_timer_start_periodic(lf_timer_handle, TIMER_PERIOD_LF * 1000); 
    //////////////////
    

    //////////////////
    // Setup High Frequency Timer
    esp_timer_handle_t hf_timer_handle;

    const esp_timer_create_args_t hf_timer_args = {
        .callback   = &hf_timer_isr,                // Timer ISR callback
        .name       = "timer"                       // Timer name
    };

    esp_timer_create(&hf_timer_args, &hf_timer_handle);
    esp_timer_start_periodic(hf_timer_handle, TIMER_PERIOD_HF * 1000);
    //////////////////
    
    adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC width to 12 bits
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0); // Set attenuation
    return;
}
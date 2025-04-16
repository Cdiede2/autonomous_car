#include "setup.h"

// Timer Configuration
#define TIMER_PERIOD_LF 5000 // Timer half-period in microseconds
#define TIMER_PERIOD_HF 2500 // Timer half-period in microseconds

void setup()
{
    const char *TAG = "SETUP";
    ESP_LOGI(TAG, "Setting up...");

    //////////////////
    // Setup GPIO
    unsigned long long gpio_mask = 0ULL;
    gpio_mask |= (1ULL << CLK_OUTP_PIN); // GPIO2
    gpio_mask |= (1ULL << SI_PULSE_PIN); // GPIO11

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,        // Disable interrupts
        .mode = GPIO_MODE_OUTPUT,              // Set as output mode
        .pin_bit_mask = gpio_mask,             // Set GPIO11 as output
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
        .pull_up_en = GPIO_PULLUP_DISABLE      // Disable pull-up
    };

    gpio_config(&io_conf); // Configure GPIO
    //////////////////

    //////////////////
    // Setup Low Frequency Timer
    esp_timer_handle_t lf_timer_handle;

    const esp_timer_create_args_t lf_timer_args = {
        .callback = &lf_timer_isr, // Timer ISR callback
        .name = "timer"            // Timer name
    };

    esp_timer_create(&lf_timer_args, &lf_timer_handle); // Create timer
    esp_timer_start_periodic(lf_timer_handle, TIMER_PERIOD_LF);
    //////////////////

    //////////////////
    // Setup High Frequency Timer
    esp_timer_handle_t hf_timer_handle;

    const esp_timer_create_args_t hf_timer_args = {
        .callback = &hf_timer_isr, // Timer ISR callback
        .name = "timer"            // Timer name
    };

    esp_timer_create(&hf_timer_args, &hf_timer_handle);
    esp_timer_start_periodic(hf_timer_handle, TIMER_PERIOD_HF);
    //////////////////


    // Configure Motor PWM
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_NUM_12);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_NUM_13);

    mcpwm_config_t pwm_config = {
        .frequency = 100,       // Frequency = 100Hz
        .cmpr_a = 50.0,       // Duty cycle of PWMxA = 50.0%
        .cmpr_b = 50.0,       // Duty cycle of PWMxB = 50.0%
        .counter_mode = MCPWM_UP_COUNTER, // Up counter mode
    };

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); // Initialize with the configuration
    
    // Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);                       // Set ADC width to 12 bits
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0); // Set attenuation
    return;
}
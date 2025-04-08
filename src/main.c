#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/adc.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

#include "esp_system.h"

/* Function Prototypes 
 *  (void) setup_adc(): Configure ADC (Bitwidth and Channel Attenuation)
 *  (int) read_adc():   Read ADC value from ADC1_CHANNEL_6
 * 
 * 
 */

// Configure and Read ADC
void setup_adc();
int read_adc();

// Configure GPIO
void setup_gpio();

// Configure PWM
void setup_pwm();


/* *
 * @brief Main function
 *
 *
 */
void app_main() {
    const char *TAG = "APP_MAIN";
    int value = 0;

    // Initialize the ESP32C6
    ESP_LOGI(TAG, "Initializing ESP32C6...");
    
    // (void)setup_gpio();     // Setup GPIO
    (void)setup_adc();      // Setup ADC
    (void)setup_pwm();      // Setup PWM

    // Body of the main function
    while(true) {
        // (void)gpio_set_level(GPIO_NUM_2, value); // Set GPIO2 to high
        
        ESP_LOGI(TAG, "ADC Value: %d", read_adc()); // Read ADC value
        (void)vTaskDelay(pdMS_TO_TICKS(25));        // Delay for 25 ms
        value ^= 0x01;
    }
    return;
}

void setup_pwm() {
    const char *TAG = "PWM_SETUP";

    ESP_LOGI(TAG, "Setting up PWM...");

    ledc_timer_config_t config = {};
    config.speed_mode   = LEDC_LOW_SPEED_MODE; // Use low speed mode
    config.timer_num    = LEDC_TIMER_0; // Use timer 0
    config.duty_resolution = LEDC_TIMER_13_BIT; // Set resolution to 13 bits
    config.freq_hz     = 1000; // Set frequency to 1 kHz 
    config.clk_cfg     = LEDC_AUTO_CLK; // Use automatic clock configuration
    ledc_timer_config(&config); // Apply the timer configuration

    ledc_channel_config_t pwm_config = {};
    pwm_config.speed_mode = LEDC_LOW_SPEED_MODE; // Use low speed mode
    pwm_config.channel    = LEDC_CHANNEL_0; // Use channel 0
    pwm_config.timer_sel  = LEDC_TIMER_0; // Use timer 0
    pwm_config.intr_type = LEDC_INTR_DISABLE; // Disable interrupts
    pwm_config.gpio_num  = GPIO_NUM_2; // Use GPIO2
    pwm_config.duty      = 4096; // Set initial duty cycle to 0
    pwm_config.hpoint    = 0; // Set high point to 0
    ledc_channel_config(&pwm_config); // Apply the channel configuration
    return;
}

void setup_gpio() {
    const char *TAG = "GPIO_SETUP";
    ESP_LOGI(TAG, "Setting up GPIO...");

    // GPIO setup code here
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE; // Disable interrupts
    io_conf.mode = GPIO_MODE_OUTPUT; // Set as output mode
    io_conf.pin_bit_mask = (1ULL << GPIO_NUM_2); // GPIO2
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pull-down
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE; // Disable pull-up
    gpio_config(&io_conf); // Apply the configuration
    ESP_LOGI(TAG, "GPIO setup complete.");
    return;
}   

void setup_adc() {
    const char *TAG = "ADC_SETUP";
    ESP_LOGI(TAG, "Setting up ADC...");
    adc1_config_width(ADC_BITWIDTH_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);     // Check Pinout
    return;
}

int read_adc() {
    return adc1_get_raw(ADC1_CHANNEL_6); // Check Pinout
}
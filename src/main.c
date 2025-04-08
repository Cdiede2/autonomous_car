#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/adc.h"
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

void setup_gpio();

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
    
    // (void)setup_adc(); // Setup ADC
    (void)setup_gpio(); // Setup GPIO


    // Body of the main function
    while(true) {
        ESP_LOGI(TAG, "Hello, ESP32C6!");
        (void)gpio_set_level(GPIO_NUM_2, value); // Set GPIO2 to high
        (void)vTaskDelay(pdMS_TO_TICKS(1000));
        value ^= 0x01;
    }
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "esp_system.h"

#define PWM_FREQUENCY 1000 // 1 kHz
#define PWM_RESOLUTION LEDC_TIMER_13_BIT // 13-bit resolution

/* Function Prototypes
 *  (void) setup_adc(): Configure ADC (Bitwidth and Channel Attenuation)
 *  (int) read_adc():   Read ADC value from ADC1_CHANNEL_6
 *
 *
 */

/* * Function Prototypes
 *  (void) setup():     Initialize GPIO, Timer, and ADC configurations
 *  (void) app_main(): Main function of the application
 *
 *  (void) send_pulse(): Send a pulse signal to GPIO2
 *  (void) timer_isr(): Timer interrupt service routine
 *
 */


/** 
 * @brief Reads the ADC value from ADC1_CHANNEL_6. 
*/
int read_adc();

/**
 * @brief Sends a pulse signal to GPIO2.
 * 
 * This function is typically used to generate a pulse signal on a specific GPIO pin.
 * The pulse duration and frequency can be configured based on the application requirements.
 */
void send_pulse() {
    const char *TAG = "SEND_PULSE";
    ESP_LOGI(TAG, "Sending pulse...");
    return;
}

/**
 * @brief Timer interrupt service routine (ISR).
 * 
 * This function is called when the timer interrupt occurs. It handles the timer
 * events and performs actions such as toggling GPIO states or updating variables.
 * The ISR should be kept short and efficient to avoid blocking other tasks.
 */
static void IRAM_ATTR timer_isr(void *arg) {
    const char *TAG = "TIMER_ISR";
    static int value = 0;
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0); // Clear interrupt status
    
    value ^= 0x01; // Toggle value
    
    ESP_LOGI(TAG, "Timer ISR triggered...");
    (void)gpio_set_level(GPIO_NUM_2, value); // Set GPIO2 HIGH

    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0); // Enable alarm in ISR
    // return pdFALSE; // Indicate that the ISR does not yield
}

/**
 * @brief Initializes the program's environment and hardware configurations.
 * 
 * This function is typically called once at the beginning of the program's execution.
 * It is used to set up initial states, configure hardware peripherals, and prepare
 * the system for the main execution loop. Common tasks include initializing pins,
 * setting up communication protocols, and configuring timers.
 */
void setup() {
    const char *TAG = "SETUP";
    ESP_LOGI(TAG, "Setting up...");

    // Setup GPIO
    unsigned long long gpio_mask = 0ULL;
    gpio_mask |= (1ULL << GPIO_NUM_2); // GPIO2
    gpio_mask |= (1ULL << GPIO_NUM_11); // GPIO11    

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,         // Disable interrupts
        .mode = GPIO_MODE_OUTPUT,               // Set as output mode
        .pin_bit_mask = gpio_mask,              // Set GPIO11 as output
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
        .pull_up_en = GPIO_PULLUP_DISABLE       // Disable pull-up
    };

    // Setup Timer
    timer_config_t timer_conf = {
        .divider = 80,                      // Set timer divider to 80 (1 MHz)
        .counter_dir = TIMER_COUNT_UP,      // Count up
        .counter_en = TIMER_PAUSE,          // Pause the timer
        .alarm_en = TIMER_ALARM_DIS,        // Disable alarm
        .auto_reload = false,               // Disable auto-reload
    };
    
    // Setup ADC
    adc1_config_width(ADC_BITWIDTH_12);     // Set ADC bitwidth to 12 bits
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // Set ADC channel attenuation

    // Apply the configuration
    gpio_config(&io_conf);                              // Apply input configuration
    timer_init(TIMER_GROUP_0, TIMER_0, &timer_conf);    // Apply timer configuration


    // Setup Timer and PWM
    uint64_t timer_period = 1000000 / PWM_FREQUENCY;
    timer_set_counter_value( TIMER_GROUP_0, TIMER_0, 0);            // Set timer counter value to 0
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, (int)1E6);    // Set timer alarm value 
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);                      // Enable timer interrupt
    
    // Register ISR
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_start(TIMER_GROUP_0, TIMER_0);                          // Start the timer

    // ledc_channel_config(&channel_conf); // Apply channel configuration
    // ledc_timer_config(&timer_conf);     // Apply timer configuration
    ESP_LOGI(TAG, "Setup complete.");
    return;
}

/**
 * @brief Main function of the application.
 * 
 * This function serves as the entry point for the program. It initializes the
 * system, sets up hardware configurations, and enters the main execution loop.
 * The main loop typically handles tasks such as reading sensors, processing data,
 * and controlling outputs.
 */
void app_main()
{
    const char *TAG = "APP_MAIN";
    (void)setup();
    
    ESP_LOGI(TAG, "Initializing ESP32C6...");
    
    
    int value = 0;

    // Initialize the ESP32C6

    // Body of the main function

    // DEBUG: Test GPIO11 is an Output Pin
    gpio_set_level(GPIO_NUM_11, 1);     // Set GPIO11 HIGH

    while (true)
    {   
        uint64_t val;

        // (void)gpio_set_level(GPIO_NUM_2, value); // Set GPIO2 to high
        // ESP_LOGI(TAG, "PWM Value: %lu", ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
        // ESP_LOGI(TAG, "ADC Value: %d", read_adc()); // Read ADC value
        ESP_LOGI(TAG, "In Control Loop...");
        // (void)gpio_set_level(GPIO_NUM_2, value);
        (void)vTaskDelay(pdMS_TO_TICKS(25)); // Delay for 25 ms
        
        // (void)timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &val); // Get timer value
        // ESP_LOGI(TAG, "Timer Value: %llu", val);
        value ^= 0x01;
    }
    return;
}

int read_adc()
{
    return adc1_get_raw(ADC1_CHANNEL_6); // Check Pinout
}
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "esp_system.h"

#include "esp_timer.h"      // Used to set Timer up for SI pulse
#include "driver/gpio.h"    // Used to drive GPIO pins
#include "driver/adc.h"     // Used to read ADC values


#define PWM_FREQUENCY 1000 // 1 kHz
#define PWM_RESOLUTION LEDC_TIMER_13_BIT // 13-bit resolution

static bool send_pulse_flag = false;
static uint64_t adc_value = 0;

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
void send_pulse();

/**
 * @brief Timer interrupt service routine (ISR).
 * 
 * This function is called when the timer interrupt occurs. It handles the timer
 * events and performs actions such as toggling GPIO states or updating variables.
 * The ISR should be kept short and efficient to avoid blocking other tasks.
 */
static void IRAM_ATTR timer_isr(void *arg);

/**
 * @brief Initializes the program's environment and hardware configurations.
 * 
 * This function is typically called once at the beginning of the program's execution.
 * It is used to set up initial states, configure hardware peripherals, and prepare
 * the system for the main execution loop. Common tasks include initializing pins,
 * setting up communication protocols, and configuring timers.
 */

void setup();

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
    ESP_LOGI(TAG, "Initializing ESP32C6 Timer...");
    (void)setup();

    gpio_set_level(GPIO_NUM_19, 1);
    /* Delay in main */
    while (true)
    {   
        (void)send_pulse();
        vTaskDelay(pdMS_TO_TICKS(250)); // Delay for 250ms
        adc_value = read_adc(); // Read ADC value
        ESP_LOGI(TAG, "ADC Value: %llu", adc_value); // Log ADC value
        vTaskDelay(pdMS_TO_TICKS(250)); // Delay for 250ms
    }
    return;
}


void setup() {
    const char *TAG = "SETUP";
    ESP_LOGI(TAG, "Setting up...");

    // Setup GPIO
    unsigned long long gpio_mask = 0ULL;
    gpio_mask |= (1ULL << GPIO_NUM_2);  // GPIO2
    gpio_mask |= (1ULL << GPIO_NUM_11); // GPIO11    
    gpio_mask |= (1ULL << GPIO_NUM_19); // GPIO19

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

    esp_timer_create(&timer_args, &timer_handle);   // Create timer
    esp_timer_start_periodic(timer_handle, 50000); // Start timer with 50ms period
}

void send_pulse() {
    const char *TAG = "SEND_PULSE";
    ESP_LOGI(TAG, "Sending pulse...");
    send_pulse_flag = true; // Set the pulse flag to true
    return;
}

int read_adc()
{
    return adc1_get_raw(ADC1_CHANNEL_6); // Check Pinout
}

static void IRAM_ATTR timer_isr(void *arg) {
    const char *TAG = "TIMER_ISR";
    static int value = 0;

    gpio_set_level(GPIO_NUM_2, value);

    if (send_pulse_flag && !value) {
        ESP_LOGI(TAG, "Sending pulse...");
        vTaskDelay(pdMS_TO_TICKS(25)); // Delay for 25ms
        gpio_set_level(GPIO_NUM_11, 1); // Set GPIO11 high
    } else if (send_pulse_flag && value) {
        ESP_LOGI(TAG, "Sending pulse...");
        send_pulse_flag = false;
        vTaskDelay(pdMS_TO_TICKS(25)); // Delay for 25ms
        gpio_set_level(GPIO_NUM_11, 0); // Set GPIO11 low
    }

    value = !value; // Toggle the value
    return;
}
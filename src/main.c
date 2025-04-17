#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "esp_system.h"

#include "esp_timer.h"   // Used to set Timer up for SI pulse
#include "driver/gpio.h" // Used to drive GPIO pins
#include "driver/adc.h"  // Used to read ADC values

#include "setup.h"
#include "camera.h"

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

    /* Delay in main */
    while (true)
    {
        uint32_t adc_value = 0;

        // (void)send_pulse();
        vTaskDelay(pdMS_TO_TICKS(20));           // Delay for 250ms
        adc_value = (uint32_t)read_camera_adc(); // Read ADC value

        // Print Boolean Representation of adc_value
        char binary_representation[17]; // Assuming a 32-bit ADC value
        for (int i = 15; i >= 0; i--)
        {
            binary_representation[16 - i] = (adc_value & (1 << i)) ? '1' : '0';
        }
        binary_representation[16] = '\0'; // Null-terminate the string
        ESP_LOGI(TAG, "%lu Base-2 Representation: %s", adc_value, binary_representation);
        // ESP_LOGI(TAG, "ADC Value: %llu", adc_value); // Log ADC value
        // vTaskDelay(pdMS_TO_TICKS(250));              // Delay for 250ms
    }
    return;
}
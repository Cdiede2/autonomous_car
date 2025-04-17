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
#include "motor.h"
#include "servo.h"

/**
 * @brief Prints the binary representation of a 32-bit unsigned integer.
 * @param value The 32-bit unsigned integer to be printed in binary format.
 * @details This function converts the given integer value into its binary representation
 * and prints it to the console. The binary representation is displayed as a string of
 * 0s and 1s, with the most significant bit on the left. The function assumes a 32-bit
 * unsigned integer and uses a fixed-width format for the output.
 */
void print_binary( uint32_t value );

/**
 * @brief Main function of the application.
 *
 * @details This function serves as the entry point for the program. It initializes the
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
        print_binary(adc_value);
        forward_vel(50.0);

        const float lower_bound = 0.0310;
        const float upper_bound = 0.0600;
        for(int i = 0; i < 100; i++)
        {   

            set_servo_duty( ( i / 100.0 ) * ( upper_bound - lower_bound ) + lower_bound);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        for(int i = 100; i >= 0; i--)
        {   

            set_servo_duty( ( i / 100.0 ) * ( upper_bound - lower_bound ) + lower_bound);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        
    }
    return;
}

void print_binary( uint32_t value )
{
    char binary_representation[17]; // Assuming a 32-bit ADC value
    for (int i = 15; i >= 0; i--)
    {
        binary_representation[16 - i] = (value & (1 << i)) ? '1' : '0';
    }
    binary_representation[16] = '\0'; // Null-terminate the string
    ESP_LOGI("ADC", "%lu Base-2 Representation: %s", value, binary_representation);
}
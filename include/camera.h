#pragma once
#define CAMERA_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#define TIMER_PERIOD 2

static bool send_pulse_flag = false;
static uint64_t adc_value = 0;

/**
 * @brief Sends a pulse signal to GPIO2.
 *
 * This function is typically used to generate a pulse signal on a specific GPIO pin.
 * The pulse duration and frequency can be configured based on the application requirements.
 */
void send_pulse();

/**
 * @brief Reads the ADC value from ADC1_CHANNEL_6.
 */
int read_camera_adc(); 

/**
 * @brief Timer interrupt service routine (ISR).
 *
 * This function is called when the timer interrupt occurs. It handles the timer
 * events and performs actions such as toggling GPIO states or updating variables.
 * The ISR should be kept short and efficient to avoid blocking other tasks.
 */
static void IRAM_ATTR timer_isr(void *arg)
{
    const char *TAG = "TIMER_ISR";
    static char value = 0;
    static char clk_outp = 0;

    // gpio_set_level(GPIO_NUM_2, clk_outp);

    if (send_pulse_flag)
    {
        if (!value)
        {
            // ESP_LOGI(TAG, "Sending pulse...");
            vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 25ms
            gpio_set_level(GPIO_NUM_11, 1);               // Set GPIO11 high
        }
        else
        {
            // ESP_LOGI(TAG, "Sending pulse...");
            send_pulse_flag = false;
            vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 25ms
            gpio_set_level(GPIO_NUM_11, 0);               // Set GPIO11 low
        }
    }
    else if (value == 128)
    {
        adc_value = adc1_get_raw(ADC1_CHANNEL_6); // Read ADC value
        // ESP_LOGI(TAG, "Setting ADC Value to %llu", adc_value);
    }
    else if (value == 255)
    {
        send_pulse_flag = true; // Set the Pulse Flag
    }

    value += 1;
    clk_outp ^= 1;
    return;
}
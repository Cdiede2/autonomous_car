#pragma once
#define CAMERA_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#include <math.h>

#define AD_THRESH ((0.9/3.30) * 4095)
#define UPPER_CYCLE 220
#define LOWER_CYCLE 40

static bool send_pulse_flag = false;
static bool lf_timer_state = false;
static bool hf_timer_state = false;

// static uint64_t adc_value = 0;

/**
 * @brief Sends a pulse signal to GPIO2.
 *
 * @details Function is typically used to generate a pulse signal on a specific GPIO pin.
 * The pulse duration and frequency can be configured based on the application requirements.
 */
void send_pulse();

/**
 * @brief Reads the ADC value from ADC1_CHANNEL_6.
 */
void get_camera_adc(size_t bit);

/**
 * @brief Clear the ADC value from static variable
 */
void clear_camera_adc();

/**
 * @brief Reads the last measured ADC value from ADC1_CHANNEL_6.
 * @returns uint32_t ADC value from ADC1_CHANNEL_6.
 */
uint32_t read_camera_adc();

/**
 * @brief Low-Frequency Timer interrupt service routine (ISR).
 *  Produces the Clock output on GPIO2
 */
static void IRAM_ATTR lf_timer_isr(void *arg)
{
    const char TAG[] = "KMS";
    static int value = 0;
    static char clk_outp = 0;

    gpio_set_level(GPIO_NUM_2, lf_timer_state);
    lf_timer_state = !lf_timer_state;



    // Are we in a read state?

    //Read current pixel value.

    int bit = (value  - LOWER_CYCLE) * 31 / (UPPER_CYCLE - LOWER_CYCLE);
    if ( value > LOWER_CYCLE ) {
        (void)get_camera_adc(bit);
    }

    //  1 + #Number of Pixels
    if (value >= UPPER_CYCLE)
    {
        send_pulse_flag = true;
        // Enable hf_timer ISR here
        (void)clear_camera_adc();
        ESP_LOGI(TAG, "----------");
        value = 0;
    }
    value++;
    return;
}

/**
 * @brief High-Frequency Timer interrupt service routine (ISR).
 */
static void IRAM_ATTR hf_timer_isr(void *arg)
{
    hf_timer_state = !hf_timer_state;
    if (send_pulse_flag && hf_timer_state)
    {
        if (lf_timer_state)
        {
            gpio_set_level(GPIO_NUM_11, 1);
        }
        else
        {
            gpio_set_level(GPIO_NUM_11, 0);
            send_pulse_flag = false; // Reset the pulse flag
            // Disable this ISR here
        }
    }
    
    return;
}
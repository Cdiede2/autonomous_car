#pragma once
#define events_h

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"   // Used to read ADC values from camera
#include "driver/mcpwm.h" // Used to drive Motor
#include "driver/ledc.h"  // Used to drive Servo
#include "driver/timer.h"
#include "esp_timer.h" // Used to set Timer up for SI pulse
#include "esp_system.h"
#include "esp_log.h"
#include <math.h>

// #include "camera.h"

// GPIO pin definitions
#define CLK_OUTP_PIN GPIO_NUM_2
#define SI_PULSE_PIN GPIO_NUM_11

// ADC Camera Read Configuration
#define AD_THRESH ((0.9 / 3.30) * 4095)
#define UPPER_CYCLE 220
#define LOWER_CYCLE 40

// Timer Configuration
#define TIMER_PERIOD_LF 180 // Timer half-period in microseconds
#define TIMER_PERIOD_HF 90  // Timer half-period in microseconds

// Servo Configuration 
#define LOWER_BOUND 0.0310
#define UPPER_BOUND 0.0600
#define SERVO_FREQUENCY 50 // Frequency in Hz

static bool send_pulse_flag = false;
static bool lf_timer_state = false;
static bool hf_timer_state = false;
static uint32_t adc_value = 0; // Variable to store ADC value

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
 * @brief Drives motor control system in forward direction.
 * @param duty_cycle The duty cycle for the motor control signal (0-100%).
 * @details This function sets the duty cycle for the motor control signal to drive
 * the motor in the forward direction. The duty cycle determines the speed of the motor.
 * A higher duty cycle results in a faster motor speed, while a lower duty cycle results
 * in a slower speed. The function uses the MCPWM (Motor Control Pulse Width Modulation)
 * module to generate the PWM signal for the motor control.
 */
void forward_vel(float duty_cycle);

/**
 * @brief Drives motor control system in reverse direction.
 * @param duty_cycle The duty cycle for the motor control signal (0-100%).
 * @details This function sets the duty cycle for the motor control signal to drive
 * the motor in the reverse direction. The duty cycle determines the speed of the motor.
 * A higher duty cycle results in a faster motor speed, while a lower duty cycle results
 * in a slower speed. The function uses the MCPWM (Motor Control Pulse Width Modulation)
 * module to generate the PWM signal for the motor control.
 */
void reverse_vel(float duty_cycle);

/**
 * @brief Stops the motor control system.
 * @details This function stops the motor control system by setting the duty cycle
 * to zero. Additionally, sets lower GPIO pins of H-Bridge to HIGH to quickly
 * stop the motor.
 */
void stop_vel(void);

/**
 * @brief Sets up the servo motor.
 * @param ledc_channel The LEDC channel to be used for the servo.
 * @details This function initializes the LEDC channel for the servo motor.
 * It configures the timer, frequency, and duty cycle for the servo motor.
 * The servo motor is typically controlled using PWM signals, and this function
 * sets up the necessary parameters for generating those signals.
 */
void set_servo_duty(float duty_cycle);

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

    // Read current pixel value.

    int bit = (value - LOWER_CYCLE) * 31 / (UPPER_CYCLE - LOWER_CYCLE);
    if (value > LOWER_CYCLE)
    {
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

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
 * @brief Controls the servo motor using PID control.
 * @param setpoint The desired position of the servo motor.
 * @param measured_value The current position of the servo motor.
 * @param dt The time interval between control updates.
 * @return The control output for the servo motor.
 * @details This function implements a PID control algorithm to adjust the
 * position of the servo motor. It calculates the error between the desired
 * position (setpoint) and the current position (measured_value), and uses
 * proportional, integral, and derivative terms to compute the control output.
 * The control output is then used to adjust the duty cycle of the servo motor
 * to achieve the desired position. The function returns the control output
 * value, which can be used to set the duty cycle of the servo motor.
 * The PID parameters (Kp, Ki, Kd) can be tuned to achieve the desired response
 * characteristics of the servo motor. The function assumes that the servo motor
 */
float servo_control(float setpoint, float measured_value, float dt);

/**
 * @brief Prints the binary representation of a 32-bit unsigned integer.
 * @param value The 32-bit unsigned integer to be printed in binary format.
 * @details This function converts the given integer value into its binary representation
 * and prints it to the console. The binary representation is displayed as a string of
 * 0s and 1s, with the most significant bit on the left. The function assumes a 32-bit
 * unsigned integer and uses a fixed-width format for the output.
 */
void print_binary(uint32_t value);

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

    const float dt = 0.01; // Time step in seconds
    for (;;)
    {
        const char *TAG_CONTROL = "APP_MAIN::Control_Loop";

        uint32_t adc_value = 0;
        vTaskDelay(pdMS_TO_TICKS(50));
        adc_value = (uint32_t)read_camera_adc(); // Read ADC value
        print_binary((uint32_t)adc_value);

        // forward_vel(50.0);
        // const float lower_bound = 0.0310;
        // const float upper_bound = 0.0600;
        // for(int i = 0; i < 100; i++)
        // {

        //     set_servo_duty( ( i / 100.0 ) * ( upper_bound - lower_bound ) + lower_bound);
        //     vTaskDelay(pdMS_TO_TICKS(10));
        // }
        // for(int i = 100; i >= 0; i--)
        // {

        //     set_servo_duty( ( i / 100.0 ) * ( upper_bound - lower_bound ) + lower_bound);
        //     vTaskDelay(pdMS_TO_TICKS(10));
        // }
    }
    return;
}

float servo_control(float setpoint, float measured_value, float dt)
{
    const float Kp = 0.1;
    const float Ki = 0.01;
    const float Kd = 0.01;

    static float previous_error = 0.0;
    static float integral = 0.0;

    // Calculate error
    float error = setpoint - measured_value;

    // Proportional term
    float Pout = Kp * error;

    // Integral term
    integral += error * dt;
    float Iout = Ki * integral;

    // Derivative term
    float derivative = (error - previous_error) / dt;
    float Dout = Kd * derivative;

    // Calculate total output
    float output = Pout + Iout + Dout;

    // Update previous error
    previous_error = error;

    return output;
}

#define binary_size 17
void print_binary(uint32_t value)
{
    const char *TAG = "app_main::print_binary";
    char binary_representation[binary_size] = "";
    for (int i = 0; i < binary_size - 1; i++)
    {
        binary_representation[i] = value & (1 << (binary_size - 2 - i)) ? '1' : '0';
    }
    binary_representation[binary_size - 1] = '\0';
    ESP_LOGI(TAG, "%s", binary_representation);
    return;
}
#pragma once
#define MOTOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"

#include "esp_log.h"

/**
 * @brief Drives motor control system in forward direction.
 * @param duty_cycle The duty cycle for the motor control signal (0-100%).
 * @details This function sets the duty cycle for the motor control signal to drive
 * the motor in the forward direction. The duty cycle determines the speed of the motor.
 * A higher duty cycle results in a faster motor speed, while a lower duty cycle results
 * in a slower speed. The function uses the MCPWM (Motor Control Pulse Width Modulation)
 * module to generate the PWM signal for the motor control.
 */
void forward_vel( float duty_cycle );


/**
 * @brief Drives motor control system in reverse direction.
 * @param duty_cycle The duty cycle for the motor control signal (0-100%).
 * @details This function sets the duty cycle for the motor control signal to drive
 * the motor in the reverse direction. The duty cycle determines the speed of the motor.
 * A higher duty cycle results in a faster motor speed, while a lower duty cycle results
 * in a slower speed. The function uses the MCPWM (Motor Control Pulse Width Modulation)
 * module to generate the PWM signal for the motor control.
 */
void reverse_vel( float duty_cycle );

/**
 * @brief Stops the motor control system.
 * @details This function stops the motor control system by setting the duty cycle
 * to zero. Additionally, sets lower GPIO pins of H-Bridge to HIGH to quickly
 * stop the motor.
 */
void stop_vel( void );
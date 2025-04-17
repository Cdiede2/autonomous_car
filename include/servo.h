#pragma once
#define SERVO_H



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"


/**
 * @brief Sets up the servo motor.
 * @param ledc_channel The LEDC channel to be used for the servo.
 * @details This function initializes the LEDC channel for the servo motor.
 * It configures the timer, frequency, and duty cycle for the servo motor.
 * The servo motor is typically controlled using PWM signals, and this function
 * sets up the necessary parameters for generating those signals.
 */
void set_servo_duty( float duty_cycle );



#pragma once
#define SETUP_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/adc.h"         // Used to read ADC values from camera
#include "driver/mcpwm.h"       // Used to drive Motor
#include "driver/ledc.h"        // Used to drive Servo

#include "esp_timer.h"          // Used to set Timer up for SI pulse
#include "esp_system.h"
#include "esp_log.h"

#include "camera.h"

// GPIO pin definitions
#define CLK_OUTP_PIN GPIO_NUM_2
#define SI_PULSE_PIN GPIO_NUM_11

/**
 * @brief Initializes the program's environment and hardware configurations.
 *
 * This function is typically called once at the beginning of the program's execution.
 * It is used to set up initial states, configure hardware peripherals, and prepare
 * the system for the main execution loop. Common tasks include initializing pins,
 * setting up communication protocols, and configuring timers.
 */
void setup();
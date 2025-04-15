#pragma once
#define SETUP_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_log.h"

#include "camera.h"

// GPIO pin definitions
#define CLK_OUTP_PIN GPIO_NUM_2
#define SI_PULSE_PIN GPIO_NUM_11

// Timer Configuration
#define TIMER_PERIOD 20 // Timer period in milliseconds

/**
 * @brief Initializes the program's environment and hardware configurations.
 *
 * This function is typically called once at the beginning of the program's execution.
 * It is used to set up initial states, configure hardware peripherals, and prepare
 * the system for the main execution loop. Common tasks include initializing pins,
 * setting up communication protocols, and configuring timers.
 */
void setup();
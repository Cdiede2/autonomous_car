#include "camera.h"

static uint32_t adc_value = 0; // Variable to store ADC value

void send_pulse()
{
    send_pulse_flag = true; // Set the pulse flag to true
    return;
}

void get_camera_adc()
{   
    adc_value = adc1_get_raw(ADC1_CHANNEL_6); // Read ADC value
    return;
}

uint32_t read_camera_adc()
{
    return adc_value; // Return the last measured ADC value
}
#include "camera.h"

static uint32_t adc_value = 0; // Variable to store ADC value

void send_pulse()
{
    send_pulse_flag = true; // Set the pulse flag to true
    return;
}

void get_camera_adc( size_t bit )
{   
    const char TAG[] = "get_camera_adc";
    adc_value =  adc1_get_raw(ADC1_CHANNEL_6); // Read ADC value
    ESP_LOGI(TAG, "[ %u ] == %i", bit, (adc_value < 1024)?1:0);
    return;
}

uint32_t read_camera_adc()
{
    return adc_value; // Return the last measured ADC value
}

void clear_camera_adc() {
    adc_value = 0;
}
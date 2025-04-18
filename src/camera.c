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
    // ESP_LOGI(TAG, ">> %u", bit);
    adc_value |= ( adc1_get_raw(ADC1_CHANNEL_6) < AD_THRESH )?( 1 << bit ):0;
    if( bit < 0 || bit > 31 ) {
        ESP_LOGE(TAG, "Bit outside 0 to 31 range: %u", bit);
    }

    // adc_value =  adc1_get_raw(ADC1_CHANNEL_6) ); // Read ADC value
    return;
}

uint32_t read_camera_adc()
{
    return adc_value; // Return the last measured ADC value
}

void clear_camera_adc() {
    adc_value = 0;
}
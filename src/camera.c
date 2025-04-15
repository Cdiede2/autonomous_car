#include "camera.h"


void send_pulse()
{
    const char *TAG = "SEND_PULSE";
    ESP_LOGI(TAG, "Sending pulse...");
    send_pulse_flag = true; // Set the pulse flag to true
    return;
}

int read_camera_adc()
{
    return adc_value;
}
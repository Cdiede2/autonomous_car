#include "servo.h"


void set_servo_duty( float duty_cycle )
{

    ledc_set_duty( LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_cycle * ( (1 << 13) - 1)); // Set duty cycle
    ledc_update_duty( LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0 ); // Update duty cycle
    return;
}
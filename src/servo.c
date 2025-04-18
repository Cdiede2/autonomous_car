#include "servo.h"

#define LOWER_BOUND 0.0310
#define UPPER_BOUND 0.0600
#define SERVO_FREQUENCY 50 // Frequency in Hz
void set_servo_duty( float duty_cycle )
{   
    duty_cycle = ( duty_cycle > UPPER_BOUND ) ? UPPER_BOUND : duty_cycle; // Limit duty cycle to upper bound
    duty_cycle = ( duty_cycle < LOWER_BOUND ) ? LOWER_BOUND : duty_cycle; // Limit duty cycle to lower bound

    ledc_set_duty( LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_cycle * ( (1 << 13) - 1)); // Set duty cycle
    ledc_update_duty( LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0 ); // Update duty cycle
    return;
}
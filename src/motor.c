#include "motor.h"
#include "setup.h"

/*
        ---------------- Vdd
        |                |
      [ M1 ]           [ M2 ]
        |                |
        +----- ( M ) ----+
        |                |
      [ M3 ]           [ M4 ]
        |                |
        ------------------ GND
*/

void forward_vel(float duty_cycle)
{
    duty_cycle = (duty_cycle > 100.0) ? 100.0 : duty_cycle; // Limit duty cycle to 100%
    duty_cycle = (duty_cycle < 0.0) ? 0.0 : duty_cycle;     // Limit duty cycle to 0%

    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, duty_cycle);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_B, 0.0);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);

    return;
}

void reverse_vel(float duty_cycle)
{
    // mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
    duty_cycle = (duty_cycle > 100.0) ? 100.0 : duty_cycle; // Limit duty cycle to 100%
    duty_cycle = (duty_cycle < 0.0) ? 0.0 : duty_cycle;     // Limit duty cycle to 0%

    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, 0.0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_B, duty_cycle);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);

    return;
}

void stop_vel(void)
{
    // mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);

    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, 0.0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_B, 0.0);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);

    return;
}
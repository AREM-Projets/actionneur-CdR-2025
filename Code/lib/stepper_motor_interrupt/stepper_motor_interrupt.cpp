#include "stepper_motor_interrupt.hpp"

volatile uint32_t pwm_isr_counter[4] = {0, 0, 0, 0};
StepperMotor_t *stepper_isr_inst[4] = {nullptr, nullptr, nullptr, nullptr};
volatile bool pwm_counter_taken[4] = {false, false, false, false};

void PWMcounterISR_function(uint32_t instance)
{
    pwm_isr_counter[instance]++;
    if((stepper_isr_inst[instance]->nbOfStepToDo) <= pwm_isr_counter[instance])
    {
        stepper_isr_inst[instance]->stop();
        stepper_isr_inst[instance]->detachInterruptPWM();
    }
}

void PWMcounterISR_callback_motor0()
{
    PWMcounterISR_function(0);
}

void PWMcounterISR_callback_motor1()
{
    PWMcounterISR_function(1);
}

void PWMcounterISR_callback_motor2()
{
    PWMcounterISR_function(2);
}

void PWMcounterISR_callback_motor3()
{
    PWMcounterISR_function(3);
}

void (*PWMcounterISR_callback[4])(void) = {
    PWMcounterISR_callback_motor0,
    PWMcounterISR_callback_motor1,
    PWMcounterISR_callback_motor2,
    PWMcounterISR_callback_motor3
};

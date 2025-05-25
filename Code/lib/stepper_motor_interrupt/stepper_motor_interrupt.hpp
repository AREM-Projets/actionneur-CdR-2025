#pragma once

// Author : Lïam LOTTE - EI22
// Creation Date : 23/02/2025

#include <Arduino.h>
#include "stepper_motor.hpp"

extern volatile uint32_t pwm_isr_counter[4];
extern StepperMotor_t* stepper_isr_inst[4];
extern volatile bool pwm_counter_taken[4];

/// Interrupt function that is trigger at the rising edge of a PWM signal 
/// that is send into the stepper motor driver.
void PWMcounterISR_callback_motor0();
void PWMcounterISR_callback_motor1();
void PWMcounterISR_callback_motor2();
void PWMcounterISR_callback_motor3();

extern void (*PWMcounterISR_callback[4])(void); 
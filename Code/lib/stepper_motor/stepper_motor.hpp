#pragma once

// Author : Lïam LOTTE - EI22
// Creation Date : 23/02/2025

// Forward declaration (avoids circular dependency)
class StepperMotor_t;

#include <Arduino.h>
#include "stepper_motor_interrupt.hpp"
#include "math.h"

#define STEPPER_DUTY_CYCLE 1 //percent

/// Stepper Motor class that allow to pilot stepper motors thanks to the
/// A4988 componant from Sparkfun in a STM32 environment.
class StepperMotor_t
{
private:
    // Mecanical Data
    uint32_t stepsPerRev;
    float anglePerStepDeg;
    float anglePerStepRad;

    // Electrical Data
    uint32_t dirPin;
    uint32_t stepPin;
    uint32_t pwmChannel;
    HardwareTimer *timer;

    // Rotation data
    // ! The volatile keyword allow to the ISR functions to modify the value of variables here !
    volatile bool isRotating;
    volatile uint32_t nbOfStepToDo;

    // ISR logic data
    uint32_t counter_isr_instance;

    /* ---------------------- ISR related methods ---------------------- */
    // We don't want to give the access of it at everyone
    bool attachInterruptPWM(uint32_t counter_isr_instance_to_set);
    bool attachInterruptPWM();
    void detachInterruptPWM();

public:
    /* ---------------------- Constructor and destructor ---------------------- */

    // To use a custom channel and a custom timer
    StepperMotor_t(uint32_t stepsPerRev, uint32_t dirPin, uint32_t stepPin, uint32_t pwmChannel, HardwareTimer &timer);
    // To use automatically a timer and a channel available thanks to ST functions
    StepperMotor_t(uint32_t stepsPerRev, uint32_t dirPin, uint32_t stepPin);
    ~StepperMotor_t();

    /* ---------------------- Methods ---------------------- */
    
    void setCounterInstance(uint32_t counter_instance);
    void setSpeed(float rotPerSec);
    void setDirection(bool direction);
    void changeDirection();
    void stop();
    void setPositionAndWait(uint32_t nbOfStep, float rotPerSec, uint32_t counter_instance);
    void setPosition(uint32_t nbOfStep, float rotPerSec, uint32_t counter_instance);

    /* ---------------------- PWM ISR Friend function ---------------------- */
    friend void PWMcounterISR_function(uint32_t instance);
    
};

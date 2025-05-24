#pragma once

// Author : Lïam LOTTE - EI22
// Creation Date : 24/05/2025

#include <Arduino.h>

class Servo_pwm
{
private:
    // Electrical Data
    uint32_t pwm_pin;
    uint32_t pwmChannel;
    HardwareTimer *timer;

public:
    // To use automatically a timer and a channel available thanks to ST functions
    Servo_pwm(uint32_t new_pwm_pin);

    // To use a custom channel and a custom timer
    Servo_pwm(uint32_t new_pwm_pin, uint32_t new_pwm_channel, HardwareTimer &new_timer);

    ~Servo_pwm();

    void writeMicroseconds(uint16_t us);
};

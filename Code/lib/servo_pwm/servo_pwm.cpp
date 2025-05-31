#include "servo_pwm.hpp"

// Constructor with automatic timer/channel assignment
ServoPwm::ServoPwm(uint32_t new_pwm_pin, int value = 0) :
    pwm_pin(new_pwm_pin),
    timer(nullptr),
    pwmChannel(1) // Default channel if not specified
{
    // Retrieve TIM instance and channel associated to pin
    TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(new_pwm_pin), PinMap_PWM);
    uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(new_pwm_pin), PinMap_PWM));

    // Instantiate and store timer globally
    this->timer = new HardwareTimer(Instance);
    this->pwmChannel = channel;

    // 50Hz for servo, default pulse width 1500us
    timer->setPWM(pwmChannel, pwm_pin, 50, 1500); 
}

// Constructor with custom timer and channel
ServoPwm::ServoPwm(uint32_t new_pwm_pin, uint32_t new_pwm_channel, HardwareTimer &new_timer, int value = 0) :
    pwm_pin(new_pwm_pin),
    pwmChannel(new_pwm_channel),
    timer(&new_timer)
{
    // 50Hz for servo, default pulse width 1500us
    timer->setPWM(pwmChannel, pwm_pin, 50, 1500);
}

// Destructor
ServoPwm::~ServoPwm()
{
    if (timer) {
        timer->pauseChannel(pwmChannel);
        delete timer; // Only delete if it was allocated internally
    }
}

// Method to control servo by angle (0 to 180 degrees)
void ServoPwm::write(int value)
{
    if (value < 0) value = 0;
    if (value > 180) value = 180;

    // Convert angle to microseconds (standard range: 1000µs to 2000µs)
    uint16_t us = map(value, 0, 180, 1000, 2000);

    this->writeMicroseconds(us);
}


// Method to control servo by pulse width
void ServoPwm::writeMicroseconds(uint16_t us)
{
    if (timer) {
        timer->setCaptureCompare(pwmChannel, us, MICROSEC_COMPARE_FORMAT);
    }
}

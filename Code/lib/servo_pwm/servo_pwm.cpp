#include "servo_pwm.hpp"

// Constructor with automatic timer/channel assignment
Servo_pwm::Servo_pwm(uint32_t new_pwm_pin) :
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
Servo_pwm::Servo_pwm(uint32_t new_pwm_pin, uint32_t new_pwm_channel, HardwareTimer &new_timer) :
    pwm_pin(new_pwm_pin),
    pwmChannel(new_pwm_channel),
    timer(&new_timer)
{
    // 50Hz for servo, default pulse width 1500us
    timer->setPWM(pwmChannel, pwm_pin, 50, 1500);
}

// Destructor
Servo_pwm::~Servo_pwm()
{
    if (timer) {
        timer->pauseChannel(pwmChannel);
        delete timer; // Only delete if it was allocated internally
    }
}

// Method to control servo by pulse width
void Servo_pwm::writeMicroseconds(uint16_t us)
{
    if (timer) {
        timer->setCaptureCompare(pwmChannel, us, MICROSEC_COMPARE_FORMAT);
    }
}

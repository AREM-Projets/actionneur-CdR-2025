#include "stepper_motor.h"

/* ---------------------- Constructor and destructor ---------------------- */

StepperMotor_t::StepperMotor_t(uint32_t stepsPerRev, uint32_t dirPin, uint32_t stepPin, uint32_t pwmChannel, HardwareTimer &timer)
    : stepsPerRev(stepsPerRev), dirPin(dirPin), stepPin(stepPin), pwmChannel(pwmChannel), timer(&timer)
{
    // Mecanical part
    this->stepsPerRev = stepsPerRev;
    this->anglePerStepDeg = 360.0f / stepsPerRev;
    this->anglePerStepRad = this->anglePerStepDeg * (M_PI / 180.0f);

    //Electrical part
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
}

StepperMotor_t::StepperMotor_t(uint32_t stepsPerRev, uint32_t dirPin, uint32_t stepPin)
{
    // Mecanical part
    this->stepsPerRev = stepsPerRev;
    this->anglePerStepDeg = 360.0f / stepsPerRev;
    this->anglePerStepRad = this->anglePerStepDeg * (M_PI / 180.0f);

    // Electrical part
    this->dirPin = dirPin;
    this->stepPin = stepPin;
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);

    // Retrieve TIM instance and channel associated to pin
    TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(stepPin), PinMap_PWM);
    uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(stepPin), PinMap_PWM));

    // Instantiate and store timer globally
    this->timer = new HardwareTimer(Instance);
    this->pwmChannel = channel;
}

StepperMotor_t::~StepperMotor_t()
{
    if (timer)
    {
        delete timer;
        timer = nullptr;
    }
}

/* ---------------------- Methods ---------------------- */

void StepperMotor_t::setSpeed(float rotPerSec)
{
    // Rotation related data
    this->isRotating = true;

    // PWM setup
    uint32_t pwmFrequency = rotPerSec * stepsPerRev;
    timer->setPWM(pwmChannel, stepPin, pwmFrequency, STEPPER_DUTY_CYCLE);
    timer -> resumeChannel(pwmChannel);
}

void StepperMotor_t::setDirection(bool direction)
{
    digitalWrite(dirPin, direction);
}

void StepperMotor_t::changeDirection()
{
    digitalWrite(dirPin, !digitalRead(dirPin));
}

void StepperMotor_t::stop()
{
    // Rotation related data
    this->isRotating = false;
    this->nbOfStepToDo = 0;

    // Stop the STM32 channel in a clean way
    timer -> pauseChannel(pwmChannel);
}

bool StepperMotor_t::attachInterruptPWM(uint32_t counter_isr_instance_to_set)
{
    bool result = true;
    counter_isr_instance = counter_isr_instance_to_set;

    if (!pwm_counter_taken[counter_isr_instance_to_set])
    {
        // Rotation related data
        this->isRotating = true;

        // ISR related data init
        stepper_isr_inst[counter_isr_instance] = this;
        pwm_isr_counter[counter_isr_instance]= 0;
        pwm_counter_taken[counter_isr_instance] = true;

        // Attach the function choose by user;
        std::function<void(void)> counter = (PWMcounterISR_callback[counter_isr_instance]);
        timer->attachInterrupt(counter);
    }
    else
    {
        // Mean that the counter is already assign
        result = false;
    }
    
    return result;
}

bool StepperMotor_t::attachInterruptPWM()
{
    bool result = this->attachInterruptPWM(counter_isr_instance);
    return result;
}


void StepperMotor_t::detachInterruptPWM()
{
    // ISR related data
    stepper_isr_inst[counter_isr_instance] = nullptr;
    pwm_isr_counter[counter_isr_instance]= 0; // in theory useless but we never know
    pwm_counter_taken[counter_isr_instance] = false;

    timer->detachInterrupt();
    while(timer->hasInterrupt() == true);
}

void StepperMotor_t::setPositionAndWait(uint32_t nbOfStep, float rotPerSec, uint32_t counter_instance)
{
   this->setPosition(nbOfStep, rotPerSec, counter_instance);

    // The motor is stop by interruption
    while(isRotating == true);
}

void StepperMotor_t::setPosition(uint32_t nbOfStep, float rotPerSec, uint32_t counter_instance)
{
    //ISR data init
    this->nbOfStepToDo = nbOfStep;

    // motor start
    this->attachInterruptPWM(counter_instance);
    this->setSpeed(rotPerSec);
}


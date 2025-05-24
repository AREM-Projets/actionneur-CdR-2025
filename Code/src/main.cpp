// Author : Lïam LOTTE - EI22
// Creation Date : 23/02/2025

#include <Arduino.h>
#include "stepper_motor.hpp"
#include "test_bench.hpp"
#include "servo_pwm.hpp"

#define NUMBER_OF_STEP_TO_DO 1200
#define MOTOR_SPEED 0.5
#define COUNTER_INSTANCE 1

// Create the uart interface instance
//                      RX    TX
HardwareSerial Serial1(PA3, PA2);

// Create a stepper motor instance
HardwareTimer timer16(TIM16);  
StepperMotor_t motor(200, PB5, PB6, 1, timer16);

void setup()
{
  // Use to trigger the test_bench for testing subfunctions and testing the hardware
  if (TEST_BENCH_ENABLE == 1)
  {
    test_bench();
  }

  // Initialize Serial for debugging
  Serial.begin(9600);
  delay(5000);

  // Initialize servos


}

void loop()
{
  motor.setPositionAndWait(NUMBER_OF_STEP_TO_DO, MOTOR_SPEED, COUNTER_INSTANCE);
  motor.changeDirection();
  delay(2000);
}

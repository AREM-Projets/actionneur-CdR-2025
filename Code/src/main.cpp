// Author : Lïam LOTTE - EI22
// Creation Date : 23/02/2025

#include <Arduino.h>
#include <Servo.h>
#include "stepper_motor.h"
#include "test_bench.h"

#define NUMBER_OF_STEP_TO_DO 1200
#define MOTOR_SPEED 0.5
#define COUNTER_INSTANCE 1

// Create the uart interface instance
//                      RX    TX
HardwareSerial Serial1(PA3, PA2);

// Create a stepper motor instance
HardwareTimer timer1(TIM1);  
StepperMotor_t motor(200, D10, D9, 1, timer1);

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

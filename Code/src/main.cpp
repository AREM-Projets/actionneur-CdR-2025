// Author : Lïam LOTTE - EI22
// Creation Date : 23/02/2025

#include <Arduino.h>
#include "stepper_motor.hpp"
#include "test_bench.hpp"
#include "servo_pwm.hpp"

// Stepper motor constants
#define NUMBER_OF_STEP_TO_DO 1200
#define MOTOR_SPEED 2
#define COUNTER_INSTANCE 1
HardwareTimer timer1(TIM1);
StepperMotor_t stepper_motor(200, D10, D9, 1, timer1);

// Servo positions
#define CLOSED_POSITION 0
#define OPEN_POSITION 140
ServoPwm servo_grippers(A5);

#define PUSHED_POSITION 30
#define IDLE_POSITION 140
ServoPwm servo_push(A2);

// Action states
enum ACTION_STATE
{
  IDLE_DOWN,
  CLOSE_GRIPPERS,
  GO_UP,
  IDLE_UP,
  GO_DOWN,
  OPEN_GRIPPERS,
};

ACTION_STATE current_state = IDLE_DOWN;
ACTION_STATE next_state = IDLE_DOWN;

// Read UART safely
char read_uart()
{
  if (Serial.available())
  {
    char input_char = Serial.read();
    return input_char;
  }
  return '\0'; // No input
}

// Finite State Machine transitions
void FSM_transition(char input_char)
{
  current_state = next_state;

  switch (current_state)
  {
  case IDLE_DOWN:
    if (input_char == 'u')
    {
      next_state = CLOSE_GRIPPERS;
    }
    else
    {
      next_state = IDLE_DOWN;
    }
    break;

  case CLOSE_GRIPPERS:
    next_state = GO_UP;
    break;

  case GO_UP:
    next_state = IDLE_UP;
    break;

  case IDLE_UP:
    if (input_char == 'd')
    {
      next_state = GO_DOWN;
    }
    else
    {
      next_state = IDLE_UP;
    }
    break;

  case GO_DOWN:
    next_state = OPEN_GRIPPERS;
    break;

  case OPEN_GRIPPERS:
    next_state = IDLE_DOWN;
    break;
  }
}

// FSM actions based on state
void FSM_state()
{
  switch (current_state)
  {
  case IDLE_DOWN:
    // Do nothing
    break;

  case CLOSE_GRIPPERS:
    servo_grippers.write(CLOSED_POSITION);
    delay(2000);
    break;

  case GO_UP:
    stepper_motor.setDirection(0);
    stepper_motor.setPositionAndWait(NUMBER_OF_STEP_TO_DO, MOTOR_SPEED, COUNTER_INSTANCE);
    break;

  case IDLE_UP:
    // Do nothing
    break;

  case GO_DOWN:
    stepper_motor.setDirection(1);
    stepper_motor.setPositionAndWait(NUMBER_OF_STEP_TO_DO, MOTOR_SPEED, COUNTER_INSTANCE);
    break;

  case OPEN_GRIPPERS:
    delay(2000);
    servo_grippers.write(OPEN_POSITION);
    break;
  }
}

void setup()
{
  // Test bench setup
  if (TEST_BENCH_ENABLE == 1)
  {
    test_bench();
  }

  // Initialize serial
  Serial.begin(9600);

  // Set servos to initial position
  servo_grippers.write(CLOSED_POSITION);
  delay(1000);
  servo_grippers.write(OPEN_POSITION);

  servo_push.write(PUSHED_POSITION);
  delay(1000);
  servo_push.write(IDLE_POSITION);

  // Send message to raspy
  delay(1000);
  Serial.println("Initialized");
}

void loop()
{
  char char_from_raspberry = read_uart();
  FSM_transition(char_from_raspberry);
  Serial.println((int)current_state);
  FSM_state();
}

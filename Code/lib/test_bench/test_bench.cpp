#include "test_bench.hpp"

#include "stepper_motor.hpp"
#include "servo_pwm.hpp"

// Create a stepper motor instance
HardwareTimer timer16_test(TIM16);  
StepperMotor_t motor_test(200, D9, D10, 1, timer16_test);

// Create 2 servo motors instances
ServoPwm servo1_test(A5);
ServoPwm servo2_test(A2);

void test_bench()
{
    bench_setup();
    while(1)
    {
        bench_loop();
    }
}

void bench_setup()
{
    motor_test.setSpeed(0.5);
}

void bench_loop()
{

}
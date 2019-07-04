#include <Arduino.h>
#include <PPG.h>

uint32_t	motor_speed[2];
uint32_t	motor_deadtime = 1;
uint32_t	motor_pwm_cycle = 500;	// 500[micro-second]
void    calc_speed();

void    setup()
{
  PPG.begin(true, true, true, true, motor_pwm_cycle, 0b0000);
  PPG.setTrigger(0, motor_deadtime, motor_pwm_cycle / 2 - motor_deadtime);
  PPG.setTrigger(1, motor_pwm_cycle / 2 + motor_deadtime, motor_pwm_cycle - motor_deadtime);
  PPG.setTrigger(2, motor_deadtime, motor_pwm_cycle / 2 - motor_deadtime);
  PPG.setTrigger(3, motor_pwm_cycle / 2 + motor_deadtime, motor_pwm_cycle - motor_deadtime);
  PPG.start();
}

void	loop()
{
  calc_speed();
  PPG.setTrigger(0, motor_deadtime, motor_speed[0] - motor_deadtime);
  PPG.setTrigger(1, motor_speed[0] + motor_deadtime, motor_pwm_cycle - motor_deadtime);
  PPG.setTrigger(2, motor_deadtime, motor_speed[1] - motor_deadtime);
  PPG.setTrigger(3, motor_speed[1] + motor_deadtime, motor_pwm_cycle - motor_deadtime);
  PPG.enableTrigger();
  delay(100);
}

void    calc_speed()
{
  // Set speed according to A0, A1
  uint32_t	motor_range;

  motor_range = motor_pwm_cycle - motor_deadtime * 2U;
  motor_speed[0] = map(analogRead(A0), 0, (1U<<12), 0, motor_range);
  motor_speed[1] = map(analogRead(A1), 0, (1U<<12), 0, motor_range);
}


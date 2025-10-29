#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"

void motor_pwm();
void pid_control();
void motor_config(enum MOTOR_CONFIG config);
void brake();
void set_rev_target(uint32_t revs);
_Bool rev_target_reached();

#endif // MOTOR_H

#include "motor.h"

void motor_pwm(){
    static uint8_t pwmCnt = 0;
    if(pwmCnt >= 255) pwmCnt = 0;
    // left
    if(pwmCnt > left_duty_cycle){
        JC &= ~(1 << L_PWM_OFFSET);
    }
    else{
        JC |= (1 << L_PWM_OFFSET);
    }
    // right
    if(pwmCnt > right_duty_cycle){
        JC &= ~(1 << R_PWM_OFFSET);
    }
    else{
        JC |= (1 << R_PWM_OFFSET);
    }
    pwmCnt++;
}

void pid_control(){
    int32_t enc_diff = (int32_t)(left_encoder_cnt - right_encoder_cnt);
    if(enc_diff > REV_DIFF_TOP){
        left_duty_cycle = LOW_SPEED;
        right_duty_cycle = MED_SPEED;
    }
    else if(enc_diff < -REV_DIFF_TOP){
        left_duty_cycle = MED_SPEED;
        right_duty_cycle = LOW_SPEED;
    }
    else{
        left_duty_cycle = MED_SPEED;
        right_duty_cycle = MED_SPEED;
    }
}

void motor_config(enum MOTOR_CONFIG config) {
    JC &= ~((1 << LEFT1_OFFSET) | (1 << LEFT2_OFFSET) | (1 << RIGHT1_OFFSET) | (1 << RIGHT2_OFFSET));
    switch (config) {
        case FORWARD:
            JC |= (1 << LEFT2_OFFSET) | (1 << RIGHT2_OFFSET);
            break;
        case BACKWARD:
            JC |= (1 << LEFT1_OFFSET) | (1 << RIGHT1_OFFSET);
            break;
        case LEFT:
            JC |= (1 << LEFT2_OFFSET) | (1 << RIGHT1_OFFSET);
            break;
        case RIGHT:
            JC |= (1 << LEFT1_OFFSET) | (1 << RIGHT2_OFFSET);
            break;
        case SHORT_BRAKE:
            JC |= (1 << LEFT1_OFFSET) | (1 << LEFT2_OFFSET) | (1 << RIGHT1_OFFSET) | (1 << RIGHT2_OFFSET);
            break;
        case LONG_BRAKE:
            // already cleared
            break;
    }
}

void brake(){
    // hard brake
    motor_config(LONG_BRAKE);
    timer_2us(TIMER_1S);
    // turn off motors
    left_duty_cycle = 0;
    right_duty_cycle = 0;    
}

void set_rev_target(uint32_t revs){
    left_encoder_cnt = 0;
    right_encoder_cnt = 0;
    rev_target = revs;
}

_Bool rev_target_reached(){
    return (left_encoder_cnt >= rev_target) || (right_encoder_cnt >= rev_target);
}

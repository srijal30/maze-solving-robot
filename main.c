#include "hardware.h"
#include "timers.h"
#include "us_sensors.h"
#include "motor.h"
#include "util.h"


// Global Variables
const uint8_t sseg_lut[16] = {
    0xC0, 0xF9, 0xA4, 0xB0, 
    0x99, 0x92, 0x82, 0xF8, 
    0x80, 0x98, 0x88, 0x83, 
    0xC6, 0xA1, 0x86, 0x8E
};
uint8_t sseg_values[4] = {0xB, 0xE, 0xE, 0xF};

uint32_t left_encoder_cnt;
uint32_t right_encoder_cnt;
uint32_t rev_target;

uint8_t right_duty_cycle;
uint8_t left_duty_cycle;

uint32_t last_distance_front;
uint32_t distance_front;
uint32_t distance_left;

uint8_t obstacle_cnt;

enum DRIVE_STATE current_drive_state;
enum DRIVE_STATE next_drive_state;


// Runs at initialization
void setup() {
    // initialize values
    left_encoder_cnt = 0;
    right_encoder_cnt = 0;
    right_duty_cycle = 0;
    left_duty_cycle = 0;

    obstacle_cnt = 0;

    // setup first motor state
    next_drive_state = START;

    // setup start switch
    start_switch_toggled();

    // setup timers
    configure_timers();

    // DDR inputs
    JA_DDR = 0xff;
    JB_DDR |= (1 << F_ECHO_OFFSET);
    JB_DDR |= (1 << L_ECHO_OFFSET);

    // DDR outputs
    JC_DDR = 0;
    JB_DDR &= ~(1 << F_TRIG_OFFSET);
    JB_DDR &= ~(1 << L_TRIG_OFFSET);

    // set the inital distances
    update_distance_front(true);
    update_distance_left(true);
}

// Loops infinitely
void loop(void){
    // update encoder values
    if(read_left_enc()){
        left_encoder_cnt++;
    }
    if(read_right_enc()){
        right_encoder_cnt++;
    }

    // update motor
    motor_pwm();

    // state machine
    current_drive_state = next_drive_state;
    LEDS = current_drive_state;
    switch(current_drive_state){
        case START:
            if(start_switch_toggled()){
                timer_2us(TIMER_1S*2); // delay 1s
                next_drive_state = DRIVE_FORWARD;
                motor_config(FORWARD);
                update_distance_front(true); // make sure first distance is accurate
                update_distance_left(true);
            }
            else{
                next_drive_state = START;
            }
        break;
        
        case DRIVE_FORWARD:
            pid_control();
            update_distance_front(false);
            update_distance_left(false);
            // if obstacle in front, turn right
            if(distance_front <= FRONT_BLOCKED_D){
                brake();
                next_drive_state = TURN_RIGHT;
                obstacle_cnt++;
                motor_config(RIGHT);
                set_rev_target(REV_TURN_RIGHT);
            }
            // if we need to correct left wall alignment
            // we are going too far
            else if(distance_left >= (FOLLOW_DIST+FOLLOW_DIST_RANGE)){
                next_drive_state = TURN;
                motor_config(LEFT);
                set_rev_target(REV_INCH*.5);
            }
            // we are getting too close
            else if(distance_left <= (FOLLOW_DIST-FOLLOW_DIST_RANGE)){
                next_drive_state = TURN;
                motor_config(RIGHT);
                set_rev_target(REV_INCH*.5);
            }
            // just keep going straight
            else{
                next_drive_state = DRIVE_FORWARD;
            }
        break;

        case TURN_RIGHT:
            pid_control();
            // once turn complete
            if(rev_target_reached()){
                brake();
                // check if we are done (if still obstacle)
                update_distance_front(true);
                if(distance_front <= END_CHECK_D){
                    next_drive_state = DONE;
                }
                // else proceed to drive forward
                else{
                    next_drive_state = DRIVE_FORWARD;
                    motor_config(FORWARD);
                    update_distance_front(true); // make sure first distance is accurate
                    update_distance_left(true);
                }
            }
            else{
                next_drive_state = TURN_RIGHT;
            }
        break;

        case TURN:
            pid_control();
            if(rev_target_reached()){
                next_drive_state = DRIVE_FORWARD;
                motor_config(FORWARD);
                update_distance_front(true); // make sure first distance is accurate
                update_distance_left(true);
            }
            else{
                next_drive_state = TURN;
            }
        break;

        case DONE:
            LEDS = 0xffff; // celebration
            left_duty_cycle = 0;
            right_duty_cycle = 0;
            next_drive_state = DONE;
            if(start_switch_toggled()){
                left_encoder_cnt = 0;
                right_encoder_cnt = 0;
                obstacle_cnt = 0;
                next_drive_state = START;
            }
            else{
                next_drive_state = DONE;
            }
        break;
    }

    // update sseg
    if(current_drive_state == DONE){
        sseg_values[0] = obstacle_cnt;
        sseg_values[1] = 0;
        sseg_values[2] = 0;
        sseg_values[3] = 0;
        show_sseg();
    }
    else if(current_drive_state == START){
        sseg_values[0] = 0xf;
        sseg_values[1] = 0xe;
        sseg_values[2] = 0xe;
        sseg_values[3] = 0xb; 
        show_sseg();  
    }
    else if(current_drive_state == DRIVE_FORWARD){
        sseg_values[0] = distance_front % 10;
        sseg_values[1] = (distance_front / 10) % 10;
        sseg_values[2] = (distance_front / 100) % 10;
        sseg_values[3] = (distance_front / 1000) % 10;
        show_sseg();
    }
    else{
        ANODES = 0xffff;
    }
}

// Entrypoint of program
int main(void){
    setup();    
    while(1){
        loop();
    }
    return 0;
}

#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include <stdbool.h>

// Memory-Mapped IO Definitions
#define BUTTONS        (*(volatile unsigned *) 0x40000000) // 4-pin Input
#define JA             (*(volatile unsigned *) 0x40001000) // 8-pin In/Out
#define JA_DDR         (*(volatile unsigned *) 0x40001004) // 8-pin DDR
#define JB             (*(volatile unsigned *) 0x40002000) // 8-pin In/Out
#define JB_DDR         (*(volatile unsigned *) 0x40002004) // 8-pin DDR
#define JC             (*(volatile unsigned *) 0x40003000) // 8-pin In/Out
#define JC_DDR         (*(volatile unsigned *) 0x40003004) // 8-pin DDR
#define JXADC          (*(volatile unsigned *) 0x40004000) // 8-pin In/Out
#define JXADC_DDR      (*(volatile unsigned *) 0x40004004) // 8-pin DDR
#define LEDS           (*(volatile unsigned *) 0x40005000) // 16-pin Output
#define ANODES         (*(volatile unsigned *) 0x40006000) // 4-pin Output
#define SEVEN_SEG      (*(volatile unsigned *) 0x40006008) // 8-pin Output
#define SWITCHES       (*(volatile unsigned *) 0x40007000) // 16-pin Input
#define UART           (*(volatile unsigned *) 0x40008000) // Out of Scope
#define TIMER_BASE     ((uint32_t *) 0x40009000) // Timer Base Address

// US Sensors
#define F_TRIG_OFFSET       0 // JB[0]
#define F_ECHO_OFFSET       1 // JB[1]
#define L_TRIG_OFFSET       2 // JB[2]
#define L_ECHO_OFFSET       3 // JB[3]

// Motor Control
#define L_PWM_OFFSET        0 // JC[0]
#define LEFT2_OFFSET        1 // JC[1]
#define LEFT1_OFFSET        2 // JC[2]
#define R_PWM_OFFSET        3 // JC[3]
#define RIGHT2_OFFSET       4 // JC[4]
#define RIGHT1_OFFSET       5 // JC[5]

// Quadrature Encoder
#define L_ENC_OFFSET        0 // JA[0]
#define R_ENC_OFFSET        1 // JA[1]

// Timing Offsets
#define TCSR_OFFSET         0
#define TLR_OFFSET          1
#define TCR_OFFSET          2

// Constant Values
// timing
#define CONSTANT_FACTOR     2 // helps adjust for off timing
#define STOPWATCH_1S        565004
#define STOPWATCH_1MS       570
#define TIMER_10US          5
#define TIMER_1S            500000
#define US_TIME_FOR_1M      6800
#define US_UPDATE_PERIOD    100*STOPWATCH_1MS*CONSTANT_FACTOR // 10 times a second
// motor control            
#define HIGH_SPEED          0xff
#define MED_SPEED           0xee
#define LOW_SPEED           0xcc
#define REV_DIFF_TOP        5
#define REV_TURN_RIGHT      250
#define REV_TURN_LEFT       250
#define REV_INCH            47.5
// distance tolerance
#define FOLLOW_DIST         100
#define FOLLOW_DIST_RANGE   50
#define FRONT_BLOCKED_D     100
#define LEFT_OPEN_D         200
#define END_CHECK_D         250

// Enum Declarations
enum MOTOR_CONFIG {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    LONG_BRAKE,
    SHORT_BRAKE
};

enum DRIVE_STATE {
    START,
    DRIVE_FORWARD,
    TURN_RIGHT,
    TURN,
    DONE
};

enum STOPWATCH {
    FILLER,
    SSEG_STOPWATCH,
    US_STOPWATCH,
    DIST_L_STOPWATCH,
    DIST_F_STOPWATCH
};

// Extern global declarations (defined in main.c)
extern const uint8_t sseg_lut[16];
extern uint8_t sseg_values[4];

extern uint32_t left_encoder_cnt;
extern uint32_t right_encoder_cnt;
extern uint32_t rev_target;

extern uint8_t right_duty_cycle;
extern uint8_t left_duty_cycle;

extern uint32_t last_distance_front;
extern uint32_t distance_front;
extern uint32_t distance_left;

extern uint8_t obstacle_cnt;

extern enum DRIVE_STATE current_drive_state;
extern enum DRIVE_STATE next_drive_state;

#endif // HARDWARE_H

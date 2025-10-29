#include "us_sensors.h"

void set_trig_pin(uint8_t offset){
    JB |= (1<<offset);
}

void clear_trig_pin(uint8_t offset){
    JB &= ~(1<<offset);
}

_Bool read_echo_pin(uint8_t offset){
    return (JB & (1<<offset)) == (1<<offset);
}

// clean up this function
uint32_t update_distance(uint8_t trig, uint8_t echo){
    // send trig pulse
    set_trig_pin(trig);
    timer_2us(TIMER_10US*CONSTANT_FACTOR);
    clear_trig_pin(trig);
    // wait for echo response
    start_stopwatch(US_STOPWATCH);
    while(!read_echo_pin(echo)){
        // timeout if it has been 1ms
        if(read_stopwatch(US_STOPWATCH) > STOPWATCH_1MS*5){
            // if we dont receive echo, assume obstacle far
            return 1000;
        }
    }
    // count echo pin length
    start_stopwatch(US_STOPWATCH);
    while(read_echo_pin(echo)){
        // add timeout for large distances
        if(read_stopwatch(US_STOPWATCH) >  (US_TIME_FOR_1M/2)){
            return 500;
        }
    }
    // find distance in cm and return
    uint32_t ticks = read_stopwatch(US_STOPWATCH);
    uint32_t new_distance = (uint32_t)(ticks / (double) (STOPWATCH_1S*CONSTANT_FACTOR) * 340 / 2 * 1000);
    return new_distance;
}

void update_distance_left(_Bool force){
    _Bool can_read = read_stopwatch(DIST_L_STOPWATCH) > US_UPDATE_PERIOD;
    if(force){
        while(!can_read){
            can_read = read_stopwatch(DIST_L_STOPWATCH) > US_UPDATE_PERIOD;
        }
    }
    if(can_read){
        distance_left = update_distance(L_TRIG_OFFSET, L_ECHO_OFFSET);
        start_stopwatch(DIST_L_STOPWATCH);
    }
}

void update_distance_front(_Bool force){
    _Bool can_read = read_stopwatch(DIST_F_STOPWATCH) > US_UPDATE_PERIOD;
    if(force){
        while(!can_read){
            can_read = read_stopwatch(DIST_F_STOPWATCH) > US_UPDATE_PERIOD;
        }
    }
    if(can_read){
        distance_front = update_distance(F_TRIG_OFFSET, F_ECHO_OFFSET);
        start_stopwatch(DIST_F_STOPWATCH);
    }
}

#include "util.h"

void show_sseg() {
    // convert from lookup table
    uint8_t sseg_out[4];
    for(int i = 0; i < 4; i++){
        sseg_out[i] = sseg_lut[sseg_values[i]];
    };
    // display in 60hz
    static uint8_t anodeCnt = 0;
    if (read_stopwatch(SSEG_STOPWATCH) > 1000) {
        anodeCnt++;
        ANODES = ~(1 << (anodeCnt % 4));
        SEVEN_SEG = sseg_out[anodeCnt % 4];
        start_stopwatch(SSEG_STOPWATCH);
    }
}

_Bool read_left_enc(void) {
    static _Bool last_reading = false;
    _Bool current_reading = (JA & (1 << L_ENC_OFFSET)) == (1 << L_ENC_OFFSET);
    if(last_reading != current_reading){
        last_reading = current_reading;
        return last_reading;
    }
    return false;
}

_Bool read_right_enc(void) {
    static _Bool last_reading = false;
    _Bool current_reading = (JA & (1 << R_ENC_OFFSET)) == (1 << R_ENC_OFFSET);
    if(last_reading != current_reading){
        last_reading = current_reading;
        return last_reading;
    }
    return false;

_Bool start_switch_toggled(){
    static _Bool last = 0, cur = 0;
    last = cur;
    cur = (SWITCHES & 1) == 1;
    return (cur ^ last);
}

int clamp(int new, int min, int max){
    if(new>max){
        return max;
    }
    else if(new<min){
        return min;
    }
    else{
        return new;
    }
}
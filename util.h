#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"
#include "timers.h"

// SSEG display
void show_sseg();

// switch input
_Bool start_switch_toggled();

// encoders
_Bool read_left_enc(void);
_Bool read_right_enc(void);

// helper funcs
int clamp(int new, int min, int max);

#endif
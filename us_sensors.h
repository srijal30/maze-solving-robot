#ifndef US_SENSORS_H
#define US_SENSORS_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"
#include "timers.h"

void set_trig_pin(uint8_t offset);
void clear_trig_pin(uint8_t offset);
_Bool read_echo_pin(uint8_t offset);
uint32_t update_distance(uint8_t trig, uint8_t echo);
void update_distance_left(_Bool force);
void update_distance_front(_Bool force);

#endif // US_SENSORS_H

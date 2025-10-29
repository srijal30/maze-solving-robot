#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"

uint32_t *convert_timer_to_hex_address(uint8_t timer_number);
void configure_timers();
void start_stopwatch(uint8_t timer_number);
uint32_t read_stopwatch(uint8_t timer_number);
void timer_2us(unsigned t);

#endif // TIMERS_H

#include "timers.h"

uint32_t *convert_timer_to_hex_address(uint8_t timer_number) {
    if (timer_number > 7) {
        return 0;
    }
    uint32_t* timer_base_address = TIMER_BASE + ((timer_number/2) * 0x0400);
    if (timer_number & 1) {
        timer_base_address += 0x40;
    }
    return timer_base_address;
}

void configure_timers() {
    for (int i = 0; i < 8; i++) {
        uint32_t *timer_base_address = convert_timer_to_hex_address(i);
        uint32_t *tcr = timer_base_address + (volatile uint32_t)TCR_OFFSET;
        uint32_t *tcsr = timer_base_address + (volatile uint32_t)TCSR_OFFSET;
        *(tcr) = 0x00000000;
        *(tcsr) = 0b010010010001;
    }
}

void start_stopwatch(uint8_t timer_number) {
    if (timer_number > 7) {
        return;
    }
    uint32_t *timer_base_address = convert_timer_to_hex_address(timer_number);
    volatile uint32_t *tcsr = timer_base_address + (volatile uint32_t)TCSR_OFFSET;
    *tcsr &= ~(1 << 7);
    *tcsr |= (1 << 5);
    *tcsr &= ~(1 << 5);
    *tcsr |= (1 << 7);
}

uint32_t read_stopwatch(uint8_t timer_number) {
    if (timer_number > 7) {
        return 0;
    }
    uint32_t *timer_base_address = convert_timer_to_hex_address(timer_number);
    volatile uint32_t *tcr = timer_base_address + (volatile uint32_t)TCR_OFFSET;
    return (*tcr) / 100;
}

void timer_2us(unsigned t) {
  volatile unsigned cntr1;
  while (t--)
    for (cntr1 = 0; cntr1 < 8; cntr1++);
}

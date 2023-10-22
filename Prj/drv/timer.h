#ifndef TIMER_H
#define TIMER_H

void set_x_ms_arr(uint8_t arr);
uint8_t get_x_ms_arr(void);
void set_8_ms_arr(uint8_t arr);
uint8_t get_8_ms_arr(void);
void set_timer_period(uint32_t p);
void process_timer_cnt_in_period(uint32_t us);

#endif

#ifndef FRQ_H
#define FRQ_H

void set_frq(uint32_t frq);
uint32_t get_polling_delay_us(void);
uint8_t is_main_task_arr(void);
void reset_main_task_arr(void);

#endif

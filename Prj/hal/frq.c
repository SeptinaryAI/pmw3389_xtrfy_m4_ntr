#include <stdint.h>
#include "timer.h"
#include "frq.h"

static uint32_t g_polling_delay_us = 1000;

void set_frq(uint32_t frq)
{
    g_polling_delay_us = 1001000 / frq; //period should slightly greater than 1000us, maybe 1001us, you must wait for SOP
    set_timer_period(g_polling_delay_us);
}

uint32_t get_polling_delay_us(void)
{
    return g_polling_delay_us;
}

uint8_t is_main_task_arr(void)
{
    return get_x_ms_arr();
}

void reset_main_task_arr(void)
{
    set_x_ms_arr(0);
}

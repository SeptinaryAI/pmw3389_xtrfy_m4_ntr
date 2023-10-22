#include <stdint.h>
#include "timer.h"

static uint32_t g_timer_cnt = 0;
static uint32_t g_timer_cnt2 = 0;
static uint32_t g_timer_period = 1000;  //1000us-frq1000 ; 2000us-frq500 ; 8000us-frq125
static uint8_t  g_x_ms_arr = 0;
static uint8_t  g_8_ms_arr = 0;


void set_x_ms_arr(uint8_t arr)
{
    g_x_ms_arr = arr;
}

uint8_t get_x_ms_arr(void)
{
    return g_x_ms_arr;
}

void set_8_ms_arr(uint8_t arr)
{
    g_8_ms_arr = arr;
}

uint8_t get_8_ms_arr(void)
{
    return g_8_ms_arr;
}

void set_timer_period(uint32_t p)
{
    g_timer_period = p;
}

void process_timer_cnt_in_period(uint32_t us)
{
    g_timer_cnt += us;
    g_timer_cnt2 += us;

    if (g_timer_cnt >= g_timer_period) {    //x ms task arr
        set_x_ms_arr(1);
        g_timer_cnt = 0;
    }

    if (g_timer_cnt2 >= 8000) { //8 ms task arr
        set_8_ms_arr(1);
        g_timer_cnt2 = 0;
    }
}

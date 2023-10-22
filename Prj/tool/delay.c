#include "stm32f103xb.h"
#include "core_cm3.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_cortex.h"
#include "delay.h"

#define SYS_CLK 72000000

static uint32_t g_fac_us = 9;

void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
    SysTick->CTRL = 0;
    g_fac_us = SYS_CLK / 8000000;
}


void delay_us(uint32_t us)
{
    uint32_t ctl = 0;
    SysTick->VAL = 0;       //reset 0
    SysTick->LOAD = us * g_fac_us;
    SysTick->CTRL |= 0x1;   //en
    do {
        //wait time 
        ctl = SysTick->CTRL;
    } while(!(ctl & 0x10000));
    SysTick->CTRL &= ~0x1;  //dis
    SysTick->VAL = 0;       //reset 0
}

void delay_ms(uint32_t ms)
{
    uint32_t s = ms / 1000;
    uint32_t re = ms % 1000;
    for (; s; --s)
        delay_us(1000 * 1000);

    if (re)
        delay_us(re * 1000);
}

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "timer.h"
#include "enc.h"

extern TIM_HandleTypeDef htim2;

static int32_t g_enc_move = 0;

//init counter to a middle value
void init_enc(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim2, ENC_TIM_PERIOD / 2);
}

void callback_enc_irq_do(void)
{
    (__HAL_TIM_GET_COUNTER(&htim2) < (ENC_TIM_PERIOD / 2)) ?
                    ++g_enc_move : --g_enc_move;
}

uint32_t get_enc_cnt(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}

void set_enc_cnt(uint32_t cnt)
{
    __HAL_TIM_SET_COUNTER(&htim2, cnt);
}


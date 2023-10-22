#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "led.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

#define BRIGHTNESS_MAX  1000

void init_rgb_main(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 999);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 999);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 999);
}


void dis_rgb_main(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
}

void set_rgb_main_by_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t)(r) * 10);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t)(g) * 10);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t)(b) * 10);
}

void init_rgb_cpi(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1000);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1000);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 100);
}

void set_rgb_cpi_by_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (uint16_t)(100 - r) * 10);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, (uint16_t)(100 - g) * 10);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, (uint16_t)(100 - b) * 10);
}

void dis_rgb_cpi(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1000);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1000);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 1000);
}


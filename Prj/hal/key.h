#ifndef KEY_H
#define KEY_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#define IS_BTN_SID_B_DOWN   (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
#define IS_BTN_SID_F_DOWN   (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET)
#define IS_BTN_L_DOWN       (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET)
#define IS_BTN_R_DOWN       (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET)
#define IS_BTN_M_DOWN       (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET)

#define IS_BTN_CPI_DOWN     (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
#define IS_BTN_FUNC_DOWN    (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET)

//use PC13 pull up, make sure : MX_RTC_Init() -> ....; hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE; ....
#define IS_BTN_FRQ1000_SELECT       (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
#define IS_BTN_FRQ500_SELECT        (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == GPIO_PIN_RESET)
#define IS_BTN_FRQ125_SELECT        (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET)

void dbg_read_all_btn(void);

#endif

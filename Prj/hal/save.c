#include <stdint.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "save.h"

//flash erase page
static void flash_erase_one_page(uint32_t page_addr)
{
    FLASH_EraseInitTypeDef fe = {0};
    uint32_t page_err = 0;

    fe.TypeErase = FLASH_TYPEERASE_PAGES;
    fe.PageAddress = page_addr;
    fe.NbPages = 1;
    HAL_FLASHEx_Erase(&fe, &page_err);  //ignore err
}

static void volatile_copy(uint8_t *t, uint8_t* addr, uint32_t len)
{
    int i = 0;

    for (; i < len; ++i) {
        *(t + i) = *((volatile uint8_t*)(addr + i));
    }
}

uint8_t load_param(t_mouse_param *param)
{
    t_mouse_param *load = NULL;
    load = (t_mouse_param*)FLASH_USE_ADDR;
    //memcpy(param, load, sizeof(t_mouse_param));
    volatile_copy((uint8_t *)param, (uint8_t *)load, sizeof(t_mouse_param));

    return 0;
}
uint8_t save_param(t_mouse_param *param)
{
    uint32_t i = 0;
    uint64_t each_wr = 0;

    HAL_FLASH_Unlock();
    flash_erase_one_page(FLASH_USE_ADDR);

    for (i = 0 ; i < sizeof(t_mouse_param) ; i += sizeof(uint32_t)) {
        each_wr = (uint64_t)(*((uint32_t *)((uint8_t *)param + i)));    //have to make a uint16_t for HAL_FLASH_Program para_3
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USE_ADDR + i, each_wr);
    }
    HAL_FLASH_Lock();

    return 0;
}


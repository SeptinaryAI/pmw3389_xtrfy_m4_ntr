#ifndef SAVE_H
#define SAVE_H

#define FLASH_USE_ADDR  0x0801FC00          //for stm32f103c8t6 medium density ,page 127

#pragma pack(1)

typedef struct {
    uint16_t cpi;
    uint8_t cpi_index;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t rgb_index;
    uint8_t rgb_mode;
} t_mouse_param;

#pragma pack()

uint8_t load_param(t_mouse_param *param);
uint8_t save_param(t_mouse_param *param);

#endif


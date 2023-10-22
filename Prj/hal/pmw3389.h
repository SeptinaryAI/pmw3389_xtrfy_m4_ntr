#ifndef PMW3389_H
#define PMW3389_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_gpio.h"

#define PMW3389_SPI_NCS_GPIO    GPIOA
#define PMW3389_SPI_NCS_PIN     GPIO_PIN_4

// Registers



#define PRODUCT_ID 0X00
#define REVISION_ID 0X01
#define MOTION 0X02
#define DELTA_X_L 0X03
#define DELTA_X_H 0X04
#define DELTA_Y_L 0X05
#define DELTA_Y_H 0X06
#define SQUAL 0X07
#define RAW_DATA_SUM 0X08
#define MAXIMUM_RAW_DATA 0X09
#define MINIMUM_RAW_DATA 0X0A
#define SHUTTER_LOWER 0X0B
#define SHUTTER_UPPER 0X0C
#define RIPPLE_CONTROL 0X0D
#define RESOLUTION_L 0X0E
#define RESOLUTION_H 0X0F
#define CONFIG2 0X10
#define ANGLE_TUNE 0X11
#define FRAME_CAPTURE 0X12
#define SROM_ENABLE 0X13
#define RUN_DOWNSHIFT 0X14
#define REST1_RATE_LOWER 0X15
#define REST1_RATE_UPPER 0X16
#define REST1_DOWNSHIFT 0X17
#define REST2_RATE_LOWER 0X18
#define REST2_RATE_UPPER 0X19
#define REST2_DOWNSHIFT 0X1A
#define REST3_RATE_LOWER 0X1B
#define REST3_RATE_UPPER 0X1C
#define OBSERVATION 0X24
#define DATA_OUT_LOWER 0X25
#define DATA_OUT_UPPER 0X26
#define SROM_ID 0X2A
#define MIN_SQ_RUN 0X2B
#define RAW_DATA_THRESHOLD 0X2C
#define CONTROL2 0X2D
#define CONFIG5_L 0X2E
#define CONFIG5_H 0X2F
#define POWER_UP_RESET 0X3A
#define SHUTDOWN 0X3B
#define INVERSE_PRODUCT_ID 0X3F
#define LIFTCUTOFF_CAL3 0X41
#define ANGLE_SNAP 0X42
#define LIFTCUTOFF_CAL1 0X4A
#define MOTION_BURST 0X50
#define SROM_LOAD_BURST 0X62
#define LIFT_CONFIG 0X63
#define RAW_DATA_BURST 0X64
#define LIFTCUTOFF_CAL2 0X65
#define LIFTCUTOFF_CAL_TIMEOUT 0X71
#define LIFTCUTOFF_CAL_MIN_LENGTH 0X72
#define PWM_PERIOD_CNT 0X73
#define PWM_WIDTH_CNT 0X74

uint8_t get_pmw3389_x(uint8_t *xl, uint8_t *xh);
uint8_t get_pmw3389_y(uint8_t *yl, uint8_t *yh);
uint8_t pmw3389_set_cpi(uint16_t cpi);

uint8_t start_up_pmw3389(void);
uint8_t set_pmw3389_burstmode(void);
uint8_t refresh_pmw3389_dat_in_burstmode(void);
uint8_t *get_burst_buf(void);

#endif

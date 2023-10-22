#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_spi.h"
#include "delay.h"
#include "pmw3389_rom.h"
#include "pmw3389.h"
#include "spi.h"

//STM32 HAL style:
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

static uint8_t ga_burst_buf[12] = {0};  //get pmw3389 dats in burst mode

uint8_t *get_burst_buf(void)
{
    return ga_burst_buf;
}

//pull down to enable
static inline void en_pmw3389(void)
{
    HAL_GPIO_WritePin(PMW3389_SPI_NCS_GPIO, PMW3389_SPI_NCS_PIN, GPIO_PIN_RESET);
}

static inline void dis_pmw3389(void)
{
    HAL_GPIO_WritePin(PMW3389_SPI_NCS_GPIO, PMW3389_SPI_NCS_PIN, GPIO_PIN_SET);
}

static inline uint8_t spi_rdwr(uint8_t wr_bt)
{
    uint8_t bt = 0;
    HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(&hspi1, &wr_bt, &bt, 1, 10);

    return bt;
}

static inline uint8_t spi_rd_buf(uint8_t *buf, uint16_t len)
{
    HAL_SPI_Receive(&hspi1, buf, len, 10);

    return 0;
}

static inline void reg_wr(uint8_t addr, uint8_t dat)
{
    en_pmw3389();

    spi_rdwr(addr | 0x80);
    spi_rdwr(dat);

    delay_us(180);
    dis_pmw3389();
}

static inline uint8_t reg_rd(uint8_t addr)
{
    uint8_t val = 0;
    en_pmw3389();

    spi_rdwr(addr & 0x7F);
    delay_us(160);
    val = spi_rdwr(0);

    delay_us(20);
    dis_pmw3389();

    return val;
}


//refrence from https://github.com/tomKPZ/pmw3389_bluemicro
uint8_t pmw3389_set_cpi(uint16_t cpi)
{
    uint16_t cpi_set = cpi / 50 - 1;

    if (cpi_set > 0x013F)   //max val 16000 / 50 -1
        cpi_set = 0x013F;

    reg_wr(RESOLUTION_H, cpi_set >> 8);
    reg_wr(RESOLUTION_L, cpi_set & 0xFF);

    return 0;
}

//refrence from https://github.com/mrjohnk/PMW3389DM
uint8_t update_pmw3389_fw(void)
{
    //Write 0 to Rest_En bit of Config2 register to disable Rest mode.
    reg_wr(CONFIG2, 0x00);

    // write 0x1d in SROM_enable reg for initializing
    reg_wr(SROM_ENABLE, 0x1d);

    // wait for more than one frame period
    delay_ms(10);

    // write 0x18 to SROM_enable to start SROM download
    reg_wr(SROM_ENABLE, 0x18);

    en_pmw3389();
    // write the SROM file (=firmware data)
    spi_rdwr(SROM_LOAD_BURST | 0x80); // write burst destination adress

    // send all bytes of the firmware
    for(int i = 0; i < sizeof(g_pmw3389_rom); ++i) {
        delay_us(16);
        spi_rdwr(g_pmw3389_rom[i]);
    }
    delay_us(20);
    dis_pmw3389();

    delay_us(200);

    return 0;
}

//refrence from https://github.com/Ghost-Girls/PMW3360-3389-PAW3395_STM32-CH32-APM32
uint8_t start_up_pmw3389(void)
{
    dis_pmw3389();
    delay_ms(3);

    reg_wr(SHUTDOWN, 0xB6);
    delay_us(300);

    en_pmw3389();
    delay_us(40);
    dis_pmw3389();
    delay_us(40);

    reg_wr(POWER_UP_RESET, 0x5A);
    delay_ms(50);

    reg_rd(MOTION);
    reg_rd(DELTA_X_L);
    reg_rd(DELTA_X_H);
    reg_rd(DELTA_Y_L);
    reg_rd(DELTA_Y_H);

    update_pmw3389_fw();
    reg_wr(CONFIG2, 0x00);  //independant XY CPI disabled
    reg_wr(RIPPLE_CONTROL, 0x00);  //camera angle
    reg_wr(ANGLE_TUNE, 0x00);  //camera angle fine tuning
    reg_wr(LIFT_CONFIG, 0x03);   //LOD = 3mm
    reg_wr(MIN_SQ_RUN, 0x80);       //Minimum SQUAL for zero motion data (default: 0x10)
    reg_wr(RAW_DATA_THRESHOLD, 0x0A);   //Minimum Valid features (reduce SQUAL score) (default: 0x0a)

    reg_wr(ANGLE_SNAP, 0x00);       //0x00: off, 0x80: on

    pmw3389_set_cpi(800);       //

    return 0;
}

uint8_t get_pmw3389_x(uint8_t *xl, uint8_t *xh)
{
    *xl = reg_rd(DELTA_X_L);
    *xh = reg_rd(DELTA_X_H);

    return 0;
}

uint8_t get_pmw3389_y(uint8_t *yl, uint8_t *yh)
{
    *yl = reg_rd(DELTA_Y_L);
    *yh = reg_rd(DELTA_Y_H);

    return 0;
}

uint8_t set_pmw3389_burstmode(void)
{
    reg_wr(MOTION_BURST, 0x00); //set burst mode
    delay_us(185);

    return 0;
}

uint8_t refresh_pmw3389_dat_in_burstmode(void)
{
    en_pmw3389();
    delay_us(1);   //>= 120us

    spi_rdwr(MOTION_BURST); //read burst mode dat
    delay_us(37);   //>= 35us

    spi_rd_buf(ga_burst_buf, 12);

    dis_pmw3389();
    delay_us(1);    //>=0.5us

    return 0;
}

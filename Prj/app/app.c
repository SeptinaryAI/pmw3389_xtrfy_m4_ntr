#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_pcd.h"
#include "usbd_def.h"
#include "usbd_hid.h"
#include "dbg.h"
#include "delay.h"
#include "key.h"
#include "enc.h"
#include "led.h"
#include "frq.h"
#include "pmw3389.h"
#include "save.h"
#include "app.h"

//choose one of them:
#define FRQ_USE_MAIN_DELAY      //use delay to make return rate frquency
//#define FRQ_USE_TIMER_IRQ       //use timer irq to make return rate frquency

extern USBD_HandleTypeDef hUsbDeviceFS;

#define CHANGED_WHL_MASK    0x1
#define CHANGED_BTN_MASK    0x2
#define CHANGED_MOTION_MASK 0x4

static uint8_t g_ope_changed = 0;

static uint8_t g_dat_send_stat = 0;

static t_mouse_param g_mouse_param = {0};

static t_mouse_dat g_mouse_dat = {
    0
};

const uint16_t ga_cpi_list[] = {400, 800, 1200, 1600, 2000};
static uint8_t ga_rgb_cpi_map[][3] = {
    //R   G    B
    {0, 0, 0  },        //cpi = 400
    {100, 100, 0},      //cpi = 800
    {80,  80,  20 },    //cpi = 1000
    {20,  20,  80 },    //cpi = 1500
    {0,  0,  100 },     //cpi = 2000
};
const uint8_t ga_rgb_list[][3] = {
    {100, 20, 20},
    {20, 100, 20},
    {20, 20, 100},
    {100, 100, 20},
    {20, 100, 100},
    {100, 20, 100},
    {100, 100, 100},
};

static inline t_mouse_dat *get_mouse_dat(void)
{
    return &g_mouse_dat;
}


/**
 * set x-index mouse move and sign
*/
static inline void set_x_move_hl(uint8_t h, uint8_t l)
{
    get_mouse_dat()->u_x_move.t_x_move.x_move_l = l;
    get_mouse_dat()->u_x_move.t_x_move.x_move_h = h;
}

/**
 * set y-index mouse move and sign
*/
static inline void set_y_move_hl(uint8_t h, uint8_t l)
{
    get_mouse_dat()->u_y_move.t_y_move.y_move_l = l;
    get_mouse_dat()->u_y_move.t_y_move.y_move_h = h;
}

//static inline void set_x_move(int16_t mv)
//{
//    get_mouse_dat()->u_x_move.x_move = mv;
//}
//
//static inline void set_y_move(int16_t mv)
//{
//    get_mouse_dat()->u_y_move.y_move = mv;
//}
//
//static inline void add_x_move(int16_t add)
//{
//    get_mouse_dat()->u_x_move.x_move += add;
//}
//
//static inline void add_y_move(int16_t add)
//{
//    get_mouse_dat()->u_y_move.y_move += add;
//}

static inline void clean_xy_move(void)
{
    get_mouse_dat()->u_x_move.x_move = 0;
    get_mouse_dat()->u_y_move.y_move = 0;
}

static inline uint8_t send_usb_dat(void)
{
    return USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)get_mouse_dat(), sizeof(t_mouse_dat));
}

void set_rgb_cpi(uint8_t cpi)
{
    uint8_t r = ga_rgb_cpi_map[cpi][0];
    uint8_t g = ga_rgb_cpi_map[cpi][1];
    uint8_t b = ga_rgb_cpi_map[cpi][2];

    set_rgb_cpi_by_rgb(r, g, b);
}

static inline void update_main_key(void)
{
    static uint8_t last_key_map = 0x0;
    uint8_t key_map = 0x0;

    if (IS_BTN_L_DOWN) {
        get_mouse_dat()->btn_l = 1;
        key_map |= 0x1;
    } else {
        get_mouse_dat()->btn_l = 0;
        key_map &= ~0x1;
    }

    if (IS_BTN_R_DOWN) {
        get_mouse_dat()->btn_r = 1;
        key_map |= 0x2;
    } else {
        get_mouse_dat()->btn_r = 0;
        key_map &= ~0x2;
    }

    if (IS_BTN_M_DOWN) {
        get_mouse_dat()->btn_m = 1;
        key_map |= 0x4;
    } else {
        get_mouse_dat()->btn_m = 0;
        key_map &= ~0x4;
    }

    if (IS_BTN_SID_F_DOWN) {
        get_mouse_dat()->btn_f = 1;
        key_map |= 0x8;
    } else {
        get_mouse_dat()->btn_f = 0;
        key_map &= ~0x8;
    }

    if (IS_BTN_SID_B_DOWN) {
        get_mouse_dat()->btn_b = 1;
        key_map |= 0x10;
    } else {
        get_mouse_dat()->btn_b = 0;
        key_map &= ~0x10;
    }

    if (key_map != last_key_map) {
        last_key_map = key_map;
        g_ope_changed |= CHANGED_BTN_MASK;
    } else {
        g_ope_changed &= ~CHANGED_BTN_MASK;
    }

}

static void update_whl_dat_by_enc(void)
{
    int32_t cnt = (int32_t)get_enc_cnt();

    if (cnt > ENC_TIM_PERIOD / 2) {   //only care about > or <
        get_mouse_dat()->wheel_move = 0x01;     //scroll up
        set_enc_cnt(ENC_TIM_PERIOD / 2);
        g_ope_changed |= CHANGED_WHL_MASK;
    } else if (cnt < ENC_TIM_PERIOD / 2){
        get_mouse_dat()->wheel_move = 0xFF;     //scroll down
        set_enc_cnt(ENC_TIM_PERIOD / 2);
        g_ope_changed |= CHANGED_WHL_MASK;
    } else {
        get_mouse_dat()->wheel_move = 0x80;     //do nothing
        g_ope_changed &= ~CHANGED_WHL_MASK;
    }
}

static inline void update_frq_by_key(void)
{
    static uint32_t last_frq = 0;

    if (IS_BTN_FRQ125_SELECT) {
        if (last_frq != 125) {
            last_frq = 125;
            set_frq(125);
        }
    } else if (IS_BTN_FRQ500_SELECT) {
        if (last_frq != 500) {
            last_frq = 500;
            set_frq(500);
        }
    } else {    //FRQ1000
        if (last_frq != 1000) {
            last_frq = 1000;
            set_frq(1000);
        }
    }
}

#define GET_INT16_BY_BYTE_HL(h, l)  ((int16_t)(uint16_t)(((h) << 8) | (l)))

static inline void update_xy_by_sensor(void)
{
    uint8_t *p_buf = NULL;
    refresh_pmw3389_dat_in_burstmode();

    p_buf = get_burst_buf();

    if (p_buf[0] & 0x80) {  //motion detected
        set_x_move_hl(p_buf[3], p_buf[2]);
        set_y_move_hl(p_buf[5], p_buf[4]);
        g_ope_changed |= CHANGED_MOTION_MASK;
    } else {
        clean_xy_move();
        g_ope_changed &= ~CHANGED_MOTION_MASK;
    }

    //add_x_move(GET_INT16_BY_BYTE_HL(p_buf[3], p_buf[2]));
    //add_y_move(GET_INT16_BY_BYTE_HL(p_buf[5], p_buf[4]));
}

static inline void update_cpi_by_key(void)
{
    static uint8_t last_key_have_up = 1;

    if (!IS_BTN_CPI_DOWN)
        last_key_have_up = 1;

    if (last_key_have_up == 1 && IS_BTN_CPI_DOWN) {
        ++g_mouse_param.cpi_index;

        if (g_mouse_param.cpi_index >= sizeof(ga_cpi_list)/sizeof(ga_cpi_list[0]) ) {
            g_mouse_param.cpi_index = 0;
        }

        g_mouse_param.cpi = ga_cpi_list[g_mouse_param.cpi_index];

        pmw3389_set_cpi(g_mouse_param.cpi);
        set_rgb_cpi(g_mouse_param.cpi_index);
        save_param(&g_mouse_param);     //save to flash, power down save

        last_key_have_up = 0;
    }
}

static inline void update_main_rgb(void)    //I don't like RGB, so rewrite it by yourself :)
{
    static uint8_t last_key_have_up = 1;

    if (!IS_BTN_FUNC_DOWN)
        last_key_have_up = 1;

    if (last_key_have_up == 1 && IS_BTN_FUNC_DOWN) {
        ++g_mouse_param.rgb_index;

        if (g_mouse_param.rgb_index >= sizeof(ga_rgb_list)/sizeof(ga_rgb_list[0])) {
            g_mouse_param.rgb_index = 0;
        }

        g_mouse_param.r = ga_rgb_list[g_mouse_param.rgb_index][0];
        g_mouse_param.g = ga_rgb_list[g_mouse_param.rgb_index][1];
        g_mouse_param.b = ga_rgb_list[g_mouse_param.rgb_index][2];

        set_rgb_main_by_rgb(g_mouse_param.r, g_mouse_param.g, g_mouse_param.b);
        save_param(&g_mouse_param);     //save to flash, power down save

        last_key_have_up = 0;
    }
}

static inline void init_mouse_param(void)
{
    load_param(&g_mouse_param);

    //check
    if (g_mouse_param.cpi_index >= sizeof(ga_cpi_list)/sizeof(ga_cpi_list[0]) ) {
        g_mouse_param.cpi_index = 0;
        g_mouse_param.cpi = DEFAULT_CPI;    //default
    }

    if ((g_mouse_param.cpi > 16000)
    ||  (g_mouse_param.cpi < 50)
    ||  (g_mouse_param.cpi % 50 != 0) ) {
        g_mouse_param.cpi = DEFAULT_CPI;    //default
    }

    if (g_mouse_param.r > 100)
        g_mouse_param.r = 100;
    if (g_mouse_param.g > 100)
        g_mouse_param.g = 100;
    if (g_mouse_param.b > 100)
        g_mouse_param.b = 100;

    if (g_mouse_param.rgb_index >= sizeof(ga_rgb_list)/sizeof(ga_rgb_list[0])) {
        g_mouse_param.rgb_index = 0;
        g_mouse_param.r = ga_rgb_list[g_mouse_param.rgb_index][0];
        g_mouse_param.g = ga_rgb_list[g_mouse_param.rgb_index][1];
        g_mouse_param.b = ga_rgb_list[g_mouse_param.rgb_index][2];
    }

    //set to hw, for sync
    pmw3389_set_cpi(g_mouse_param.cpi);
    set_rgb_cpi(g_mouse_param.cpi_index);
    set_rgb_main_by_rgb(g_mouse_param.r, g_mouse_param.g, g_mouse_param.b);

    save_param(&g_mouse_param);
}

void user_init(void)
{
    start_up_pmw3389();
    init_mouse_param();
    set_pmw3389_burstmode();
}

void do_report(void)
{
    update_main_key();
    update_whl_dat_by_enc();
    update_xy_by_sensor();

    if (g_ope_changed) {
        g_dat_send_stat |= send_usb_dat();
    }
}


void main_loop(void)
{
#ifdef FRQ_USE_MAIN_DELAY
    //nothing
#else
#ifdef FRQ_USE_TIMER_IRQ
    if (is_main_task_arr()) {        //wait for x ms (1000frq-1ms;500frq-2ms;125frq-8ms) task arriving
        reset_main_task_arr();
#endif
#endif
        do_report();

        //clean_xy_move();
        //delay_us(get_polling_delay_us());
        update_cpi_by_key();
        update_frq_by_key();
        update_main_rgb();
        if (g_dat_send_stat) {
            set_rgb_main_by_rgb(0, 0, 0);
        }

#ifdef FRQ_USE_MAIN_DELAY
        delay_us(get_polling_delay_us());
#else
#ifdef FRQ_USE_TIMER_IRQ
    }
#endif
#endif
}

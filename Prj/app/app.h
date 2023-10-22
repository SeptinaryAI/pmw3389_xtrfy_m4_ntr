#ifndef APP_H
#define APP_H

#pragma pack(1)

#define DEFAULT_CPI 800

//typedef struct {
//    //byte0
//    uint32_t btn_l:1;           //b0
//    uint32_t btn_r:1;           //b1
//    uint32_t btn_m:1;           //b2
//    uint32_t always_1:1;      //b3
//    uint32_t x_move_sign:1;     //b4
//    uint32_t y_move_sign:1;     //b5
//    uint32_t x_overflow:1;      //b6
//    uint32_t y_overflow:1;      //b7
//    //byte1
//    uint8_t x_move;
//    //byte2
//    uint8_t y_move;
//    //byte3
//    uint8_t wheel_move;
//} t_mouse_dat;


typedef struct {
    uint8_t btn_l:1;            //byte0 :bit0
    uint8_t btn_r:1;            //      b1
    uint8_t btn_m:1;            //      b2
    uint8_t btn_f:1;            //      b3
    uint8_t btn_b:1;            //      b4
    uint8_t nouse0:1;           //      b5
    uint8_t nouse1:1;           //      b6
    uint8_t nouse2:1;           //      b7
    union {
        struct {
            uint8_t x_move_l;        //byte1
            uint8_t x_move_h;        //byte2
        } t_x_move;
        int16_t x_move;
    } u_x_move;
    union {
        struct {
            uint8_t y_move_l;        //byte3
            uint8_t y_move_h;        //byte4
        } t_y_move;
        int16_t y_move;
    } u_y_move;

    uint8_t wheel_move;         //byte5
} t_mouse_dat;

#pragma pack()

void user_init(void);
void main_loop(void);

#endif

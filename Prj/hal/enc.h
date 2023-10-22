#ifndef ENC_H
#define ENC_H

#define ENC_TIM_PERIOD 65536


void init_enc(void);
void callback_enc_irq_do(void);
uint32_t get_enc_cnt(void);
void set_enc_cnt(uint32_t cnt);


#endif

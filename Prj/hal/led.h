#ifndef LED_H
#define LED_H

void init_rgb_main(void);
void dis_rgb_main(void);
void init_rgb_cpi(void);
void dis_rgb_cpi(void);
void set_rgb_main_by_rgb(uint8_t r, uint8_t g, uint8_t b);
void set_rgb_cpi_by_rgb(uint8_t r, uint8_t g, uint8_t b);


#endif

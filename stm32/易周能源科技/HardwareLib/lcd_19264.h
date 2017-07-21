
#ifndef _LCD_19264_H
#define _LCD_19264_H

#include "stm32f1xx_hal.h"

void init_lcd_192_64(void);

void write_cmd_lcd_192_64(uint8_t cmd);

void write_data_lcd_192_64(uint8_t data);

void set_area_lcd_192_64(uint8_t area);

void clear_lcd_192_64(void);

void set_row_lcd_192_64(uint8_t row);

void set_rank_lcd_192_64(uint8_t rank);



#endif
#ifndef _GUI_H

#define _GUI_H
#include "stm32f1xx_hal.h"
#include "lcd_12864.h"

typedef enum{
	FILL_TYPE_DEFUALT,
	FILL_TYPE_CLEAR,
	FILL_TYPE_INVERSE,	
}fill_type;

typedef struct{
	uint8_t string[LCD_LONG / 8];
	uint8_t value;
}register_t;

typedef struct {
	uint8_t          *string;
	void             (*opt)(uint8_t p);
  struct	menu_t   *next;
  struct	menu_t   *last;
	register_t       *regis;
}menu_t;

void ui_drew_line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,fill_type fill_type);
void ui_drew_rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,fill_type fill_type);

#endif

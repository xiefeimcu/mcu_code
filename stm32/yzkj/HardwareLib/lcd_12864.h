
#ifndef _LCD12864_H_

#define _LCD12864_H_

#include "stm32f1xx_hal.h"

#define LCD_OPEN_BK()   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_SET)
#define LCD_CLOSE_BK()  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_RESET)
#define LCD_OPEN_PW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9, GPIO_PIN_SET)
#define LCD_CLOSE_PW()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9, GPIO_PIN_RESET)

/*RS*/
#define LCD_RS_DATA()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5, GPIO_PIN_SET)
#define LCD_RS_CMD()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5, GPIO_PIN_RESET)

/*R/W*/
#define LCD_RW_READ()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4, GPIO_PIN_SET)
#define LCD_RW_WRITE() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4, GPIO_PIN_RESET)

/*E*/
#define LCD_E_ENABLE()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_SET)
#define LCD_E_DISABLE() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET)

#define LCD_SETUP_DATA(a) do{\
	uint16_t temp=0x0000;\
	temp |= a;\
	GPIOD->ODR |=temp;\
}while(0);

#define LCD_WIDTH 64
#define LCD_LONG  128

#define LCD_BUSY_PIN    GPIO_PIN_7
#define LCD_BUSY_PORT   GPIOD

typedef struct{
	void (*init)            (void);
	void (*show_char)       (uint8_t  row, uint8_t  col, uint8_t  dat);
	void (*show_string)     (uint8_t  row, uint8_t  col, uint8_t  *s);
	void (*show_gbs)        (uint8_t  row, uint8_t  col, uint8_t  *s);
	void (*show_num)        (uint8_t  row, uint8_t  col, uint16_t num,uint8_t  DecOrHex);
	void (*show_image)      (const uint8_t *str);
	void (*open_power)      (void);
	void (*close_power)     (void);
	void (*open_back_light) (void);
	void (*close_back_light)(void);
}lcd_12864_opt;

extern const lcd_12864_opt lcd_handle;

#endif



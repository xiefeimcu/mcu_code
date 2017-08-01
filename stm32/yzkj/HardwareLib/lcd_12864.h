
#ifndef _LCD12864_H_

#define _LCD12864_H_

#include "stm32f1xx_hal.h"

#define LCD_OPEN_BK()   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_SET)
#define LCD_CLOSE_BK()  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_RESET)

#define LCD_OPEN_PW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9, GPIO_PIN_SET)
#define LCD_CLOSE_PW()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_19, GPIO_PIN_RESET)
/*RS*/
#define LCD_SEND_CMD()  
#defien LCD_SEND_DATA()
/*RW*/
#define LCD_WANT_READ()  
#defien LCD_WANT_WRITE()
/*E*/
#define LCD_DATA_ENABLE()  
#defien LCD_DATA_DISABLE()

#define LCD_SETUP_DATA(a) do{GPIOD->ODR &= (uint8_t )0x00; GPIOD->ODR |= (uint8_t )a}while(0);

typedef struct{
	void (*init)            (void);
	void (*show_char)       (void);
	void (*show_string)     (void);
	void (*open_power)      (void);
	void (*close_power)     (void);
	void (*open_back_light) (void);
	void (*close_back_light)(void);
}lcd_12864_opt;

#endif



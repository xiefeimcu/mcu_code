
#ifndef _KEY_H

#define _KEY_H

#include "stm32f1xx_hal.h"

#define KEY_FILTE_TIME_MS 10

#define	READ_KEY_UP()     HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1)
#define	READ_KEY_DOWN()   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)
#define	READ_KEY_LEFT()   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)
#define	READ_KEY_RIGHT()  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6)
#define	READ_KEY_SET()    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)
#define	READ_KEY_RETURN() HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)

#define KEY_DATA (uint8_t)((uint8_t)(GPIOE->IDR >>1) | 0xC0)

typedef enum {
	KEY_NULL,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_SET,
	KEY_RETURN,
}key_event_t;

extern key_event_t key_event_handle;

key_event_t scan_key(void);

#endif


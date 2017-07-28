
#ifndef _LCD12864_H_

#define _LCD12864_H_

#incldue "stm32f1xx_hal.h"

typedef struct{
	void (*init)            (void);
	void (*show_char)       (void);
	void (*show_string)     (void);
	void (*open_power)      (void);
	void (*close_power)     (void);
	void (*open_back_light) (void);
	void (*close_back_light)(void);
}lcd_12864_opt

#endif



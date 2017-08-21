
#ifndef _RTU_CONFIG_H

#define _RTU_CONFIG_H

#include "stm32f1xx_hal.h"

typedef enum{
	BAUD_1200,
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_14400,
	BAUD_19200,
	BAUD_43000,
	BAUD_57600,
	BAUD_76800,
	BAUD_115200,
}baud_t;

typedef struct{
	uint8_t baud_485_1;
	uint8_t baud_485_2;
	uint8_t baud_232_1;
	uint8_t baud_232_2;
	
	int16_t  Backlight_time_s;
	uint16_t sensor_sampling_time_s;
	
	uint16_t alarm_interval_time_s
	uint16_t alarm_rebuild_time_s;
		
}rtu_config;

void load_config(void);

void execute_config(void);

void shell_config(void);

#endif


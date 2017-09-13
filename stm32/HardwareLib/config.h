/*
 * config.h
 *
 *  Created on: 2017年8月16日
 *      Author: Administrator
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f1xx_hal.h"
#include "hydrologyStack.h"
#include "math.h"


/*
 * 端口映射关系
 */
#define RS2321_UART_HANDLE huart4
#define RS2322_UART_HANDLE huart1
#define RS4851_UART_HANDLE huart2
#define RS4852_UART_HANDLE huart3

typedef struct{
	uint16_t mcuStopTime;
	uint16_t lcdCloseTime;
	uint16_t lcdcloseBkTime;
}sysConfig_t;

typedef enum{
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_14400,
	BAUD_19200,
	BAUD_57600,
	BAUD_115200,
}comBaudRateType;

typedef struct{
	comBaudRateType baudRateRs4851;
	comBaudRateType baudRateRs4852;
	comBaudRateType baudRateRs2321;
	comBaudRateType baudRateRs2322;
}comBaudRate_t;

typedef struct {
	uint8_t  centreStationAddr; //中心站地址
	uint8_t RtuStationAddr[5]; //RTU站地址
	uint16_t passWord;
	uint8_t  rtuType;
	uint16_t  timeAverageInterval;
}upDataArg_t;

typedef struct{
	sysConfig_t    sysConfig;
	comBaudRate_t  comBaudRate;
	upDataArg_t    upDataArg;
}rtuParameter_t;

extern rtuParameter_t rtuParameter;

#endif /* CONFIG_H_ */

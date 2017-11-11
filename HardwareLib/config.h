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
 * 端口操作
 */

#define IO0_OUT_HIGH()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET)
#define IO0_OUT_LOW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET)

#define IO1_OUT_HIGH()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET)
#define IO1_OUT_LOW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_SET)

#define IO2_OUT_HIGH()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET)
#define IO2_OUT_LOW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET)

#define IO3_OUT_HIGH()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)
#define IO3_OUT_LOW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET)



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

	uint8_t timingMsgInterval;//定时报间隔
	uint8_t appendMsgInterval;//加报间隔
	uint8_t rainFallDayStartTime;//间水量日起始时间
	uint16_t sampleTime;
	uint8_t waterLevelSaveInterval;

	uint8_t rainGaugeRatio;//雨量计分辨率
	uint8_t waterLevelRatio;//水位分辨率

	uint8_t rainFallAppendThreshold;//雨量加你报阀值
	float   waterLevalBaseValue;//水位基值
	float   waterLevalAmendBaseValue;//水位修正基值

}upDataArg_t;

typedef struct {
	uint8_t rainGaugePsc;
}rainGaugeParamater_t;

typedef struct{
	sysConfig_t    sysConfig;
	comBaudRate_t  comBaudRate;
	upDataArg_t    upDataArg;
	rainGaugeParamater_t rainGaugeParamater;
}rtuParameter_t;

extern rtuParameter_t rtuParameter;

void load_config_Default(void);

#endif /* CONFIG_H_ */

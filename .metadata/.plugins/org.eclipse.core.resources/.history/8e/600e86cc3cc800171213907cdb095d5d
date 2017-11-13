/*
 * stmPwr.h
 *
 *  Created on: 2017Äê11ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef STMPWR_H_
#define STMPWR_H_

#include "stm32f1xx_hal.h"

#define PWR_VCC_ON()   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET)
#define PWR_VCC_OFF()  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET)

#define PWR_SVCC_ON()  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET)
#define PWR_SVCC_OFF() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET)

#define PWR_NET_ON()    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)
#define PWR_NET_OFF()    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)

typedef enum{
	MCU_PWR_STATE_STOP,
	MCU_PWR_STATE_RUN,
}mcuState_t;

typedef struct{
	uint8_t mcuPwrState;
	uint8_t mcu_sysClk;
}mcuPwrCfg_t;

void APP_PWR_stopMode();
void APP_PWR_runMode();

void APP_PWR_SetAlarmCounter(uint32_t AlarmCounter);

extern mcuPwrCfg_t mcuPwrCfg;

#endif /* STMPWR_H_ */

/*
 * stmPwr.c
 *
 *  Created on: 2017年11月7日
 *      Author: Administrator
 */

#include "include.h"
#include "stmPwr.h"
#include "rtc.h"

mcuPwrCfg_t mcuPwrCfg;

void APP_PWR_SetAlarmCounter(uint32_t AlarmCounter){
    RTC_AlarmTypeDef sAlarm;
    uint32_t counter_alarm = 0;
    uint32_t counter_time = 0;
    RTC_TimeTypeDef stime = {0};

    HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);

    /* Convert time in seconds */
    counter_time = (uint32_t)(((uint32_t)stime.Hours * 3600) + ((uint32_t)stime.Minutes * 60) + ((uint32_t)stime.Seconds));

    counter_alarm = counter_time + AlarmCounter;

    sAlarm.Alarm = RTC_ALARM_A;
    sAlarm.AlarmTime.Hours   = (uint32_t)((counter_alarm / 3600) % 24);
    sAlarm.AlarmTime.Minutes = (uint32_t)((counter_alarm % 3600) / 60);
    sAlarm.AlarmTime.Seconds = (uint32_t)((counter_alarm % 3600) % 60);

    if(HAL_RTC_SetAlarm_IT(&hrtc,&sAlarm,RTC_FORMAT_BIN) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}

void APP_PWR_stopMode(){
	/**关闭 sysTick时钟
	 */
	PWR_VCC_OFF();
	PWR_NET_OFF();
	SysTick->CTRL = 0;

	mcuPwrCfg.mcuPwrState = MCU_PWR_STATE_STOP;
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
}

void APP_PWR_runMode(){

	if(mcuPwrCfg.mcuPwrState == MCU_PWR_STATE_STOP){
		mcuPwrCfg.mcuPwrState = MCU_PWR_STATE_RUN;
		SystemClock_Config();
	}

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	APP_PWR_runMode();
}



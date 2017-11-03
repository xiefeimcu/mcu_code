/*
 * tinyRtu.c
 *
 *  Created on: 2017年11月2日
 *      Author: Administrator
 */

#include "tinyRtu.h"

event_t hevent;
waterInf_t hwater;
uint32_t timeCount = 0;

void TTU_sleep(){
	HAL_PWR_EnterSTANDBYMode();
}

void TTU_execute_event(){

	switch(hevent.timeEvent){
	case TIME_EVENT_NULL:
		break;
	case 	TIME_EVENT_MINUTE:
		break;
	case TIME_EVENT_MINUTE5:
		move_waterInf_timeWindow(&hwater);
		break;
	case TIME_HOUR_HANG:
		break;

	case TIME_DAY_HANG:
		break;
	case TIME_HOUR6_HANG:
		break;
	}
	hevent.timeEvent = TIME_EVENT_NULL;



	switch(hevent.collEvent){
	case COLL_EVENT_NULL:
		break;
	case  COLL_RAINFAIL:
		break;
	}
	hevent.collEvent = COLL_EVENT_NULL;



	switch(hevent.commEvent){
	case COMM_EVENT_NULL:
		break;
	case COMM_SEND_MSG_HANG:
		break;
	}
	hevent.commEvent = COMM_EVENT_NULL;
}


void TTU_main_loop(){
	TTU_execute_event();
	TTU_sleep();
}

/*
 * 雨量计的中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	UBaseType_t uxSavedInterruptStatus;
	static  BaseType_t xHigherPriorityTaskWoken;

	portSET_INTERRUPT_MASK_FROM_ISR();
	if(GPIO_Pin == GPIO_PIN_6){
		trigger_rain_signal(&waterInf);
		hevent.collEvent = COLL_RAINFAIL;
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {

}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc){
	timeCount++;

	if(timeCount == TIME_COUNT_MINUTE){
		hevent.timeEvent = TIME_EVENT_MINUTE;
	}
	else if(imeCount == TIME_COUNT_5MINUTE){
		hevent.timeEvent = TIME_EVENT_5MINUTE;
	}
	else if (imeCount == TIME_COUNT_HOUR){
		hevent.timeEvent = TIME_HOUR_HANG;
	}
	else if (imeCount == TIME_COUNT_6HOUR){
		hevent.timeEvent = TIME_6HOUR_HANG;
	}
	else if (imeCount == TIME_COUNT_DAY){
		hevent.timeEvent = TIME_DAY_HANG;
		timeCount =0;
	}
}



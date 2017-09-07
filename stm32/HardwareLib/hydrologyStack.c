/*
 * hydrologyStack.c
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */
#include "hydrologyStack.h"

static message_t         message;
static messageMainBody_t messageSubject;
static uint16_t          messageSerialNum=0;

/*
 * 清除报文中的要素
 * */
void clear_element_from_message(messageMainBody_t * messageSubject){
	memset((uint8_t)(messageSubject->element),ELEMENT_IDENT_NONE,sizeof(messageSubject->element));
}

/*
 * 生成报文正文
 * */
void create_message_subject(messageMainBody_t * messageSubject) {
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	/*流水号*/
	messageSubject->serialNumH = (messageSerialNum & 0xFF00) >> 8;
	messageSubject->serialNumH = (messageSerialNum & 0x00FF);
	messageSerialNum++;

	/*发报时间*/
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
	messageSubject->sendTime.yearBCD = date.Year;    //YY
	messageSubject->sendTime.minuteBCD = date.Month; //MM
	messageSubject->sendTime.dayBCD = date.Date;     //DD
	messageSubject->sendTime.hoursBCD = time.Hours;   //HH
	messageSubject->sendTime.minuteBCD = time.Minutes;   //mm
	messageSubject->sendTime.secondBCD = time.Seconds;   //ss

	/*地址标识符 TODO 标识符是多少？*/
	messageSubject->identifierAddr = 0;
	/*遥测站地址*/
	messageSubject->RtuStationAddr[0] = 0;
	messageSubject->RtuStationAddr[1] = (rtuParameter.upDataArg.RtuStationAddr
			& 0xFF000000) >> 24;
	messageSubject->RtuStationAddr[2] = (rtuParameter.upDataArg.RtuStationAddr
			& 0x00FF0000) >> 16;
	messageSubject->RtuStationAddr[3] = (rtuParameter.upDataArg.RtuStationAddr
			& 0x0000FF00) >> 8;
	messageSubject->RtuStationAddr[5] = (rtuParameter.upDataArg.RtuStationAddr
			& 0x000000FF);

	/*遥测站分类码*/
	messageSubject->RtuType = rtuParameter.upDataArg.rtu_type;
	/*清除要素*/
	clear_element_from_message(messageSubject);
}

int8_t add_element_to_message(messageMainBody_t * messageSubject,element_t *element) {
	uint8_t i = 0;
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	/*观测时间*/
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
	messageSubject->measureTime.yearBCD = date.Year;     //YY
	messageSubject->measureTime.minuteBCD = date.Month;  //MM
	messageSubject->measureTime.dayBCD = date.Date;      //DD
	messageSubject->measureTime.hoursBCD = time.Hours;   //HH
	messageSubject->measureTime.minuteBCD = time.Minutes;//mm
	messageSubject->measureTime.secondBCD = time.Seconds;//ss

	while ((messageSubject->element[i].dataType == ELEMENT_IDENT_NONE)
			&& (i < MAX_ELEMENT_IN_MESSAGE)) {
		i++;
	}

	if (i == MAX_ELEMENT_IN_MESSAGE - 1) { //要素满了
		return -1;
	} else {
		messageSubject->element[i].elementIdentifier = element->elementIdentifier;
		messageSubject->element[i].value = element->value;
	}
}

/*
 * 上电后调用，初始化报文中的常量
 * */
void init_msg_prg(message_t *message ,messageMainBody_t *messageSubject){
	message->frameStartChar = CT_SOH_ASCLL;
	message->centreStationAddr = rtuParameter.upDataArg.centreStationAddr;

	message->RtuStationAddr[0]= 0;
	message->RtuStationAddr[1]= (rtuParameter.upDataArg.RtuStationAddr & 0xFF000000)>>24;
	message->RtuStationAddr[2]= (rtuParameter.upDataArg.RtuStationAddr & 0x00FF0000)>>16;
	message->RtuStationAddr[3]= (rtuParameter.upDataArg.RtuStationAddr & 0x0000FF00)>>8;
	message->RtuStationAddr[4]= (rtuParameter.upDataArg.RtuStationAddr & 0x000000FF);

	message->pswH= (rtuParameter.upDataArg.password & 0xff00)>>8;
	message->pswL= (rtuParameter.upDataArg.password & 0x00ff);

	/*暂时不赋值，发送前根据实际功能再更改*/
	message->funCode = 0;

	message->messageStartChar = CT_STX;
	message->messageMainBody = messageSubject;
	message->messageStopChar = CT_ETX;
}

<<<<<<< HEAD
void convert_message_ascall(message_t *message,uint8_t *txBuf){
	uint8_t i=0;

	message->

}

void send_messag(){
=======
void send_messag(void){
>>>>>>> 1fd01a9bfa554bcadf7472ca6a87ff370fda988d

}


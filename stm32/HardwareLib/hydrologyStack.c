/*
 * hydrologyStack.c
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */
#include "hydrologyStack.h"
#include "Public.h"

static messageInf_t         message;
static messageMainBody_t messageSubject;
static uint16_t          messageSerialNum=0;

/*
 * 清除报文中的要素
 * */
void clear_element_from_message(messageInf_t *message){
	memset((uint8_t)(message->elementInf),ELEMENT_IDENT_NONE,sizeof(message->elementInf));
}

int8_t add_element(messageInf_t *message, const uint8_t *str, float value,uint8_t dataType) {
	uint8_t i = 0;

	/*获取当前时间作为观测时间*/
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BCD);

	while ((message->elementInf.element[i].dataType == ELEMENT_IDENT_NONE)
			&& (i < MAX_ELEMENT_IN_MESSAGE)) {
		i++;
	}

	if (i == MAX_ELEMENT_IN_MESSAGE - 1) { //要素满了
		return -1;
	} else {
		message->elementInf.element[i].elementIdentifier = str; //要素标识符是一个字符串常量
		message->elementInf.element[i].value = value;
		message->elementInf.element.dataType = dataType;
	}
}
void convert_short_to_ascll(uint16_t num, uint8_t *p){
	convert_byte_to_ascll(num >> 8,p++,p++);
	convert_byte_to_ascll((uint8_t)num ,p++,p++);
	sprintf(p,"%4d",num);
}

void convert_float_to_ascll(float num, uint8_t *p , uint8_t dataType){
	uint8_t z = dataType >> 8;
	uint8_t x = (uint8_t)dataType;
	sprintf(p,"%z.xf",num);
}

/*
 * 均时报
 * */
void creat_timeAverage_message(messageInf_t *message,uint8_t *buf){
	uint8_t i = 0;

	/*
	 * 流水号处理
	 * */
	message->serialNum++;
	sprintf(buf, "%4d", essage->serialNum);
	buf += 4;

	/*
	 * 发报时间处理
	 * */
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BCD);
	sprintf(buf, "%2d", message->elementInf.date.Year);
	sprintf(buf, "%2d", message->elementInf.date.Month);
	sprintf(buf, "%2d", message->elementInf.date.Date);
	sprintf(buf, "%2d", message->elementInf.time.Hours);
	sprintf(buf, "%2d", message->elementInf.time.Minutes);
	buf += 10;

	/*
	 *
	 * */

}

/*
 * 维持报
 * */
void creat_keep_message(messageInf_t *message,uint8_t *buf){

}
/*
 * 定时报
 * */
void creat_timing_mesage(messageInf_t *message,uint8_t *buf){

}
/*
 * 小时报
 * */
void creat_hour_mesage(messageInf_t *message,uint8_t *buf){

}
/*
 * 加报
 * */
void creat_extra_mesage(messageInf_t *message,uint8_t *buf){

}
/*
 * 人工报
 * */
void creat_manMade_message(messageInf_t *message,uint8_t *buf){

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
	messageSubject->RtuStationAddr[1] = (rtuParameter.upDataArg.RtuStationAddr & 0xFF000000) >> 24;
	messageSubject->RtuStationAddr[2] = (rtuParameter.upDataArg.RtuStationAddr & 0x00FF0000) >> 16;
	messageSubject->RtuStationAddr[3] = (rtuParameter.upDataArg.RtuStationAddr & 0x0000FF00) >> 8;
	messageSubject->RtuStationAddr[5] = (rtuParameter.upDataArg.RtuStationAddr & 0x000000FF);

	/*遥测站分类码*/
	messageSubject->RtuType = rtuParameter.upDataArg.rtu_type;
	/*清除要素*/
	clear_element_from_message(messageSubject);
}




/*
 * 上电后调用，初始化报文中的常量
 * */
void init_msg_prg(messageInf_t *message ,messageMainBody_t *messageSubject){
	message->frameStartChar = CT_SOH_ASCLL;
	message->centreStationAddr = rtuParameter.upDataArg.centreStationAddr;

	message->RtuStationAddr[0]= 0;
	message->RtuStationAddr[1]= (rtuParameter.upDataArg.RtuStationAddr & 0xFF000000)>>24;
	message->RtuStationAddr[2]= (rtuParameter.upDataArg.RtuStationAddr & 0x00FF0000)>>16;
	message->RtuStationAddr[3]= (rtuParameter.upDataArg.RtuStationAddr & 0x0000FF00)>>8;
	message->RtuStationAddr[4]= (rtuParameter.upDataArg.RtuStationAddr & 0x000000FF);

	message->pswH= (rtuParameter.upDataArg.password & 0xff00)>>8;
	message->pswL= (rtuParameter.upDataArg.password & 0x00ff);

	message->identifierAndLen = (messageInf_tYPE_DOW << 15);
	message->identifierAndLen |= sizeof (messageMainBody_t);

	message->messageStartChar = CT_STX;
	message->messageMainBody = messageSubject;
	message->messageStopChar = CT_ETX;
}
/*
 *将报文转成ascll，并放入发送缓冲区。
 * */
void convert_message_ascll(messageInf_t *message,uint8_t *txBuf){
	uint8_t i=0;

	txBuf[i++] = message->frameStartChar;//不需要转换

	convert_byte_to_ascll(message->centreStationAddr,(txBuf + i)++,(txBuf + i)++);

	convert_byte_to_ascll(message->RtuStationAddr[0],(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->RtuStationAddr[1],(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->RtuStationAddr[2],(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->RtuStationAddr[3],(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->RtuStationAddr[4],(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->pswH ,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->pswL ,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->funCode,(txBuf + i)++,(txBuf + i)++);

	convert_byte_to_ascll(message->identifierAndLen >> 8,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll((uint8_t)(message->identifierAndLen),(txBuf + i)++,(txBuf + i)++);
	txBuf[i++] =message ->messageStartChar;//不需要转换
	/****************报文正文部分***********************/
	convert_byte_to_ascll(message->messageMainBody->serialNumH,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->messageMainBody->serialNumL,(txBuf + i)++,(txBuf + i)++);

	convert_byte_to_ascll(message->messageMainBody->sendTime.yearBCD,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->messageMainBody->sendTime.monthBCD,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->messageMainBody->sendTime.dayBCD,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->messageMainBody->sendTime.hoursBCD,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->messageMainBody->sendTime.minuteBCD,(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll(message->messageMainBody->sendTime.secondBCD,(txBuf + i)++,(txBuf + i)++);
	txBuf[i++]='S';
	txBuf[i++]='T';

	txBuf[i++]=' ';
	convert_byte_to_ascll(message->messageMainBody->identifierAddr,(txBuf + i)++,(txBuf + i)++);
	/****************报文正文部分END***********************/

	txBuf[i++] =message ->messageStopChar;//不需要转换
	convert_byte_to_ascll((message->crc >> 8),(txBuf + i)++,(txBuf + i)++);
	convert_byte_to_ascll((uint8_t)(message->crc),(txBuf + i)++,(txBuf + i)++);
}

void send_messag(void){

}


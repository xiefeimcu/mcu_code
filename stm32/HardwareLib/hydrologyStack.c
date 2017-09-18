/*
 * hydrologyStack.c
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */
#include "include.h"

txBuf_t       txDataBuf;
messageInf_t  messageHandle;

uint16_t getLen_of_txBuf(void){
	return (txDataBuf.dataIdx + 1);
}

uint8_t *get_addr_txBuf(void){
	return (uint8_t*)(txDataBuf.dataBuf);
}

void clear_txBuf(void){
	memset(txDataBuf.dataBuf,0,TX_BUF_LEN);
	txDataBuf.dataIdx = 0;
}

void clear_tail(uint16_t len){
	txDataBuf.dataIdx -= len;
}

void push_char_to_txBuf(uint8_t ch){
	txDataBuf.dataBuf[txDataBuf.dataIdx] = ch;
	txDataBuf.dataIdx++;
}

void push_integer_to_txBuf(uint32_t num , uint8_t dataType){
	uint8_t str[]={"%0ald"};

	str[2] = GET_HIGH_4BIT(dataType) + '0';

	sprintf((char*)(txDataBuf.dataBuf  + txDataBuf.dataIdx),(const char*)str,num);
	txDataBuf.dataIdx += GET_HIGH_4BIT(dataType);
}

void push_float_to_txBuf(float num,uint8_t dataType){
	uint8_t str[]={"%a.bf"};

	str[1] = GET_HIGH_4BIT(dataType) + '0';
	str[3] = GET_LOW_4BIT(dataType) +  '0';
	sprintf((char*)(txDataBuf.dataBuf  + txDataBuf.dataIdx),(const char *)str,num);
	txDataBuf.dataIdx += (GET_HIGH_4BIT(dataType) +  GET_LOW_4BIT(dataType) + 1);
}

void push_data_to_txBuf(uint8_t *srcData,uint16_t len){
	uint8_t i;

	for(i=0;i<len;i++){
		txDataBuf.dataBuf[txDataBuf.dataIdx] = srcData[i];
		txDataBuf.dataIdx++;
	}
}
/*
 * 清除报文中的要素
 * idx：需要清除的要素的小标，为负数则全部清零；
 */
void clear_element_from_message(messageInf_t *message,int8_t idx){
	uint8_t i;

	if(idx<0){
		for(i=0;i<MAX_ELEMENT_IN_MESSAGE;i++){
			message->elementInf.element[i].dataType=ELEMENT_IDENT_NONE;
			message->elementInf.element[i].value=0;
			message->elementInf.element[i].elementIdentifier=NULL;
		}
	}
	else{
		message->elementInf.element[idx].dataType=ELEMENT_IDENT_NONE;
		message->elementInf.element[idx].value=0;
		message->elementInf.element[idx].elementIdentifier=NULL;
	}
}

int8_t add_element(messageInf_t *message,const char* str, float value,uint8_t dataType) {
	uint8_t i = 0;

	/*获取当前时间作为观测时间*/
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BCD);

	while ((message->elementInf.element[i].dataType != ELEMENT_IDENT_NONE) && (i < MAX_ELEMENT_IN_MESSAGE)) {
		i++;
	}

	if (i > MAX_ELEMENT_IN_MESSAGE -1 ) { //要素满了
		return -1;
	} else {
		message->elementInf.element[i].elementIdentifier = (uint8_t*)str; //要素标识符是一个字符串常量
		message->elementInf.element[i].value = value;
		message->elementInf.element[i].dataType = dataType;
	}
	return 0;
}

/*
 * 生成报文正文的公共部分到发送buf
 * */
static void creat_public_message(messageInf_t *message){
	uint8_t i;
	/*
	 * 流水号处理
	 */
	push_integer_to_txBuf(message->serialNum++,N(4,0));

	/*
	 * 发报时间处理
	 */
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BCD);
	push_integer_to_txBuf(message->elementInf.date.Year,N(2,0));
	push_integer_to_txBuf(message->elementInf.date.Month,N(2,0));
	push_integer_to_txBuf(message->elementInf.date.Date,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Hours,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Minutes,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Seconds,N(2,0));

	/*
	 * 地址标识符
	 */
	push_char_to_txBuf('S');
	push_char_to_txBuf('T');
	push_char_to_txBuf(' ');

	/*
	 * 测站地址
	 */
	for(i=0;i<sizeof(rtuParameter.upDataArg.RtuStationAddr);i++){
		push_integer_to_txBuf(rtuParameter.upDataArg.RtuStationAddr[i],N(2,0));
	}
	push_char_to_txBuf(' ');

	/*
	 * 遥测站分类码
	 */
	push_char_to_txBuf(rtuParameter.upDataArg.rtuType);
	push_char_to_txBuf(' ');

	/*
	 * 观测时间标识符
	 */
	push_char_to_txBuf('T');
	push_char_to_txBuf('T');
	push_char_to_txBuf(' ');

	/*
	 *观测时间
	 */
	push_integer_to_txBuf( message->elementInf.date.Year,N(2,0));
	push_integer_to_txBuf( message->elementInf.date.Month,N(2,0));
	push_integer_to_txBuf( message->elementInf.date.Date,N(2,0));
	push_integer_to_txBuf( message->elementInf.time.Hours,N(2,0));
	push_integer_to_txBuf( message->elementInf.time.Minutes,N(2,0));
	push_char_to_txBuf(' ');

}

/*
 * 添加要素到发送buf
 */
static void creat_element_message(messageInf_t *message) {
	uint8_t i=0;
	uint8_t j=0;

	while (message->elementInf.element[i].dataType != ELEMENT_IDENT_NONE) {

		/*复制标识符信息*/
		while(message->elementInf.element[i].elementIdentifier[j]){
			push_char_to_txBuf(message->elementInf.element[i].elementIdentifier[j]);
			j++;
		}
		push_char_to_txBuf(' ');

		/*
		 * 处理数据
		 */
		push_float_to_txBuf(message->elementInf.element[i].value,
				message->elementInf.element[i].dataType);
		push_char_to_txBuf(' ');

		i++;
	}
}

/*
 * 均时报
 * */
static uint16_t creat_timeAverage_message(messageInf_t *message) {
	uint16_t lenBefor = getLen_of_txBuf();
	/*
	 * 增加公共部分
	 */
	creat_public_message(message);

	/*
	 * 时间步长
	 */
	push_char_to_txBuf('D');
	push_char_to_txBuf('R');

	if (rtuParameter.upDataArg.timeAverageInterval < 60) {        //分钟
		push_char_to_txBuf('N');
		push_char_to_txBuf(' ');
		push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval,N(2,0));
	} else if (rtuParameter.upDataArg.timeAverageInterval >= 1440) { //天
		push_char_to_txBuf('D');
		push_char_to_txBuf(' ');
		push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 1440 ,N(2,0));

	} else {
		push_char_to_txBuf('H');
		push_char_to_txBuf(' ');
		push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 60,N(2,0));
	}
	push_char_to_txBuf(' ');

	/*
	 * 要素标识符、数据
	 */
	creat_element_message(message);

	return getLen_of_txBuf() - lenBefor;
}

/*
 * 维持报
 * */
static uint16_t creat_keep_message(messageInf_t *message) {
	uint16_t lenBefor = getLen_of_txBuf();

	/*
	 * 流水号处理
	 */
	push_integer_to_txBuf(message->serialNum++,N(4,0));
	push_char_to_txBuf(' ');

	/*
	 * 发报时间处理
	 */
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BCD);
	push_integer_to_txBuf(message->elementInf.date.Year,N(2,0));
	push_integer_to_txBuf(message->elementInf.date.Month,N(2,0));
	push_integer_to_txBuf(message->elementInf.date.Date,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Hours,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Minutes,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Seconds,N(2,0));
	push_char_to_txBuf(' ');

	return getLen_of_txBuf() - lenBefor;
}

/*
 * 定时报
 * */
static uint16_t creat_timing_mesage(messageInf_t *message) {
	uint16_t lenBefor = getLen_of_txBuf();

	/*
	 * 增加公共部分
	 */
	creat_public_message(message);

	/*
	 * 均时报间隔
	 */
	push_char_to_txBuf('D');
	push_char_to_txBuf('R');


	if (rtuParameter.upDataArg.timeAverageInterval < 60) {        //分钟
		push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval,N(2,0));
		push_char_to_txBuf('N');
		push_char_to_txBuf(' ');
	} else if (rtuParameter.upDataArg.timeAverageInterval >= 1440) { //天
		push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 1440,N(2,0));
		push_char_to_txBuf('D');
		push_char_to_txBuf(' ');
	} else {                                                      //小时
		push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 60,N(2,0));
		push_char_to_txBuf('H');
		push_char_to_txBuf(' ');
	}

	/*要素标识符、数据*/
	creat_element_message(message);
	push_char_to_txBuf(' ');

	/*添加电压信息*/
	push_char_to_txBuf('V');
	push_char_to_txBuf('T');
	push_char_to_txBuf(' ');
	push_float_to_txBuf(message->rtu_state.batteryVoltage, N(2, 2));
	push_char_to_txBuf(' ');
	return getLen_of_txBuf() - lenBefor;
}
/*
 * 小时报
 * */
static uint16_t creat_hour_mesage(messageInf_t *message) {
	uint16_t lenBefor = getLen_of_txBuf();

	/*增加公共部分*/
	creat_public_message(message);

	/*要素: 1h内每5min时段降水量、降水量累计值、1h内每5min时段间隔相对水位*/
	creat_element_message(message);

	/*
	 * 添加电压信息
	 */
	push_char_to_txBuf('V');
	push_char_to_txBuf('T');
	push_char_to_txBuf(' ');
	push_float_to_txBuf(message->rtu_state.batteryVoltage,N(2, 2));
	push_char_to_txBuf(' ');

	return getLen_of_txBuf() - lenBefor;
}

/*
 * 加报
 * */
static uint16_t creat_extra_mesage(messageInf_t *message) {
	uint16_t lenBefor = getLen_of_txBuf();

	/*增加公共部分*/
	creat_public_message(message);

	/* 触发要素 */
	creat_element_message(message);

	/*
	 * 添加电压信息
	 */
	push_char_to_txBuf('V');
	push_char_to_txBuf('T');
	push_char_to_txBuf(' ');
	push_float_to_txBuf(message->rtu_state.batteryVoltage, N(2,2));
	push_char_to_txBuf(' ');

	return getLen_of_txBuf() - lenBefor;
}


/*
 * 人工报
 * */
static uint16_t creat_manMade_message(messageInf_t *message) {
	uint16_t lenBefor = getLen_of_txBuf();

	push_integer_to_txBuf(message->serialNum++,N(2,0));

	/*
	 * 发报时间处理
	 */
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BCD);
	push_integer_to_txBuf(message->elementInf.date.Year,N(2,0));
	push_integer_to_txBuf(message->elementInf.date.Month,N(2,0));
	push_integer_to_txBuf(message->elementInf.date.Date,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Hours,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Minutes,N(2,0));
	push_integer_to_txBuf(message->elementInf.time.Seconds,N(2,0));
	push_char_to_txBuf(' ');

	/*
	 * 人工置数标识符，人工置数。
	 */
	//TODO 等待补充
	return getLen_of_txBuf() - lenBefor;
}

/*
 * 调用此函数生成水文数据帧
 * 参数funCode表明帧的类型
 *
 * */
void creat_msg(messageInf_t *message,  uint8_t funCode) {
	uint8_t i = 0;
	uint16_t len=0;

	clear_txBuf();

	/* 帧启始符*/
	push_char_to_txBuf(CT_SOH_ASCLL);

	/*中心站地址*/
	push_integer_to_txBuf(rtuParameter.upDataArg.centreStationAddr,N(2,0));

	/*
	 * 测站地址
	 */
	for (i = 0; i < sizeof(rtuParameter.upDataArg.RtuStationAddr); i++) {
		push_integer_to_txBuf(rtuParameter.upDataArg.RtuStationAddr[i],N(2,0));
	}

	/*密码*/
	push_integer_to_txBuf(rtuParameter.upDataArg.passWord,N(4,0));

	/*功能码*/
	push_integer_to_txBuf(funCode,N(2,0));

	/*上下行标识符*/
	push_char_to_txBuf(MESSAGE_TYPE_UP);

	switch (funCode) {
	/*
	 * 均时段
	 */
	case FUN_CODE_JYSD:
		/*
		 * 先计得到正文长度
		 * 添加长度标识
		 */
		len = creat_timeAverage_message(message);
		clear_tail(len);
		push_integer_to_txBuf(len, N(3, 0));

		/*报文启始符*/
		push_char_to_txBuf(CT_STX);

		/*添加正文*/
		creat_timeAverage_message(message);
		break;

		/*
		 * 小时报
		 * */
	case FUN_CODE_XSB:
		/*
		 * 先计得到正文长度
		 * 添加长度标识
		 */
		len = creat_hour_mesage(message);
		clear_tail(len);
		push_integer_to_txBuf(len, N(3, 0));

		/*报文启始符*/
		push_char_to_txBuf(CT_STX);

		/*添加正文*/
		creat_hour_mesage(message);
		break;

		/*
		 * 定时报
		 * */
	case FUN_CODE_DSB:
		/*
		 * 先计得到正文长度
		 * 添加长度标识
		 */
		len = creat_timing_mesage(message);
		clear_tail(len);
		push_integer_to_txBuf(len, N(3, 0));

		/*报文启始符*/
		push_char_to_txBuf(CT_STX);

		/*添加正文*/
		creat_timing_mesage(message);
		break;

		/*
		 * 链路维持报
		 */
	case FUN_CODE_LLWC:
		/*
		 * 先计得到正文长度
		 * 添加长度标识
		 */
		len = creat_keep_message(message);
		clear_tail(len);
		push_integer_to_txBuf(len, N(3, 0));

		/*报文启始符*/
		push_char_to_txBuf(CT_STX);

		/*添加正文*/
		creat_keep_message(message);
		break;

		/*
		 * 加报
		 */
	case FUN_CODE_JBB:
		/*
		 * 先计得到正文长度
		 * 添加长度标识
		 */
		len = creat_extra_mesage(message);
		clear_tail(len);
		push_integer_to_txBuf(len, N(3, 0));

		/*报文启始符*/
		push_char_to_txBuf(CT_STX);

		/*添加正文*/
		creat_extra_mesage(message);
		break;

		/*
		 * 人工置数报
		 */
	case FUN_CODE_RGZS:
		/*
		 * 先计得到正文长度
		 * 添加长度标识
		 */
		len = creat_manMade_message(message);
		clear_tail(len);
		push_integer_to_txBuf(len, N(3, 0));

		/*报文启始符*/
		push_char_to_txBuf(CT_STX);

		/*添加正文*/
		creat_manMade_message(message);
		break;

	default:
		break;
	}

	/*报文结束符*/
	push_char_to_txBuf(CT_ETX);

	/*求校验*/
	push_integer_to_txBuf(CRC16(get_addr_txBuf(),getLen_of_txBuf()) ,N(4,0));
}

/*
 * hydrologyStack.c
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */
#include "include.h"

txBuf_t txDataBuf;
messageInf_t messageHandle;
hydrologyInf_t hydrologyInf;

const char* identifierCode[]={
		"TT",
		"ST",
		"RGZS",
		"PIC",
		"DRP",
		"DRZ1",
		"DRZ2",
		"DRZ3",
		"DRZ4",
		"DRZ5",
		"DRZ6",
		"DRZ7",
		"DRZ8",
		"DATA",
		"AC",
		"AI",
		"C",
		"DRxnn",
		"DT",
		"ED",
		"EJ",
		"FL",
		"GH",
		"GN",
		"GS",
		"GT",
		"GTP",
		"H",
		"HW",
		"M10",
		"M20",
		"M30",
		"M40",
		"M50",
		"M60",
		"M80",
		"M100",
		"MST",
		"NS",
		"P1",
		"P2",
		"P3",
		"P6",
		"P12",
		"PD",
		"PJ",
		"PN01",
		"PN05",
		"PN10",
		"PN30",
		"PR",
		"PT",
		"Q",
		"Q1",
		"Q2",
		"Q3",
		"Q4",
		"Q5",
		"Q6",
		"Q7",
		"Q8",
		"QA",
		"QZ",
		"SW",
		"UC",
		"UE",
		"US",
		"VA",
		"VJ",
		"VT",
		"Z",
		"ZB",
		"ZU",
		"Z1",
		"Z2",
		"Z3",
		"Z4",
		"Z5",
		"Z6",
		"Z7",
		"Z8",
		"SQ",
		"ZT",
		"pH",
		"DO",
		"COND",
		"TURB",
		"CODMN",
		"REDOX",
		"NH4N",
		"TP",
		"TN",
		"TOC",
		"CU",
		"ZN",
		"SE",
		"AS",
		"THG",
		"CD",
		"PB",
		"CHLA",
		"WP1",
		"WP2",
		"WP3",
		"WP4",
		"WP5",
		"WP6",
		"WP7",
		"WP8",
		"SYL1",
		"SYL2",
		"SYL3",
		"SYL4",
		"SYL5",
		"SYL6",
		"SYL7",
		"SYL8",
		"SBL1",
		"SBL2",
		"SBL3",
		"SBL4",
		"SBL5",
		"SBL6",
		"SBL7",
		"SBL8",
		"VTA",
		"VTB",
		"VTC",
		"VIA",
		"VIB",
		"VIC",
};

uint16_t get_txbuf_len(void) {
	return (txDataBuf.dataIdx );
}

uint8_t *get_txbuf_addr(void) {
	return (uint8_t*) (txDataBuf.dataBuf);
}

void clear_txBuf(void) {
	memset(txDataBuf.dataBuf, 0, TX_BUF_LEN);
	txDataBuf.dataIdx = 0;
}

void clear_tail(uint16_t len) {
	txDataBuf.dataIdx -= len;
	memset((uint8_t*) (txDataBuf.dataBuf + txDataBuf.dataIdx), 0, len);

}

void push_one_byte_to_txBuf(uint8_t ch) {
	txDataBuf.dataBuf[txDataBuf.dataIdx] = ch;
	txDataBuf.dataIdx++;
}

void push_string_to_txBuf(const char* str){
	uint8_t i=0;

	for(i=0;str[i];i++){
		push_one_byte_to_txBuf(str[i]);
	}
}

void push_hex_integer_to_txBuf(uint32_t num, uint8_t byteCount){
	uint8_t i;

	typedef union {
		uint32_t num;
		uint8_t  byte[4];
	}unionData;

	unionData data;

	data.num = num;

	for(i=0;i<byteCount;i++){
		txDataBuf.dataBuf[txDataBuf.dataIdx] = data.byte[i];
		txDataBuf.dataIdx++;
	}
}

void push_hex_data(uint8_t *pdata,uint16_t len){
	memcpy(get_txbuf_addr(),pdata,len);
}

void push_integer_to_txBuf(uint32_t num, uint8_t dataType,uint8_t isHexOrDec) {
	if(isHexOrDec){
		sprintf((char*) (txDataBuf.dataBuf + txDataBuf.dataIdx), "%0*lx",GET_HIGH_4BIT(dataType), num);
		txDataBuf.dataIdx += GET_HIGH_4BIT(dataType);
	}else{
		sprintf((char*) (txDataBuf.dataBuf + txDataBuf.dataIdx), "%0*ld",GET_HIGH_4BIT(dataType), num);
		txDataBuf.dataIdx += GET_HIGH_4BIT(dataType);
	}
}

void clear_hydrologyInf(hydrologyInf_t *hydrologyData) {
	hydrologyInf.timeIdx = 0;
	memset(hydrologyData->rainfall.rainfallPer12Minute, 0,COUNT_OF_12MINUTE_IN_HOUR);

	memset(hydrologyData->waterLevel.waterLevelPer12Minute, 0,COUNT_OF_12MINUTE_IN_HOUR);
}
/*
 * 每隔5分钟调用一次
 */
void add_waterInf_data(hydrologyInf_t *hydrologyData,uint8_t rainFall, uint16_t waterLevel) {
	if (hydrologyData->timeIdx >= COUNT_OF_12MINUTE_IN_HOUR) {
		clear_hydrologyInf(hydrologyData);
	}

	hydrologyData->rainfall.rainfallPer12Minute[hydrologyInf.timeIdx] = rainFall;
	hydrologyData->waterLevel.waterLevelPer12Minute[hydrologyInf.timeIdx] =waterLevel;
	hydrologyData->timeIdx++;
}
/*
 * 测试用代码
 */
void add_sim_waterInf_data(hydrologyInf_t *hydrologyData) {
	uint8_t i;
	clear_hydrologyInf(hydrologyData);

	for (i = 200; i < 200 + COUNT_OF_12MINUTE_IN_HOUR; i++) {
		add_waterInf_data(hydrologyData, i, 10 * i);
	}
}

uint32_t power(float x, uint16_t y) {
	const uint32_t powerCode[] = { 0, 10, 100, 1000, 10000 };

	if (y > 4) {
		return -1;
	}

	return (uint32_t) ((float)powerCode[y] * x);
}

uint8_t get_DecNum_chars_count(uint32_t num){
	uint8_t i;
	const uint32_t code[] = { 0, 10, 100, 1000, 10000, 100000, 1000000,
			10000000, 100000000, 1000000000,};

	for(i=0;i<sizeof(code);i+=2){
		if((num >= code[i]) && (num < code[i+1])){
			return i + 1;
		}
	}
	return 0;
}



void push_ascll_float_to_txBuf(float num, uint8_t dataType) {
	uint32_t intArea;
	float decimalArea;
	uint32_t intDecimal;

	if (num < 0) {
		push_one_byte_to_txBuf('-');
		num = -1 * num;
	}

	intArea = (uint32_t) num;
	decimalArea = num - (float)intArea;
	intDecimal = power(decimalArea, GET_LOW_4BIT(dataType));

	sprintf((char*) (txDataBuf.dataBuf + txDataBuf.dataIdx), "%ld", intArea);
	txDataBuf.dataIdx += get_DecNum_chars_count(intArea);

	push_one_byte_to_txBuf('.');

	sprintf((char*) (txDataBuf.dataBuf + txDataBuf.dataIdx), "%ld", intDecimal);
	txDataBuf.dataIdx += get_DecNum_chars_count(intDecimal);
}

void push_ascll_data_to_txBuf(uint8_t *srcData, uint16_t len) {
	uint8_t i;

	for (i = 0; i < len; i++) {
		txDataBuf.dataBuf[txDataBuf.dataIdx] = srcData[i];
		txDataBuf.dataIdx++;
	}
}
/*
 * 清除报文中的要素
 * idx：需要清除的要素的小标，为负数则全部清零；
 */
void clear_element_from_message(messageInf_t *message, int8_t idx) {
	uint8_t i;

	if (idx < 0) {
		for (i = 0; i < MAX_ELEMENT_IN_MESSAGE; i++) {
			message->elementInf.element[i].dataType = ELEMENT_IDENT_NONE;
			message->elementInf.element[i].value = 0;
		}
	} else {
		message->elementInf.element[idx].dataType = ELEMENT_IDENT_NONE;
		message->elementInf.element[idx].value = 0;
	}
}

/*
 * 添加报文启始符 区分M3 模式
 */

void add_control_symbol(uint16_t ct){
	/*ASCLL HEX 的 SOH 不同*/
	if(ct == CT_SOH){
		if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
			push_hex_integer_to_txBuf(CT_SOH_HEX_BCD,2);
		}else{
			push_one_byte_to_txBuf(CT_SOH_ASCLL);
		}

	}else{
		/*控制符有的是一byte 有的是两 byte*/
		if(ct > 0xFF){
			push_hex_integer_to_txBuf(ct,2);
		}else{
			push_one_byte_to_txBuf(ct);
		}
	}
}

void add_rtu_addr(void) {
	uint8_t i;
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		for (i = 0; i < 5; i++) {
			push_one_byte_to_txBuf(rtuParameter.upDataArg.RtuStationAddr[i]);
		}

	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		for (i = 0; i < 5; i++) {
			push_integer_to_txBuf(rtuParameter.upDataArg.RtuStationAddr[i], N(2, 0),CDEC_ASCII);
		}
		push_one_byte_to_txBuf(' ');
	}
}

void add_station_addr(void) {
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_one_byte_to_txBuf(rtuParameter.upDataArg.centreStationAddr);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_integer_to_txBuf(rtuParameter.upDataArg.centreStationAddr, N(2, 0),CDEC_ASCII);
	}
}

void add_paswd(void) {
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_hex_integer_to_txBuf(rtuParameter.upDataArg.passWord , sizeof(rtuParameter.upDataArg.passWord));
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_integer_to_txBuf(rtuParameter.upDataArg.passWord, N(4, 0), CDEC_ASCII);
	}
}

void add_fun_code(uint8_t funCode) {
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_one_byte_to_txBuf(funCode);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_integer_to_txBuf(funCode, N(2, 0), CHEX_ASCII);
	}
}

void add_up_down_len_identifier(uint16_t SerialNum,uint16_t len){
	uint32_t temp;
	/*M3*/
	if (COMM_MODE == COMM_MODE_M3) {
		temp =( len << 11 ) | SerialNum;
		if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
			push_hex_integer_to_txBuf(temp,3);
		} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
			push_integer_to_txBuf(len, N(3, 0), CHEX_ASCII);
			push_integer_to_txBuf(SerialNum, N(3, 0), CHEX_ASCII);
		}
	}
	/*M1 M2 M4*/
	else {
		/*上行标识*/
		len &= ~0x800;

		if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
			push_hex_integer_to_txBuf(len, sizeof(len));
		} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
			push_integer_to_txBuf(len, N(4,0), CHEX_ASCII);
		}
	}
}

void add_serial_num(uint16_t num){
	if(MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX){
		push_hex_integer_to_txBuf(num,sizeof(num));
	}else if(MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII){
		push_integer_to_txBuf(num,N(4,0),CDEC_ASCII);
	}
}

void add_rtu_type_code (uint8_t ch){
	push_one_byte_to_txBuf(ch);

    if(MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII){
    	push_one_byte_to_txBuf(' ');
	}


}

void add_watch_time(RTC_TimeTypeDef time, RTC_DateTypeDef date) {
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_integer_to_txBuf(date.Year, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(date.Month, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(date.Date, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(time.Hours, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(time.Minutes, N(2, 0), CDEC_ASCII);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_one_byte_to_txBuf('T');
		push_one_byte_to_txBuf('T');
		push_one_byte_to_txBuf(' ');
		push_integer_to_txBuf(date.Year, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(date.Month, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(date.Date, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(time.Hours, N(2, 0), CDEC_ASCII);
		push_integer_to_txBuf(time.Minutes, N(2, 0), CDEC_ASCII);
		push_one_byte_to_txBuf(' ');
	}


}

void add_now_time(void) {
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	push_integer_to_txBuf(date.Year, N(2, 0),  CDEC_ASCII);
	push_integer_to_txBuf(date.Month, N(2, 0), CDEC_ASCII);
	push_integer_to_txBuf(date.Date, N(2, 0),  CDEC_ASCII);
	push_integer_to_txBuf(time.Hours, N(2, 0), CDEC_ASCII);
	push_integer_to_txBuf(time.Minutes, N(2, 0), CDEC_ASCII);
	push_integer_to_txBuf(time.Seconds, N(2, 0), CDEC_ASCII);
}

void add_msg_end_identifier(uint8_t ch){
	push_one_byte_to_txBuf(ch);
}

void add_voltage(float voltage){
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_one_byte_to_txBuf(0x38);
		push_one_byte_to_txBuf(0x12);
		push_integer_to_txBuf((uint32_t) voltage,N(2,2),CDEC_ASCII);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_one_byte_to_txBuf('V');
		push_one_byte_to_txBuf('T');
		push_one_byte_to_txBuf(' ');
		push_ascll_float_to_txBuf(voltage,N(2,2));
		push_one_byte_to_txBuf(' ');
	}
}

void add_time_step_identifier(void){
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_one_byte_to_txBuf(0x04);
		push_one_byte_to_txBuf(0x18);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_one_byte_to_txBuf('D');
		push_one_byte_to_txBuf('R');
	}
}

void add_time_step(void){
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		if (rtuParameter.upDataArg.timeAverageInterval < 60) {//分钟
			push_one_byte_to_txBuf(0);
			push_one_byte_to_txBuf(0);
			push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval,N(2, 0),CDEC_ASCII);
		} else if (rtuParameter.upDataArg.timeAverageInterval >= 1440) { //天
			push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 1440,N(2, 0),CDEC_ASCII);
			push_one_byte_to_txBuf(0);
			push_one_byte_to_txBuf(0);
		} else {
			push_one_byte_to_txBuf(0);
			push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 60,N(2, 0),CDEC_ASCII);
			push_one_byte_to_txBuf(0);
		}
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		if (rtuParameter.upDataArg.timeAverageInterval < 60) {        //分钟
			push_one_byte_to_txBuf('N');
			push_one_byte_to_txBuf(' ');
			push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval,N(2, 0),CDEC_ASCII);
		} else if (rtuParameter.upDataArg.timeAverageInterval >= 1440) { //天
			push_one_byte_to_txBuf('D');
			push_one_byte_to_txBuf(' ');
			push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 1440,N(2, 0),CDEC_ASCII);

		} else {
			push_one_byte_to_txBuf('H');

			push_integer_to_txBuf(rtuParameter.upDataArg.timeAverageInterval / 60,N(2, 0),CDEC_ASCII);
		}
		push_one_byte_to_txBuf(' ');
	}
}

void add_identifier(identifierCodeIdx_t idx){
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_one_byte_to_txBuf(idx + IDENT_CODE_ST);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_string_to_txBuf(identifierCode[idx]);
		push_one_byte_to_txBuf(' ');
	}
}

void add_element_data(float data,uint8_t dataType){
	uint8_t h;
	uint8_t l;
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		h = GET_HIGH_4BIT(dataType);
		l = GET_HIGH_4BIT(dataType);

		push_one_byte_to_txBuf((h << 3) | l );
		push_integer_to_txBuf(data,N((l + h),0),CDEC_ASCII);

	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_ascll_float_to_txBuf(data,dataType);
		push_one_byte_to_txBuf(' ');

	}
}

void add_element(messageInf_t *message) {
	uint8_t i = 0;
	/*遍历要素缓存*/
	for (i = 0; i < MAX_ELEMENT_IN_MESSAGE; i++) {
		if (message->elementInf.element[i].dataType == ELEMENT_IDENT_NONE){
			break;
		}
		add_identifier(message->elementInf.element[i].elementIdentifier);
		add_element_data(message->elementInf.element[i].value,message->elementInf.element[i].dataType);
	}
}

/*
 * 用于处理小时报中 雨量 水位信息到buf
 */
static void add_hour_msg_data(hydrologyInf_t *hydrologyData){
	uint8_t i=0;
	/*先是标识符*/
	add_identifier(IDT_DRP);
	/*1h 内 间隔5 分钟 的降水量*数据*/
	for(i=0;i < COUNT_OF_12MINUTE_IN_HOUR;i++){
		push_integer_to_txBuf(hydrologyData->waterLevel.waterLevelPer12Minute[i],N(2,0),CHEX_ASCII);
	}

	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {//ASCLL 模式下补空格
		push_one_byte_to_txBuf(' ');
	}

	/*降水量累计值标识符*/
	add_identifier(IDT_PT);
	/*降水量累计值数据处理*/
	add_element_data(hydrologyData->rainfall.rainfallTotal,N(2,0));

	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {//ASCLL 模式下补空格
		push_one_byte_to_txBuf(' ');
	}

	/*
	 * 1 小时内 5 分钟间隔相对水位 1(每组水位占 2 字节
     * HEX ，分辨力是为厘米，最大值为 655.34 米，数
     * 据中不含小数点； FFFFH表示非法数据 )； 对于河道、
     * 闸坝（泵） 站分别表示河道水位、 闸（站）上水位
	 */

	/*1h 内 间隔5 分钟 的标识符据*/
	add_identifier(IDT_DRZ1);
	/*1h 内 间隔5 分钟 的水位数据*/
	for(i=0;i<COUNT_OF_12MINUTE_IN_HOUR;i++){
		push_integer_to_txBuf(hydrologyData->waterLevel.waterLevelPer12Minute[i],N(4,0),CHEX_ASCII);
	}

	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {//ASCLL 模式下补空格
		push_one_byte_to_txBuf(' ');
	}
}

void add_crc(void) {
	if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_HEX) {
		push_hex_integer_to_txBuf(CRC16(get_txbuf_addr(), get_txbuf_len()),2);
	} else if (MESSAGE_DATA_FORMAT == MESSAGE_DATA_FORMAT_ASCII) {
		push_integer_to_txBuf(CRC16(get_txbuf_addr(), get_txbuf_len()),N(4, 0), CHEX_ASCII);
	}
}

/*
 * 帧的公共部分
 */
void creat_frame_public(void){
	add_control_symbol(CT_SOH);
	add_station_addr();
	add_rtu_addr();
	add_paswd();
}

/*
 * 报文的公共部分
 */

void creat_msg_public(messageInf_t *message){
	add_serial_num(message->serialNum++);
	add_now_time();
	add_identifier(IDT_ST);
	add_rtu_addr();
	add_rtu_type_code(rtuParameter.upDataArg.rtuType);
	add_watch_time(message->elementInf.time , message->elementInf.date);
}

int8_t write_element(messageInf_t *message, identifierCodeIdx_t ident, float value , uint8_t dataType) {
	uint8_t i = 0;

	/*获取当前时间作为观测时间*/
	HAL_RTC_GetTime(&hrtc, &(message->elementInf.time), RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &(message->elementInf.date), RTC_FORMAT_BIN);

	while ((message->elementInf.element[i].dataType != ELEMENT_IDENT_NONE)&& (i < MAX_ELEMENT_IN_MESSAGE)) {
		i++;
	}

	if (i > MAX_ELEMENT_IN_MESSAGE - 1) { //要素满了
		return -1;
	} else {
		message->elementInf.element[i].elementIdentifier = ident; //要素标识符是一个字符串常量
		message->elementInf.element[i].value =  value;
		message->elementInf.element[i].dataType = dataType;
	}
	return 0;
}


/*
 * 均时报
 * */
static uint16_t creat_timeAverage_message(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();

	creat_msg_public(message);
	/*时间步长*/
	add_time_step_identifier();
	add_time_step();

	add_element(message);
	return get_txbuf_len() - lenBefor;
}

/*
 * 维持报
 * */
static uint16_t creat_keep_message(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();
	/*流水号处理 */
	add_serial_num(message->serialNum);
	/*发报时间处理*/
	add_now_time();
	return get_txbuf_len() - lenBefor;
}

/*
 * 定时报
 * */
static uint16_t creat_timing_mesage(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();

	/*增加公共部分*/
	creat_msg_public(message);

	/*要素标识符、数据*/
	add_element(message);

	/*添加电压信息*/
	add_voltage(message->rtu_state.batteryVoltage);
	return get_txbuf_len() - lenBefor;
}

/*
 * 小时报
 * */
static uint16_t creat_hour_mesage(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();

	/*增加公共部分*/
	creat_msg_public(message);
	/*要素: 1h内每5min时段降水量、降水量累计值、1h内每5min时段间隔相对水位 TODO*/
	add_hour_msg_data(&hydrologyInf);
	/* 添加电压信息*/
	add_voltage(message->rtu_state.batteryVoltage);

	return get_txbuf_len() - lenBefor;
}

/*
 * 加报
 * */
static uint16_t creat_extra_mesage(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();

	/*增加公共部分*/
	creat_msg_public(message);
	/* 触发要素 */
	add_element(message);
	/* 添加电压信息*/
	add_voltage(message->rtu_state.batteryVoltage);
	return get_txbuf_len() - lenBefor;
}

/*
 * 人工报
 * */
static uint16_t creat_manMade_message(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();

	add_serial_num(message->serialNum);
	/*发报时间处理*/
	add_now_time();
	add_identifier(IDT_Q);
	add_element_data(1234,N(1,3));

	return get_txbuf_len() - lenBefor;
}

/*
 * 测试报文
 */

uint16_t creat_test_message(messageInf_t *message) {
	uint16_t lenBefor = get_txbuf_len();

	/*增加公共部分*/
	creat_msg_public(message);
	/* TODO 降水量、水位、其他要素*/
	add_element(message);

	/* 添加电压信息*/
	add_voltage(message->rtu_state.batteryVoltage);
	return get_txbuf_len() - lenBefor;
}

/*
 * 调用此函数生成水文数据帧
 * 参数funCode表明帧的类型
 *
 * */
void creat_msg(messageInf_t *message, uint8_t funCode,uint16_t msgSerial,uint8_t isLastMsg) {
	uint16_t len = 0;
	clear_txBuf();
	creat_frame_public();
	/*功能码*/
	push_integer_to_txBuf(funCode, N(2, 0),CHEX_ASCII);
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
		add_up_down_len_identifier(msgSerial,len);
		/*报文启始符*/
		add_control_symbol(CT_STX);
		/*添加正文*/
		creat_timeAverage_message(message);
		break;

		/*
		 * 小时报
		 * */
	case FUN_CODE_XSB:
		len = creat_hour_mesage(message);
		clear_tail(len);
		add_up_down_len_identifier(msgSerial,len);
		/*报文启始符*/
		add_control_symbol(CT_STX);
		/*添加正文*/
		creat_hour_mesage(message);
		break;

		/*
		 * 定时报
		 * */
	case FUN_CODE_DSB:
		len = creat_timing_mesage(message);
		clear_tail(len);
		add_up_down_len_identifier(msgSerial,len);

		/*报文启始符*/
		add_control_symbol(CT_STX);

		/*添加正文*/
		creat_timing_mesage(message);
		break;

		/*
		 * 链路维持报
		 */
	case FUN_CODE_LLWC:
		len = creat_keep_message(message);
		clear_tail(len);
		add_up_down_len_identifier(msgSerial,len);

		/*报文启始符*/
		add_control_symbol(CT_STX);

		/*添加正文*/
		creat_keep_message(message);
		break;

		/*
		 * 加报
		 */
	case FUN_CODE_JBB:
		len = creat_extra_mesage(message);
		clear_tail(len);

		push_integer_to_txBuf(len, N(3, 0),CHEX_ASCII);
		/*报文启始符*/
		add_control_symbol(CT_STX);
		/*添加正文*/
		creat_extra_mesage(message);
		break;

		/*
		 * 人工置数报
		 */
	case FUN_CODE_RGZS:
		len = creat_manMade_message(message);
		clear_tail(len);


		push_integer_to_txBuf(len, N(3,0),CDEC_ASCII);
		/*报文启始符*/
		add_control_symbol(CT_STX);

		/*添加正文*/
		creat_manMade_message(message);
		break;

	case FUN_CODE_CSB:
		len = creat_test_message(message);
		clear_tail(len);
		add_up_down_len_identifier(msgSerial,len);
		/*报文启始符*/
		add_control_symbol(CT_STX);
		/*添加正文*/
		creat_hour_mesage(message);
		break;

		break;

	default:
		break;
	}

	/*报文结束符*/

	if(isLastMsg){
		add_msg_end_identifier(CT_ETX);
	}
	else{
		add_msg_end_identifier(CT_ETB);
	}

	/*求校验*/
	add_crc();
}

/***********************测试代码********************************/
/*
 * 上行测试报
 * */
void TEST_HYK_test_msg(messageInf_t *message){
	/*添加测试报的要素信息*/
	write_element(&messageHandle,IDT_PJ,1.5,N(5,1));
	write_element(&messageHandle,IDT_PT,1.5,N(6,1));
	write_element(&messageHandle,IDT_Z,0.301,N(7,3));
	/*生成报文*/
	creat_msg(&messageHandle, FUN_CODE_CSB,1,1);
	DEBUG_INF((uint8_t *)"\r\n\r\ntest:",9);
	DEBUG_INF(get_txbuf_addr(),get_txbuf_len());
	/*测试完成后清空要素*/
	clear_element_from_message(message,-1);
}

/*
 * 上行维持报
 * */
void TEST_HYK_keep_msg(messageInf_t *message){
	/*生成报文*/
	creat_msg(&messageHandle, FUN_CODE_LLWC,1,1);
	DEBUG_INF((uint8_t *)"\r\n\r\nkeep:",9);
	DEBUG_INF(get_txbuf_addr(),get_txbuf_len());
}

/*
 * 上行定时报
 * */
void TEST_HYK_timing_msg(messageInf_t *message){
	/*添加定时报的要素信息*/
	write_element(&messageHandle,IDT_PJ,1.5,N(5,1));
	write_element(&messageHandle,IDT_PT,1.5,N(6,1));
	write_element(&messageHandle,IDT_Z,10.58,N(7,3));
	/*生成报文*/
	creat_msg(&messageHandle, FUN_CODE_DSB,1,1);
	DEBUG_INF((uint8_t *)"\r\n\r\ntiming:",11);
	DEBUG_INF(get_txbuf_addr(),get_txbuf_len());
	/*测试完成后清空要素*/
	clear_element_from_message(message,-1);
}

/*
 * 上行小时报
 * */
void TEST_HYK_hour_msg(messageInf_t *message){
	/*添加定时报的要素信息*/
	write_element(&messageHandle,IDT_PJ,1.5,N(5,1));
	write_element(&messageHandle,IDT_PT,1.5,N(6,1));
	write_element(&messageHandle,IDT_Z,10.58,N(7,3));
	/*生成报文*/
	creat_msg(&messageHandle, FUN_CODE_XSB,1,1);
	DEBUG_INF((uint8_t *)"\r\n\r\nhour:",9);
	DEBUG_INF(get_txbuf_addr(),get_txbuf_len());
	/*测试完成后清空要素*/
	clear_element_from_message(message,-1);
}

/*
 * 上行人工报
 * */
void TEST_HYK_man_msg(messageInf_t *message){
	/*生成报文*/
	creat_msg(&messageHandle, FUN_CODE_RGZS,1,1);
	DEBUG_INF((uint8_t *)"\r\n\r\nman:",8);
	DEBUG_INF(get_txbuf_addr(),get_txbuf_len());
}

/*
 * 上行加报
 * */
void TEST_HYK_plus_msg(messageInf_t *message){
	/*添加定时报的要素信息*/
	write_element(&messageHandle,IDT_PJ,1.5,N(5,1));
	write_element(&messageHandle,IDT_PT,1.5,N(6,1));
	write_element(&messageHandle,IDT_Z,10.58,N(7,3));
	/*生成报文*/
	creat_msg(message, FUN_CODE_DSB,1,1);
	DEBUG_INF((uint8_t *)"\r\n\r\nplus:",9);
	DEBUG_INF(get_txbuf_addr(),get_txbuf_len());
	/*测试完成后清空要素*/
	clear_element_from_message(message,-1);
}

/*
 * 上行均时报 TODO 暂时不支持
 * */
void TEST_HYK_timeAverage_msg(messageInf_t *message){

}


/***********************测试代码END*****************************/

uint8_t analysis_msg(uint8_t *rxBuf , uint8_t len){
	return 0;
}

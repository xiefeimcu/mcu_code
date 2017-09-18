/*
 * hydrologyStack.h
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */

#ifndef HYDROLOGYSTACK_H_
#define HYDROLOGYSTACK_H_

#include "stm32f1xx_hal.h"
#include "include.h"

/**************************协议配置**************************************/
#define MAX_ELEMENT_IN_MESSAGE  6
#define TX_BUF_LEN 255
/**************************控制字符定义**************************************/
#define CT_SOH_ASCLL   0x01
#define CT_SOH_HEX_BCD 0x7EE
#define CT_STX         0x02
#define CT_SYN         0x16
#define CT_ETX         0x03
#define CT_ETB         0x17
#define CT_ENQ         0x05
#define CT_EOT         0x04
#define CT_ACK         0x06
#define CT_NAK         0x15
#define CT_ESC         0x1B4
/**************************遥测站分类码***************************************/
#define RTU_TYPE_JS      'P'             //遥测站分类码 降水
#define RTU_TYPE_HD      'H'             //遥测站分类码 河道
#define RTU_TYPE_SK      'K'             //遥测站分类码 水库（湖泊）
#define RTU_TYPE_ZB      'Z'             //遥测站分类码 闸坝
#define RTU_TYPE_BZ      'D'             //遥测站分类码 泵站
#define RTU_TYPE_CX      'T'             //遥测站分类码 潮汐
#define RTU_TYPE_SQ      'M'             //遥测站分类码 墒情
#define RTU_TYPE_DXS     'G'             //遥测站分类码 地下水
#define RTU_TYPE_SZ      'Q'             //遥测站分类码 水质
#define RTU_TYPE_QSK     'I'             //遥测站分类码 取水口
#define RTU_TYPE_PSK     'O'             //遥测站分类码 排水口
/****************************功能码定义***************************************/
#define FUN_CODE_LLWC        0X2F        //链路维持报
#define FUN_CODE_CSB         0X30        //测试报
#define FUN_CODE_JYSD        0X31        //均有时段水文信息报
#define FUN_CODE_DSB         0X32        //遥测站定时报
#define FUN_CODE_JBB         0X33        //遥测站加报报
#define FUN_CODE_XSB         0X34        //遥测站小时报
#define FUN_CODE_RGZS        0X35        //遥测站人工置数报
#define FUN_CODE_TPB         0X36        //遥测图片报或中心站查询遥测站图片采集信息
#define FUN_CODE_CXSS        0X37        //中心站查询遥测站实时数据
#define FUN_CODE_CXSD        0X38        //中心站查询遥测站时段数据
#define FUN_CODE_CXRGZS      0X39        //中心站查询遥测站人工置数
#define FUN_CODE_CXZDYS      0X3A        //中心站查询遥测指定要素数据
#define FUN_CODE_XGJBPZ      0X40        //中心站修改遥测站基本配置表
#define FUN_CODE_DQJBPZ      0X41        //中心站读取遥测站基本配置表/遥测站自报基本配置表
#define FUN_CODE_XGYXCS      0X42        //中心站修改遥测运行参数配置表
#define FUN_CODE_DQYXCS      0X43        //中心站读取遥测运行参数配置表/遥测站自报运行参数配置表
#define FUN_CODE_CXSBDJ      0X44        //查询水泵电机实时工作数据
#define FUN_CODE_CXRJBB      0X45        //查询遥测终端软件版本
#define FUN_CODE_CXZTBJ      0X46        //查询遥测站状态和报警信息
#define FUN_CODE_CSHSJ       0X47        //初始化固态存储数据
#define FUN_CODE_HFCCSZ      0X48        //恢复终端机出厂设置
#define FUN_CODE_XGMM        0X49        //修改密码
#define FUN_CODE_SZSZ        0X4A        //设置遥测站时钟
#define FUN_CODE_SWJB        0XE0        //自定义功能码 水位加报
#define FUN_CODE_XGPZ        0XE1        //自定义功能码 修改自定义配置
#define FUN_CODE_DQPZ        0XE2        //自定义功能码 读取自定义配置

/**************************要素标识符**************************************/
#define ELEMENT_IDENT_NONE 0x00


/**************************协议配置**************************************/
#define MESSAGE_TYPE_UP  '0'
#define MESSAGE_TYPE_DOW '8'

#define N(a,b) (uint8_t)( a << 4 | b)
#define GET_HIGH_4BIT(a) (a >> 4)
#define GET_LOW_4BIT(a)  (a & 0x0F)

/*要素结构*/
typedef struct
{
  uint8_t* elementIdentifier;        //要素标识符
  float value;                        //要素数据值
  uint8_t dataType;
}element_t;

typedef struct{
	element_t element[MAX_ELEMENT_IN_MESSAGE];
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
}elementInf_t;

/*时间变量*/
typedef struct{
	uint8_t yearBCD;
	uint8_t monthBCD;
	uint8_t dayBCD;
	uint8_t hoursBCD;
	uint8_t minuteBCD;
	uint8_t secondBCD;
}sendTime_t;

typedef struct{
	float batteryVoltage;
}rtuStateInf_t;


/*报文参数*/
typedef struct{
	uint16_t serialNum;
	elementInf_t elementInf;
	rtuStateInf_t rtu_state;
}messageInf_t;

typedef struct{
	uint8_t dataBuf[TX_BUF_LEN];
	uint16_t dataIdx;
}txBuf_t;

extern txBuf_t txDataBuf;
extern messageInf_t messageHandle;

uint16_t getLen_of_txBuf();

void clear_txBuf();
void clear_tail(uint16_t len);
void push_byte_to_txBuf(uint8_t num);
void push_short_to_txBuf(uint16_t num);
void push_int_to_txBuf(uint32_t num);
void push_float_to_txBuf(float num,uint8_t dataType);
void push_data_to_txBuf(uint8_t *srcData,uint16_t len);
uint8_t *get_addr_txBuf(void);
uint16_t getLen_of_txBuf(void);
void clear_element_from_message(messageInf_t *message,int8_t idx);

int8_t add_element(messageInf_t *message,const char* str, float value,uint8_t dataType);
void creat_msg(messageInf_t *message,  uint8_t funCode);

#endif /* HYDROLOGYSTACK_H_ */

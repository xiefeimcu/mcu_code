/*
 * hydrologyStack.h
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */

#ifndef HYDROLOGYSTACK_H_
#define HYDROLOGYSTACK_H_

#include "stm32f1xx_hal.h"

/**************************协议配置**************************************/
#define MAX_ELEMENT_IN_MESSAGE  8
#define TX_BUF_LEN 255
/**************************控制字符定义**************************************/
#define CT_SOH         0xFF

#define CT_SOH_ASCLL   0x01
#define CT_SOH_HEX_BCD (uint16_t)0x7E7E
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
#define COUNT_OF_12MINUTE_IN_HOUR 12

#define CHEX_ASCII 1
#define CDEC_ASCII 0

#define HEX_BYTE

#define MESSAGE_TYPE_UP  0x00
#define MESSAGE_TYPE_DOW 0x08

#define MESSAGE_DATA_FORMAT_HEX   0
#define MESSAGE_DATA_FORMAT_ASCII 1

#define MESSAGE_DATA_FORMAT 1

#define COMM_MODE_M1 0
#define COMM_MODE_M2 1
#define COMM_MODE_M3 2
#define COMM_MODE_M4 3
#define COMM_MODE    COMM_MODE_M1

#define N(a,b) (uint8_t)( a << 4 | b)
#define GET_HIGH_4BIT(a) (a >> 4)
#define GET_LOW_4BIT(a)  (a & 0x0F)

#define IDENT_CODE_ST 0xF0
/*要素标识符索引*/
typedef enum {
	IDT_TT,
	IDT_ST,
	IDT_RGZS,
	IDT_PIC,
	IDT_DRP,
	IDT_DRZ1,
	IDT_DRZ2,
	IDT_DRZ3,
	IDT_DRZ4,
	IDT_DRZ5,
	IDT_DRZ6,
	IDT_DRZ7,
	IDT_DRZ8,
	IDT_DATA,
	IDT_AC,
	IDT_AI,
	IDT_C,
	IDT_DRxnn,
	IDT_DT,
	IDT_ED,
	IDT_EJ,
	IDT_FL,
	IDT_GH,
	IDT_GN,
	IDT_GS,
	IDT_GT,
	IDT_GTP,
	IDT_H,
	IDT_HW,
	IDT_M10,
	IDT_M20,
	IDT_M30,
	IDT_M40,
	IDT_M50,
	IDT_M60,
	IDT_M80,
	IDT_M100,
	IDT_MST,
	IDT_NS,
	IDT_P1,
	IDT_P2,
	IDT_P3,
	IDT_P6,
	IDT_P12,
	IDT_PD,
	IDT_PJ,
	IDT_PN01,
	IDT_PN05,
	IDT_PN10,
	IDT_PN30,
	IDT_PR,
	IDT_PT,
	IDT_Q,
	IDT_Q1,
	IDT_Q2,
	IDT_Q3,
	IDT_Q4,
	IDT_Q5,
	IDT_Q6,
	IDT_Q7,
	IDT_Q8,
	IDT_QA,
	IDT_QZ,
	IDT_SW,
	IDT_UC,
	IDT_UE,
	IDT_US,
	IDT_VA,
	IDT_VJ,
	IDT_VT,
	IDT_Z,
	IDT_ZB,
	IDT_ZU,
	IDT_Z1,
	IDT_Z2,
	IDT_Z3,
	IDT_Z4,
	IDT_Z5,
	IDT_Z6,
	IDT_Z7,
	IDT_Z8,
	IDT_SQ,
	IDT_ZT,
	IDT_pH,
	IDT_DO,
	IDT_COND,
	IDT_TURB,
	IDT_CODMN,
	IDT_REDOX,
	IDT_NH4N,
	IDT_TP,
	IDT_TN,
	IDT_TOC,
	IDT_CU,
	IDT_ZN,
	IDT_SE,
	IDT_AS,
	IDT_THG,
	IDT_CD,
	IDT_PB,
	IDT_CHLA,
	IDT_WP1,
	IDT_WP2,
	IDT_WP3,
	IDT_WP4,
	IDT_WP5,
	IDT_WP6,
	IDT_WP7,
	IDT_WP8,
	IDT_SYL1,
	IDT_SYL2,
	IDT_SYL3,
	IDT_SYL4,
	IDT_SYL5,
	IDT_SYL6,
	IDT_SYL7,
	IDT_SYL8,
	IDT_SBL1,
	IDT_SBL2,
	IDT_SBL3,
	IDT_SBL4,
	IDT_SBL5,
	IDT_SBL6,
	IDT_SBL7,
	IDT_SBL8,
	IDT_VTA,
	IDT_VTB,
	IDT_VTC,
	IDT_VIA,
	IDT_VIB,
	IDT_VIC,
}identifierCodeIdx_t;
extern const char* identifierCode[];

typedef enum{
	FIVE_MINUTE_WATER_DATA,
	/*
	 * TODO此处添加其他水文指标
	 */
}hydrologyDataType_t;
/*
 * 雨量数位信息
 */
typedef struct{
	uint8_t  rainfallPer12Minute[12];
	uint16_t rainfallTotal;
}rainfall_t;

typedef struct{
	uint16_t  waterLevelPer12Minute[12];
}waterLevel_t;

typedef struct{
	waterLevel_t waterLevel;
	rainfall_t   rainfall;
	uint8_t      timeIdx;
}hydrologyInf_t;

/*要素结构*/
typedef struct
{
  identifierCodeIdx_t elementIdentifier;        //要素标识符
  uint8_t elementIdentifierHex;
  float value;                       //要素数据值
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
	uint16_t batteryVoltage;
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
extern hydrologyInf_t hydrologyInf;

uint16_t get_txbuf_len();

void clear_txBuf();
void clear_tail(uint16_t len);
void push_ascll_integer_to_txBuf(uint32_t num, uint8_t dataType,uint8_t isHexaDec);
void push_ascll_float_to_txBuf(float num,uint8_t dataType);
void push_ascll_data_to_txBuf(uint8_t *srcData,uint16_t len);
void add_sim_waterInf_data(hydrologyInf_t *hydrologyData);
void add_waterInf_data(hydrologyInf_t *hydrologyData,uint8_t rainFall, uint16_t waterLevel);

uint8_t *get_txbuf_addr(void);
uint16_t get_txbuf_len(void);
void clear_element_from_message(messageInf_t *message,int8_t idx);

int8_t write_element(messageInf_t *message, identifierCodeIdx_t ident, float value , uint8_t dataType);
void creat_msg(messageInf_t *message, uint8_t funCode,uint16_t msgSerial,uint8_t isLastMsg);

/*************test fun******/
void TEST_HYK_test_msg(messageInf_t *message);
void TEST_HYK_keep_msg(messageInf_t *message);
void TEST_HYK_timing_msg(messageInf_t *message);
void TEST_HYK_hour_msg(messageInf_t *message);
void TEST_HYK_man_msg(messageInf_t *message);
void TEST_HYK_plus_msg(messageInf_t *message);
void TEST_HYK_timeAverage_msg(messageInf_t *message);
/*************test fun end***/

#endif /* HYDROLOGYSTACK_H_ */

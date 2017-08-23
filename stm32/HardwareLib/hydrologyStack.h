/*
 * hydrologyStack.h
 *
 *  Created on: 2017年8月21日
 *      Author: xiefei
 */

#ifndef HYDROLOGYSTACK_H_
#define HYDROLOGYSTACK_H_

#include "stm32f1xx_hal.h"
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
#define FUN_CODE_LLWC        0X2F            //链路维持报
#define FUN_CODE_CSB         0X30            //测试报
#define FUN_CODE_JYSD        0X31            //均有时段水文信息报
#define FUN_CODE_DSB         0X32            //遥测站定时报
#define FUN_CODE_JBB         0X33            //遥测站加报报
#define FUN_CODE_XSB         0X34            //遥测站小时报
#define FUN_CODE_RGZS        0X35            //遥测站人工置数报
#define FUN_CODE_TPB         0X36            //遥测图片报或中心站查询遥测站图片采集信息
#define FUN_CODE_CXSS        0X37            //中心站查询遥测站实时数据
#define FUN_CODE_CXSD        0X38            //中心站查询遥测站时段数据
#define FUN_CODE_CXRGZS      0X39            //中心站查询遥测站人工置数
#define FUN_CODE_CXZDYS      0X3A            //中心站查询遥测指定要素数据
#define FUN_CODE_XGJBPZ      0X40            //中心站修改遥测站基本配置表
#define FUN_CODE_DQJBPZ      0X41            //中心站读取遥测站基本配置表/遥测站自报基本配置表
#define FUN_CODE_XGYXCS      0X42            //中心站修改遥测运行参数配置表
#define FUN_CODE_DQYXCS      0X43            //中心站读取遥测运行参数配置表/遥测站自报运行参数配置表
#define FUN_CODE_CXSBDJ      0X44            //查询水泵电机实时工作数据
#define FUN_CODE_CXRJBB      0X45            //查询遥测终端软件版本
#define FUN_CODE_CXZTBJ      0X46            //查询遥测站状态和报警信息
#define FUN_CODE_CSHSJ       0X47            //初始化固态存储数据
#define FUN_CODE_HFCCSZ      0X48            //恢复终端机出厂设置
#define FUN_CODE_XGMM        0X49            //修改密码
#define FUN_CODE_SZSZ        0X4A            //设置遥测站时钟
#define FUN_CODE_SWJB        0XE0            //自定义功能码 水位加报
#define FUN_CODE_XGPZ        0XE1            //自定义功能码 修改自定义配置
#define FUN_CODE_DQPZ        0XE2            //自定义功能码 读取自定义配置


/*单个上报要素结构*/
typedef struct
{
  unsigned char *elementIdentifier;   //要素标识符
  long int value;                     //要素数据值
  unsigned char dataType;             //数据类型 BCD码 N(x,y)
}element_t;

/*时间变量*/
typedef struct{
	uint8_t yearBCD;
	uint8_t monthBCD;
	uint8_t dayBCD;
	uint8_t hoursBCD;
	uint8_t minuteBCD;
}sendTime_t;

/*报文正文*/
typedef struct {
	uint8_t serialNumH;
	uint8_t serialNumL;

	sendTime_t sendTime;

	uint8_t RtuType;

	sendTime_t measureTime;


}messageMainBody_t;

/*报文*/
typedef struct{
	uint8_t frameStartCharH;//帧启始符
	uint8_t frameStartCharL;

	uint8_t centreStationAddr;//中心站地址
	uint8_t RtuStationAddr[5];//遥测站地址

	uint8_t pswH;//密码
	uint8_t pswL;

	uint8_t funCode;//功能码

	uint8_t identifierAndLenH :4;//上下行标识符
	uint8_t identifierAndLenL :12;//报文长度

	uint8_t messageStartChar;//报文启始符

	messageMainBody_t * messageMainBody;

	uint8_t messageStopChar;//报文启始符
	uint16_t crc;
}message_t;



#endif /* HYDROLOGYSTACK_H_ */

/***********************************************************************************/
//                       PCF8563.h With MSP430F149 Driver
//Version: 1.0.0
//Author:  LiHuaiLiang
//Email:   li-huai-liang@163.com
//Date:    2009-03-14
//Modify:  
//====================================================================================
//                       PCF8563.h With MSP430F149 Driver
//版本: 1.0.0
//作者:  李怀良
//Email:   li-huai-liang@163.com
//日期:    2009-03-14
//修改:    赵阳 2012-05-16
///***********************************************************************************/

#ifndef __PCF8563_H
#define __PCF8563_H

#include "MSP430_hardware.h"
#include "Public.h"


#define RTCIN           (P1IN & 0X04)

#define WRITE_ADDR      0xA2
#define READ_ADDR       0xA3

//PCF8563地址宏定义
#define year          	0x08
#define month         	0x07
#define day          	0x05
#define week          	0x06
#define hour         	0x04
#define minute       	0x03
#define second        	0x02

#define PCF8563delayus(x)       delayus(x)

///***********************************************************************************/
void PCF8563writeData(char address,char mdata);
//char PCF8563ReadData(char address); /*单字节*/
//char PCF8563WriteData(char address,char *p,char count);
char PCF8563ReadDatas(char address,char  *p,char count);//多字节


/*读取系统时间*/
char ReadTime(DataTime *t);
void SetTime(DataTime *t);
char PCF8563Ini(void);
//unsigned int TimeInterval(Time *t0,Time *t1);

#endif /*#ifndef __PCF8563_H*/
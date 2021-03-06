/*********************************************************
*文件名称：AT24C04.h
*摘    要：AT24C04配置文件
*
*当前版本：1.0
*作    者：赵阳
*开始日期：2012年5月17日
*完成日期：
**********************************************************/

#ifndef __AT24C04_H
#define __AT24C04_H
 
#include "MSP430_hardware.h"
  
//I/O口定义
#define SCL_H                   P1OUT |= BIT4
#define SCL_L                   P1OUT &= ~BIT4
#define SDA_H                   P1OUT |= BIT3
#define SDA_L                   P1OUT &= ~BIT3
#define SDA_in                  P1DIR &= ~BIT3        //SDA改成输入模式
#define SDA_out                 P1DIR |= BIT3         //SDA变回输出模式
#define SDA_val                 P1IN&BIT3             //SDA的位值
#define IniRAM                  P1DIR |= BIT3+BIT4; 
#define STOPRAM                 P1DIR &= ~(BIT4 +BIT3);//设置I/O为输入
#define deviceaddress           0xa0                  //AT24C02的设备地址

#define AT24C04delayus(x)       delayus(x)      //延时us
#define AT24C04delayms(x)       delayms(x)      //延时ms

#define WriteUInt8(x,y)         Write_1Byte(x,y)                        //写入1byte数据
#define ReadUInt8(x)            Read_1Byte_Randomaddress(x)             //读1byte数据


unsigned char AT24C04TEST(unsigned char testadd);
unsigned char WriteUInt16(unsigned int num,unsigned char dataaddr);
unsigned int ReadUInt16(unsigned char addr);
unsigned char WriteSInt32(long num,unsigned char dataaddr);
signed long ReadSInt32(unsigned char addr);
unsigned char Write_1Byte(unsigned char wdata,unsigned char dataaddress);                        
unsigned char Write_NByte(unsigned char * outbuf,unsigned char n,unsigned char dataaddress);
unsigned char Read_1Byte_currentaddress(void);
unsigned char Read_NByte_currentaddress(unsigned char * readbuf,unsigned char n);
unsigned char Read_1Byte_Randomaddress(unsigned char dataaddress);
unsigned char Read_NByte_Randomaddress(unsigned char * readbuf,unsigned char n,unsigned char dataaddress);

#endif /* #ifndef __AT24C04_H */
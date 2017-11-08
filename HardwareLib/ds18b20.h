/*********************************************************
*文件名称：ds18b20.h
*摘    要：温度传感器DS18B20接口
*
*当前版本：1.0
*作    者：赵阳
*开始日期：2012年5月15日
*完成日期：2012年5月16日
*
**********************************************************/

#ifndef __DS18B20_H
#define __DS18B20_H

#include "system.h"
#include "Public.h"


/***************************************************************
                    DS18B20 I/O口定义
***************************************************************/
#define B20                     BIT3                    
#define PORTIN                  P6IN
#define DQ1                     P6OUT |= BIT3
#define DQ0                     P6OUT &= ~BIT3
#define DQIN                    P6DIR &=~ BIT3
#define DQOUT                   P6DIR |= BIT3

#define DS18B20delayus(x)       delayus(x)
#define DS18B20delayms(x)       delayms(x)


typedef struct
{
  unsigned char Ti;
  unsigned char Event;
}ClassDS18B20;

extern ClassDS18B20 DS18B20;

//extern int Temperature;


/***************************************************************
                        接口函数                          
****************************************************************/
 
/*************** 控制DS18B20 完成一次温度读取 ******************/
unsigned int Do1Convert(void);

int Readtemp(unsigned int temper);

unsigned char Ini_18B20(void);
unsigned char StartDS18B20(void);
unsigned int ReadDs18B20temp(void);

#endif /*__DS18B20_H*/
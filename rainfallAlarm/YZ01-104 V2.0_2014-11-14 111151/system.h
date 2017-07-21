#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Public.h"

#define DEBUG                   0                       //调试等级
#define SET                     1                       //置位 
#define RESET                   0                       //复位

#define KEYIN                   (P2IN & (~(BIT2|BIT7)))                           //按键输入
#define KEYINTIME               10                                      //按键输入保持时间 x ms
//键值定义
#define Esc	                0X5B                    //返回
#define Up	                0X73                    //上
#define Down	                0X79                    //下
#define Left	                0X6B                    //左
#define Right	                0X7A                    //右
#define Enter	                0X3B                    //确定


typedef struct                  
{ 
  union                         //系统事件寄存器
  {
    unsigned int Register;
    struct
    {
      char Second        :1;//1秒定时
      char Minute        :1;//1分钟定时
      char Hour          :1;//整点定时
      char UART0BYTE     :1;//收到1byte
      char UART1BYTE     :1;
      char UART0         :1;//收到一包
      char UART1         :1;
      char Rain          :1;//雨量输入
      char Key           :1;//按键输入
      char Flash         :1;//读取历史数据
    }Bit;
  }Event;
  unsigned int Voltage;                      //电源电压
  int Temp;                         //温度
  DataTime Time;                        //系统日期时间
  char Key;                    //键值缓存 
  
}ClassSystem;

typedef union
{
  unsigned int Register;
  struct
  {
    char RTC   :1;
    char EROM  :1;
    char Flash :1;
    
  }Bit;
}ClsaaSysError;


extern ClassSystem System;
extern ClsaaSysError SysError;


void SysInitHandlers(void);
void SysTimePlue(void);
void SysEventHandlers(void);
void SysEventHandlers_Second(void);
void SysEventHandlers_Minute(void);
void SysEventHandlers_Hour(void);
void SysEventHandlers_UART0_Rxd(void);
void SysEventHandlers_UART1_Rxd(void);
void SysEventHandlers_Key(void);
void UserEventHandlers(void);

void ICInit(void);
void ReadSysConfig(void);
void RestorationData(void);
void PeripheralsInit(void);
void SystemSelfTest(void);
void TiTimeHandlers(void);


#endif  /*#ifndef SYSTEM_H_*/
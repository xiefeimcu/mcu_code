#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Public.h"

#define Model                   "YZ02-101 V3.0"
#define Version                 "V1.2_20170721"

#define DEBUG                   0                       //调试等级
#define SET                     1                       //置位 
#define RESET                   0                       //复位

#define KEYIN                   (P2IN & 0X3F)                           //按键输入
#define KEYINTIME               20                                      //按键输入保持时间 x ms
//键值定义
/*#define Esc	                0X1F                                    //返回
#define Up	                0X37                                    //上
#define Down	                0X3B                                    //下
#define Left	                0X2F                                    //左
#define Right	                0X3D                                    //右
#define Enter	                0X3E                                    //确定
#define Esc_Up                  0X17                                    //返回+上
#define Esc_Enter               0X1E                                    //返回+确定*/

#define Esc	                0X3E                                    //返回
#define Up	                0X3B                                    //上
#define Down	                0X37                                    //下
#define Left	                0X3D                                    //左
#define Right	                0X2F                                    //右
#define Enter	                0X1F                                    //确定
#define Esc_Up                  0X3A                                    //返回+上
#define Esc_Enter               0X1E                                    //返回+确定



typedef struct                  
{ 
  union                         //系统事件寄存器
  {
    unsigned int Register;
    struct
    {
      unsigned char Second        :1;//1秒定时
      unsigned char Minute        :1;//1分钟定时
      unsigned char Hour          :1;//整点定时
      unsigned char UART0BYTE     :1;//收到1byte
      unsigned char UART1BYTE     :1;
      unsigned char UART0         :1;
      unsigned char UART1         :1;
      unsigned char Rain          :1;//雨量输入
      unsigned char Key           :1;//按键输入
      unsigned char Flash         :1;//读取历史数据
    }Bit;
  }Event;
  DataTime Time;                        //系统日期时间
  unsigned char Key;                    //键值缓存 
  
}ClassSystem;

typedef union
{
  unsigned int Register;
  struct
  {
    unsigned char RTC   :1;
    unsigned char EROM  :1;
    unsigned char Flash :1;
    
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

#endif  /*#ifndef SYSTEM_H_*/
/*********************************************************
*文件名称：Display.h
*摘    要：显示功能函数声明
*
*作    者：赵阳
*开始日期：2013年4月16日
*完成日期：
**********************************************************/

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "ST7920.h"
#include "Public.h"

#define OFFLCDDELAY     100              //关闭显示延时

/*显示状态信息*/
typedef struct
{
  unsigned int TiOFF;                  //关闭延时计时
  unsigned char Mon;                            //监测界面编号
  union
  {
    unsigned char Register;
    struct
    {
      unsigned char Light       :1;     //背光状态
      unsigned char Power       :1;     //电源状态
      unsigned char Mon         :1;     //监测界面
      unsigned char Menu        :1;     //设置菜单界面
      unsigned char KeyOut      :1;     //等待按键超时标志
      unsigned char OFFEvent    :1;     //显示关闭事件标志
      unsigned char SetMode     :1;     //设置模式
    }Flag;   
  }Status; 
}DisplayClass;

extern DisplayClass Display;

void LCDOFF(void);
void LCDON(void);     
      
void BootDisplay(void);

void DisplayNum(unsigned char H,unsigned char Ver,long int value,unsigned char type,
                unsigned char cmd,unsigned char *s,unsigned int fb);

void DisplayNum8X6(unsigned char H,unsigned char Ver,long int value,
                unsigned char type,unsigned char cmd,unsigned char *s);

void DisplaySysTime(void);

void DisplayMon(void);
void DisBattery(unsigned char H,unsigned char Ver, unsigned char power);
void DisplayRunInfo(unsigned char *infostr);

void DisplayRainfall(void);
void DisplayAnalogData(void);
void DisplayMonData(void);

#endif  /*#ifndef __DISPLAY_H*/



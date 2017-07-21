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

#define OFFLCDDELAY     50              //关闭显示延时

/*显示状态信息*/
typedef struct
{
  unsigned int TiOFF;                  //关闭延时计时
  char Mon;                            //监测界面编号
  union
  {
    char Register;
    struct
    {
      char Light       :1;     //背光状态
      char Power       :1;     //电源状态
      char Mon         :1;     //监测界面
      char Menu        :1;     //设置菜单界面
      char KeyOut      :1;     //等待按键超时标志
      char OFFEvent    :1;     //显示关闭事件标志
      char SetMode     :1;     //设置模式
    }Flag;   
  }Status; 
}DisplayClass;

extern DisplayClass Display;

void LCDOFF(void);
void LCDON(void);     
      
void DisplayMon(void);
void DisBattery(char H,char Ver,char power);
void DisplayRunInfo(char *infostr);

void DisplayRainfall(void);

void DisplayMonData(void);
void DisplaySysTime(void);
void Battery(unsigned char H,unsigned char Ver, unsigned int dianya);
#endif  /*#ifndef __DISPLAY_H*/



#include "include.h"
#include "Rainfall.h"

RainfallClass Rainfall={0};

//雨量输入初始化
void RainInit(void)
{
  RAIN_IES |=  RAINPORT_1+RAINPORT_2;                         //雨量输入选择下降沿中断
  RAIN_IE  |=  RAINPORT_1+RAINPORT_2;                         //打开中断使能
  RAIN_DIR &= ~(RAINPORT_1+RAINPORT_2);                       //雨量输入I/O为输入
  Rainfall.InDelay = 0;
}

void SysEventHandlers_Rain(void)
{
  System.Event.Bit.Rain = RESET;

  Rainfall.PN05 += Rainfall.Resolution; //降水量增加
  Rainfall.PN10 += Rainfall.Resolution;
  Rainfall.PN30 += Rainfall.Resolution;
  Rainfall.PJ += Rainfall.Resolution;
  Rainfall.PT += Rainfall.Resolution;     
  if(Display.Status.Flag.Mon)
  {
    DisplayRainfall();
  }
  
  WriteUInt16(Rainfall.PT,EEPROM.PT);
  WriteUInt16(Rainfall.PJ,EEPROM.PJ);
  WriteUInt16(Rainfall.PN05,EEPROM.PN05);
  WriteUInt16(Rainfall.PN10,EEPROM.PN10);
  WriteUInt16(Rainfall.PN30,EEPROM.PN30);
}

/*雨量清零*/
void RainfallClear(void)
{
  Rainfall.PN05 = 0;
  Rainfall.PN10 = 0;
  Rainfall.PN30 = 0;
  Rainfall.PJ = 0;
  Rainfall.PT = 0;
  WriteUInt16(Rainfall.PT,EEPROM.PT);
  WriteUInt16(Rainfall.PJ,EEPROM.PJ);
  WriteUInt16(Rainfall.PN05,EEPROM.PN05);
  WriteUInt16(Rainfall.PN10,EEPROM.PN10);
  WriteUInt16(Rainfall.PN30,EEPROM.PN30);
  InitDRP();
  SaveDRP();
}
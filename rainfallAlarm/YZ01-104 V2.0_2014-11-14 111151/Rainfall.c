#include "include.h"
#include "Rainfall.h"

RainfallClass Rainfall={0};
char RainArray[288]={0};

//雨量输入初始化
void RainInit(void)
{
  P1IES |= BIT5;
  P2IES |= BIT7;
  P1IE |= BIT5;
  P2IE |= BIT7;
  P1DIR &= ~BIT5;
  P2DIR &= ~BIT7;
  //RAIN_IES |=  RAINPORT_1|RAINPORT_2;                         //雨量输入选择下降沿中断
  //RAIN_IE  |=  RAINPORT_1|RAINPORT_2;                         //打开中断使能
  //RAIN_DIR &= ~(RAINPORT_1|RAINPORT_2);                       //雨量输入I/O为输入
  Rainfall.InDelay = 0;
}

void SysEventHandlers_Rain(void)
{
  System.Event.Bit.Rain = RESET;
  
  Rainfall.PN05 += Rainfall.Resolution;
  Rainfall.PN10 += Rainfall.Resolution;
  Rainfall.PN30 += Rainfall.Resolution;
  Rainfall.P1 += Rainfall.Resolution;
  Rainfall.P3 += Rainfall.Resolution;
  Rainfall.P6 += Rainfall.Resolution;
  Rainfall.P12 += Rainfall.Resolution;
  Rainfall.P24 += Rainfall.Resolution;
  Rainfall.P += Rainfall.Resolution;
  Rainfall.PJ += Rainfall.Resolution;
  Rainfall.PT += Rainfall.Resolution;
  RainArray[287] += Rainfall.Resolution;
  LsData.P1 += Rainfall.Resolution;
  AlarmLevelHandle(1);

  DisplayRainfall();

  WriteUInt16(Rainfall.PJ,AT_PJ,0); 
  WriteUInt16(Rainfall.PT,AT_PT,0); 
  WriteUInt16(LsData.P1,AT_LISP1,0);
 
}

/*雨量清零*/
void RainfallClear(void)
{
  unsigned int i=0;
  
  for(i=0;i<288;i++)
  {
    RainArray[i] = 0;
  }
  Rainfall.PN05 = 0;
  Rainfall.PN10 = 0;
  Rainfall.PN30 = 0;
  Rainfall.P1 = 0;
  Rainfall.P3 = 0;
  Rainfall.P6 = 0;
  Rainfall.P12 = 0;
  Rainfall.P24 = 0;
  Rainfall.P = 0;
  Rainfall.PJ = 0;
  Rainfall.PT = 0;
  LsData.P1 = 0;
  AlarmLevel.Level = 0;
  AlarmLevel.Qlevel = 0;
  AlarmLevel.Rain = 0;
  WriteUInt16(Rainfall.PJ,AT_PJ,0); 
  WriteUInt16(Rainfall.PT,AT_PT,0);
  WriteUInt16(LsData.P1,AT_LISP1,0);  
}



void IniRainArray(char value)
{
  unsigned int i=0;
  for(i=0;i<288;i++)
  {
    RainArray[i] = value;
  }
}

void RainArrayRefresh(void) 
{
  unsigned int i=0;

  for(i=0;i<287;i++)
  {
    RainArray[i] = RainArray[i+1];
    if((i+1)>=287)
      break;
  }
  RainArray[287] = 0;
   
  Rainfall.PN05 = 0;
  Rainfall.PN10 = GetRain(10);
  Rainfall.PN30 = GetRain(30);
  Rainfall.P1 = GetRain(1);
  Rainfall.P3 = GetRain(3);
  Rainfall.P6 = GetRain(6);
  Rainfall.P12 = GetRain(12);
  Rainfall.P24 = GetRain(24);
  if(0==Rainfall.PN30)
  {
    Rainfall.P = 0;
  }
  AlarmLevel.Qlevel = 0;
  AlarmLevel.Level = 0;
  AlarmLevel.Rain = 0;
  AlarmLevelHandle(0);

}

unsigned int GetRain(unsigned int x)
{
  unsigned int j=287;
  unsigned int sum=0;
  unsigned int i=0;
  sum = 0;
  switch(x)
  {
    case 5: 
      {
        sum = RainArray[287];
      }break;
    case 10: 
      {
        sum = RainArray[287]+RainArray[286];
      }break;
    case 30: 
      {
        j = 287;
        for(i=0;i<6;i++)
        {
          sum += RainArray[j--];
        }
      }break;
    case 1: 
      {
        j = 287;
        for(i=0;i<12;i++)
        {
          sum += RainArray[j--];
        }
      }break; 
    case 3: 
      {
        j = 287;
        for(i=0;i<36;i++)
        {
          sum += RainArray[j--];
        }
      }break; 
    case 6: 
      {
        j = 287;
        for(i=0;i<72;i++)
        {
          sum += RainArray[j--];
        }
      }break; 
    case 12: 
      {
        j = 287;
        for(i=0;i<144;i++)
        {
          sum += RainArray[j--];
        }
      }break;
    case 24: 
      {
        for(i=0;i<287;i++)
        {
          sum += RainArray[i];        
        }
      }break;
  }
  return sum;
}

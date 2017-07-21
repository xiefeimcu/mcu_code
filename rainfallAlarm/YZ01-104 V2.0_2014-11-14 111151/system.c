//---------------------------------------------------------
//文件名称：system.c
//摘    要：系统处理
//
//作    者：赵阳
//版    本：1.0
//修改日期：2013年11月24日
//---------------------------------------------------------
#include "MSP430_hardware.h"
#include "system.h"
#include "include.h"

ClassSystem System={0};
ClsaaSysError SysError={0};
 
unsigned int dianya=0;

void SysInitHandlers(void)
{
  char i=1;
  LEDINI;
  LCD_LIGHT_OFF;
  LCD_POWER_OFF;
  Except();                             //非正常复位处理
  InitSysCKL();                         //初始化系统时钟

  SC_Reset();
  SC_Reset();
  Ini_Lcd();
  for(i=1;i<62;i++)
  {
    Draw(4,0,8,i,0,logp);               //开机动画
    delayms(10);
  } 
  delayms(1000);
  KeyInit();                            //按键初始化
  
  RainInit();
  ICInit();                             //
  SystemSelfTest();                     //自检
  ReadSysConfig();                      //读取配置信息
  
  IniTimerA();                          //定时器A初始化
  TimerBInit();
  SETTBMS(20);                          //
  
  _EINT();                              //中断ON
  
  if(!ReadTime(&System.Time))           //读取时钟
  {
    System.Time.Year = 1;
    System.Time.Month = 1;
    System.Time.Day = 1;
    System.Time.Hour = 0;
    System.Time.Minute = 0;
    System.Time.Second = 0;
  }
  System.Voltage = GetVcc();
  RestorationData();                    //数据恢复初始化
  PeripheralsInit();                    //外设初始化
  
  Display.Mon = 1;
  ClearScreen;
  DisplayMon();  
}


//秒钟定时系统时间计算
void SysTimePlue(void)
{
  System.Event.Bit.Second = SET;          //1秒定时事件置位
  if(++System.Time.Second > 59)
  {
    System.Time.Second = 0;
    System.Event.Bit.Minute = SET;        //分钟定时事件置位
    if(++System.Time.Minute > 59)
    {
      System.Time.Minute = 0;
      System.Event.Bit.Hour = SET;        //整点事件置位
      if(++System.Time.Hour > 23)
      {
        System.Time.Hour = 0;
        if(++System.Time.Day > Timedaymax(System.Time))//日期计算包含闰年处理
        {
          System.Time.Day = 1;
          if(++System.Time.Month > 12)
          {
            System.Time.Month = 1;
            if(++System.Time.Year > 99)
            {
              System.Time.Year = 0;
            }
          }
        }
      }
    }
  } 
}

/*******************************************************************************
函数名称：void SysEventHandlers(void) 
函数介绍：系统事件处理
输入参数：无                             
输出参数：系统事件标志寄存器   
返回值  ：无
*******************************************************************************/
void SysEventHandlers(void)
{
  if(System.Event.Bit.Rain)
  {
    SysEventHandlers_Rain();
  }
  if(System.Event.Bit.Second)
  {
    SysEventHandlers_Second();
  }
  if(System.Event.Bit.Minute)
  {
    SysEventHandlers_Minute();
  }
  if(System.Event.Bit.Hour)
  {
    SysEventHandlers_Hour();
  }
  if(System.Event.Bit.UART0)
  {
    SysEventHandlers_UART0_Rxd();
  }
  if(System.Event.Bit.UART1)
  {
    SysEventHandlers_UART1_Rxd();
  }
  if(System.Event.Bit.Key)
  {
    SysEventHandlers_Key();
  }
}

/*******************************************************************************
函数名称：void SysEventHandlers_Second(void) 
函数介绍：秒钟定时事件处理
输入参数：无                             
输出参数：系统事件标志寄存器
返回值  ：无
*******************************************************************************/
void SysEventHandlers_Second(void)
{
  /*unsigned long int secondnum=0; 
   
  //计算当前时刻距00：00：00时刻的秒钟数
  secondnum = (System.Time.Hour*3600u)+(System.Time.Minute*60u)+System.Time.Second;*/
  System.Event.Bit.Second = RESET;
  
  if((27 == System.Time.Minute) && (27 == System.Time.Second))//每小时的27分27秒从时钟芯片更新系统时间
  {
    ReadTime(&System.Time);
  }

  if(AlarmMode.Status.Flag.YuYing)
  {
    JDQON;
    if(0==AlarmMode.Time)
    {
      AlarmSpeech();
    }   
  }
}

/*******************************************************************************
函数名称：void SysEventHandlers_Minute(void) 
函数介绍：分钟定时事件处理
输入参数：无                             
输出参数：系统事件标志寄存器
返回值  ：无
*******************************************************************************/
void SysEventHandlers_Minute(void)
{ 
 /*unsigned int minutenum = 0;
  
  
  //计算当前时刻距00：00：00时刻的分钟数
  minutenum = (System.Time.Hour*60u)+System.Time.Minute;*/ 
  
  System.Event.Bit.Minute = RESET;
  
  if((System.Time.Minute%5)==0)
  {
    RainArrayRefresh();
    DisplayRainfall();
  }
  if(Display.Status.Flag.Mon)                   //当前显示界面为监测界面时 更新系统时间显示
  {
    DisplaySysTime();
  }
  
}

/*******************************************************************************
函数名称：void SysEventHandlers_Minute(void) 
函数介绍：整点事件处理
输入参数：无                             
输出参数：系统事件标志寄存器
返回值  ：无
*******************************************************************************/
void SysEventHandlers_Hour(void)
{
  System.Event.Bit.Hour = RESET;
  
  if(8==System.Time.Hour)
  {
    if(Rainfall.PJ!=0)
    {
      Rainfall.PJ = 0;
      WriteUInt16(Rainfall.PJ,AT_PJ,0); 
      DisplayRainfall();
    }
  }
  if(LsData.P1 != 0)
  {
    LsData.PT = Rainfall.PT;
    SaveLisdata();
    LsData.P1 = 0;
    WriteUInt16(LsData.P1,AT_LISP1,0); 
  }
  
}

//UART数据接收事件处理
void SysEventHandlers_UART0_Rxd(void)
{ 
  System.Event.Bit.UART0 = RESET;
  
  if((UART0_RxdLeng<40)&&(0X0A==UART0_RxdBuff[UART0_RxdLeng-1])&&(0X0D==UART0_RxdBuff[UART0_RxdLeng-2]))
  {
    Display.TiOFF = OFFLCDDELAY;
    if(ATcmdif(UART0_RxdBuff))
    {
      ATcmdconfig();
    } 
  }
  DELETE_UART0RXD;
}

void SysEventHandlers_UART1_Rxd(void)
{ 
  System.Event.Bit.UART1 =RESET;
 
  DeleteUART1_RxdBuff(UART1_RxdLeng);
}

//按键输入事件处理
void SysEventHandlers_Key(void)
{   
  if(Display.Status.Flag.SetMode)
    return;
  
  System.Event.Bit.Key = RESET;
  if(Display.Status.Flag.Power)
  {
    LCD_LIGHT_ON;                   //LCD背光开
    Display.Status.Flag.Light = SET;
    switch(System.Key)
    {
      case Up:
        {
          
        }break;      
      case Down:
        {

        } break;    
      case Left:
        {
          Display.Mon--;
          DisplayMon();
        }break;   
      case Right:
        {
          Display.Mon++;
          DisplayMon();
        } break;    
      case Enter:
        {
          Display.Status.Flag.SetMode = SET;
        }break;    
      case Esc:
        {
          if(AlarmMode.Status.Flag.Run)
          {
            StopAlarm();
          }
          else
          {
            LCDOFF();
          }
        }break;
      default: break;
    }
  }
  else
  {
    LCDON();
  }
  System.Key = 0;
}

//用户事件任务处理
void UserEventHandlers(void)
{
  
  
}

/*读取配置信息*/
void ReadSysConfig(void)
{ 
  Rainfall.PJ = ReadUInt16(AT_PJ,0);
  Rainfall.PT = ReadUInt16(AT_PT,0);
  Rainfall.Resolution = ReadUInt8(AT_FBL,0);
  
  Alarm.PN05.I = ReadUInt16(AT_PN05I,0);
  Alarm.PN05.II = ReadUInt16(AT_PN05II,0);
  Alarm.PN05.III = ReadUInt16(AT_PN05III,0);
  Alarm.PN10.I = ReadUInt16(AT_PN10I,0);
  Alarm.PN10.II = ReadUInt16(AT_PN10II,0);
  Alarm.PN10.III = ReadUInt16(AT_PN10III,0);
  Alarm.PN30.I = ReadUInt16(AT_PN30I,0);
  Alarm.PN30.II = ReadUInt16(AT_PN30II,0);
  Alarm.PN30.III = ReadUInt16(AT_PN30III,0);
  Alarm.P1.I = ReadUInt16(AT_P1I,0);
  Alarm.P1.II = ReadUInt16(AT_P1II,0);
  Alarm.P1.III = ReadUInt16(AT_P1III,0);
  Alarm.P3.I = ReadUInt16(AT_P3I,0);
  Alarm.P3.II = ReadUInt16(AT_P3II,0);
  Alarm.P3.III = ReadUInt16(AT_P3III,0);
  Alarm.P6.I = ReadUInt16(AT_P6I,0);
  Alarm.P6.II = ReadUInt16(AT_P6II,0);
  Alarm.P6.III = ReadUInt16(AT_P6III,0);
  Alarm.P12.I = ReadUInt16(AT_P12I,0);
  Alarm.P12.II = ReadUInt16(AT_P12II,0);
  Alarm.P12.III = ReadUInt16(AT_P12III,0);
  Alarm.P24.I = ReadUInt16(AT_P24I,0);
  Alarm.P24.II = ReadUInt16(AT_P24II,0);
  Alarm.P24.III = ReadUInt16(AT_P24III,0);
  Alarm.P.I = ReadUInt16(AT_PI,0);
  Alarm.P.II = ReadUInt16(AT_PII,0);
  Alarm.P.III = ReadUInt16(AT_PIII,0);
  Alarm.PJ.I = ReadUInt16(AT_PJI,0);
  Alarm.PJ.II = ReadUInt16(AT_PJII,0);
  Alarm.PJ.III = ReadUInt16(AT_PJIII,0);
  Alarm.PT.I = ReadUInt16(AT_PTI,0);
  Alarm.PT.II = ReadUInt16(AT_PTII,0);
  Alarm.PT.III = ReadUInt16(AT_PTIII,0);
  //Rainfall.Count = ReadUInt16(AT_COUNT,0);
  Flash.Page = ReadUInt16(AT_PAGE,0);
  Flash.Bytes = ReadUInt16(AT_BYTE,0);
  LsData.P1 = ReadUInt16(AT_LISP1,0);
  Display.TiOFF = OFFLCDDELAY;
}

/*数据恢复*/ 
void RestorationData(void)
{
 
}
  
void ICInit(void)
{
  
}

/*外设初始化*/
void PeripheralsInit(void)
{
  LEDINI;

}

/*系统自检*/
void SystemSelfTest(void)
{
  SysError.Register = 0;
  if(!PCF8563Ini())
  {
    SysError.Bit.RTC = SET;
  }
  if(!AT24C04TEST(AT_TEST))
  {
    SysError.Bit.EROM = SET;
  }
  if(!FindFlash())
  {
    SysError.Bit.Flash = SET;
  }
  if(SysError.Register)
  {
    if(SysError.Bit.RTC)
    {
      DisplayString(0,0,"RTC ERROR",0);
    }
    if(SysError.Bit.EROM)
    {
      DisplayString(0,16,"EROM ERROR",0);
    }
    if(SysError.Bit.Flash)
    {
      DisplayString(0,32,"Flash ERROR",0);
    }
    delayms(2000);
  }  
}
/**************************************************************
函数名称：void TiTimeHandlers(void)
函数介绍：定时时间处理
输入参数：无
输出参数：相关寄存器
返回值  ：无
*************************************************************/
void TiTimeHandlers(void)
{
  if(AlarmMode.Time > 0)
  {
    AlarmMode.Time--;
  }
  
  if(Display.TiOFF > 0)
  {
    Display.TiOFF--;
    if(Display.TiOFF==0)
    {
      LCDOFF();
    }
    else if(Display.TiOFF==(OFFLCDDELAY-20))
    {
      LCD_LIGHT_OFF;      //LCD背光关
      Display.Status.Flag.Light = RESET;
    }
  }
}

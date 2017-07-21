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
  Except();                             //非正常复位处理
  InitSysCKL();                         //初始化系统时钟
  Ini_Lcd();
  Draw(4,0,8,61,0,logp);
  KeyInit();                            //按键初始化
  RainInit();
  ICInit();                             //
  SystemSelfTest();                     //自检
  ReadSysConfig();                      //读取配置信息
  
  IniTimerA();                          //定时器A初始化
  TimerBInit();
  SETTBMS(20);                          //
  _EINT();                              //中断ON

  ReadTime(&System.Time);
  RTU.Voltage = GetVcc();
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
  //unsigned long int secondnum=0;
  
  System.Event.Bit.Second = RESET;
   
  //secondnum = (System.Time.Hour*3600u)+(System.Time.Minute*60u)+System.Time.Second;
  
  if((27==System.Time.Minute)&&(27==System.Time.Second))
  {
    ReadTime(&System.Time);
  }

  if(Display.Status.Flag.Mon)
  {
    DisplaySysTime();
  }

  if(33==System.Time.Second)    //读电源电压
  {
    RTU.Voltage = GetVcc();
    if(Display.Status.Flag.Mon&&(4==Display.Mon))
    {
      DisplayNum(9,16,RTU.Voltage,0X42,space,"",0);
    }
  }
  
  if(5==System.Time.Second)     //读设备温度
  {
    DS18B20.Event = SET;//温度处理事件
    StartDS18B20();     //启动一次温度测量
    DS18B20.Ti = 3;     //测量完成定时
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
  unsigned int minutenum=0;
  
  System.Event.Bit.Minute = RESET;
  minutenum = (System.Time.Hour*60u)+System.Time.Minute; 
   
  if(RTU.CaiYangJianGe&&((minutenum%RTU.CaiYangJianGe)==0))
  {
    AddCaiYangTask(0);
  }
  
  if(RTU.JiaBaoTime&&((minutenum%RTU.JiaBaoTime)==0))
  {
    AddCaiYangTask(AFN.JiaBao);
  }
  if((System.Time.Minute%5)==0)
  {
    Message.RainTT.Year = System.Time.Year;             //雨量发报内容更新
    Message.RainTT.Month = System.Time.Month;
    Message.RainTT.Day = System.Time.Day;
    Message.RainTT.Hour = System.Time.Hour;
    Message.RainTT.Minute = System.Time.Minute;
    Message.PT = Rainfall.PT;
    Message.PJ = Rainfall.PJ;
    Message.PN05 = Rainfall.PN05;
    if(Rainfall.PN05 > 254)
    {
      Message.PN05 = 254;
    }
    Message.PN10 = Rainfall.PN10;
    Message.PN30 = Rainfall.PN30;
    
    UpdateDRP();
    SaveDRP();
    
    if(Rainfall.PN05!=0)
    {
      if((Rainfall.YLJBSD==5)&&(Rainfall.PN05>=Rainfall.JBFZ))
      {
        AddCaiYangTask(AFN.JiaBao);
      }
      if(Rainfall.PN05 > 254)
      {
        Rainfall.PN05 -= 254;
      }
      else
      {
        Rainfall.PN05 = 0;
      }
      WriteUInt16(Rainfall.PN05,EEPROM.PN05);
    }
    if((System.Time.Minute%10)==0)
    {
      if(Rainfall.PN10!=0)
      {
        if((Rainfall.YLJBSD==10)&&(Rainfall.PN10>=Rainfall.JBFZ))
        {
          AddCaiYangTask(AFN.JiaBao);
        }
        Rainfall.PN10 = 0;
        WriteUInt16(Rainfall.PN10,EEPROM.PN10);
      }
      if(System.Time.Minute==0||System.Time.Minute==30)
      {
        if(Rainfall.PN30!=0)
        {
          if((Rainfall.YLJBSD==30)&&(Rainfall.PN30>=Rainfall.JBFZ))
          {
            AddCaiYangTask(AFN.JiaBao);
          }
          Rainfall.PN30 = 0;
          WriteUInt16(Rainfall.PN30,EEPROM.PN30);
        }
      }
    }
    if(Display.Status.Flag.Mon)
    {
      DisplayRainfall();
    }
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
  
  if(RTU.DingShiBaoTime&&((System.Time.Hour%RTU.DingShiBaoTime)==0))
  {
    AddCaiYangTask(AFN.DingShi);
  }
  
  if(System.Time.Hour==Rainfall.PDTime)
  {
    if(Rainfall.PJ!=0)
    {
      Rainfall.PJ = 0;
      WriteUInt16(Rainfall.PJ,EEPROM.PJ);
    }
    if(System.Time.Month==1&&System.Time.Day==1)
    {
      Rainfall.PT = 0;
      WriteUInt16(Rainfall.PT,EEPROM.PT);
    }    
    if(Display.Status.Flag.Mon)
    {
      DisplayRainfall();
    }
  }
  AddDRPZ(Flash.Page,Flash.Bytes);
  InitDRP(); 
  Flashadd();
  SaveDRP();
}

//UART数据接收事件处理
void SysEventHandlers_UART0_Rxd(void)
{ 
  unsigned char *p;
#if DEBUG >= 3
  WriteLineStrUART1("UART0 REVE:");
  WriteUART1(UART0_RxdBuff,UART0_RxdLeng);
#endif
  
  if(UART0_RxdLeng < 20)
  {
    if(NULL != Search(UART0_RxdBuff,20,"YZ+CXSS",7))
    {
      AddCaiYangTask(AFN.ChaXunShiShi); 
    }
  }
  
  if((UART0_RxdLeng<40)&&(0X0A==UART0_RxdBuff[UART0_RxdLeng-1])&&(0X0D==UART0_RxdBuff[UART0_RxdLeng-2]))
  {
    if(ATcmdif(UART0_RxdBuff))
    {
      ATcmdconfig();
    }   
  }
  else 
  {
    p = RxdMessageCrc();
    if(NULL!=p)
    {   
      MessageHandlers(p);
    }    
  }
  DeleteUART0_RxdBuff(UART0_RxdLeng);
}

void SysEventHandlers_UART1_Rxd(void)
{ 
  if(NULL!=Search(UART1_RxdBuff,12,"YZ+UART1\x0D\x0A",11))
  {
    WriteLineStrUART1("UART1 OK!");
  }
  else
  {
    RS485RxdDataHandlers();  
  }
  
  DeleteUART1_RxdBuff(UART1_RxdLeng);;
}

//按键输入事件处理
void SysEventHandlers_Key(void)
{
  if(Display.Status.Flag.SetMode)
    return;
  
  System.Event.Bit.Key = RESET;
  if(Display.Status.Flag.Power)
  {
    switch(System.Key)
    {
      case Up:
        {
          AddMessengeTask(AFN.Heartbeat);
        }break;      
      case Down:
        {
          AddCaiYangTask(AFN.CeShi);
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
  if(DS18B20.Event&&(0==DS18B20.Ti))
  {
    DS18B20.Event = RESET;      //温度测量事件复位
    RTU.Temp = Readtemp(ReadDs18B20temp());//读温度
    if(Display.Status.Flag.Mon&&(4==Display.Mon))
    {
      DisplayNum(9,32,RTU.Temp,0X31,space,"",0);
    }   
  }
  
  if(RS485Com.Status.Flag.ComEvent)
  {
    RS485EventHandlers();
  }
  if(Com.Status.Flag.ComEvent)
  {
    ComEventHandlers();
  }
}

/*读取配置信息*/
void ReadSysConfig(void)
{ 
  Rainfall.PT = ReadUInt16(EEPROM.PT);
  Rainfall.PJ = ReadUInt16(EEPROM.PJ);
  Rainfall.PN05 = ReadUInt16(EEPROM.PN05);  
  Rainfall.PN10 = ReadUInt16(EEPROM.PN10);
  Rainfall.PN30 = ReadUInt16(EEPROM.PN30);
  Rainfall.JBFZ = ReadUInt16(EEPROM.RainJBFZ);
  Rainfall.YLJBSD = ReadUInt8(EEPROM.RainJBSD);
  Rainfall.PDTime = ReadUInt8(EEPROM.PDTime);
  Rainfall.Type = ReadUInt8(EEPROM.RainType);
  Rainfall.Resolution = ReadUInt8(EEPROM.RainFBL);
  RTU.RTUType = ReadUInt8(EEPROM.RTUtype);
  RTU.CenterAdd = ReadUInt8(EEPROM.CenterAdd);
  Read_NByte_Randomaddress(RTU.ST,5,EEPROM.ST);
  RTU.WorkMode = ReadUInt8(EEPROM.WorkMode);
  RTU.DingShiBaoTime = ReadUInt8(EEPROM.DingShiBaoTime);
  RTU.JiaBaoTime = ReadUInt8(EEPROM.JiaBaoTime);
  RTU.CaiYangJianGe = ReadUInt16(EEPROM.CaiYangJianGe);
  RTU.CunChuJianGe = ReadUInt16(EEPROM.CunChuJianGe);
  Com.PowerType = ReadUInt8(EEPROM.ComPowerType);
  Com.Preheat = ReadUInt8(EEPROM.ComPreheat);
  Com.CloseDelay = ReadUInt8(EEPROM.ComCloseDelay);
  Com.TxdTime = ReadUInt8(EEPROM.ComTxdTime);
  Com.TxdMax = ReadUInt8(EEPROM.TxdMax);
  Com.bps = ReadUInt8(EEPROM.ComBps);
  RTU.ShuiWeiJiZhi = ReadSInt32(EEPROM.ShuiWeiJiZhi);
  RTU.XiuZhengJiZhi = ReadSInt32(EEPROM.XiuZhengJiZhi);
  RTU.WaterChaLiang = ReadUInt16(EEPROM.WaterChaLiang);
  RS485Com.bps = ReadUInt8(EEPROM.RS485Bps);
  RS485Com.PowerType = ReadUInt8(EEPROM.RS485PowerType);
  RS485Com.Preheat = ReadUInt8(EEPROM.RS485Preheat);
  RS485.CgqType = ReadUInt8(EEPROM.RS485CgqType);
  RS485.Name = ReadUInt8(EEPROM.RS485Name);
  Analog1.Name = ReadUInt8(EEPROM.TD1name);
  Analog1.SType = ReadUInt8(EEPROM.TD1stype);;
  Analog1.Max = ReadSInt32(EEPROM.TD1max);
  Analog1.Min = ReadSInt32(EEPROM.TD1min);
  Analog2.Name = ReadUInt8(EEPROM.TD2name);
  Analog2.SType = ReadUInt8(EEPROM.TD2stype);;
  Analog2.Max = ReadSInt32(EEPROM.TD2max);
  Analog2.Min = ReadSInt32(EEPROM.TD2min);
  Flash.Page = ReadUInt16(EEPROM.Page);
  Flash.Bytes = ReadUInt16(EEPROM.Bytes);
  /*Analog3.Name = ReadUInt8(EEPROM.TD3name);
  Analog3.SType = ReadUInt8(EEPROM.TD3stype);;
  Analog3.Max = ReadSInt32(EEPROM.TD3max);
  Analog3.Min = ReadSInt32(EEPROM.TD3min);
  Analog4.Name = ReadUInt8(EEPROM.TD4name);
  Analog4.SType = ReadUInt8(EEPROM.TD4stype);;
  Analog4.Max = ReadSInt32(EEPROM.TD4max);
  Analog4.Min = ReadSInt32(EEPROM.TD4min);*/
  Height = ReadUInt16(EEPROM.AnZhuangGaoDu);
}

/*数据恢复*/ 
void RestorationData(void)
{
  RTU.Password = 0X1234;

  Display.Mon = 1;
  RS485.Value=0;
  RS485.Max = 999999;
  RS485Com.CloseDelay = 10;
  RS485Com.Com = 1;
  RS485Com.TxdMax = 10;
  RS485Com.TxdTime = 3;
  Com.Com = 0;
  CaiYang.AFN = 0;   
  
  Message.RainTT.Year = System.Time.Year;             //雨量发报内容更新
  Message.RainTT.Month = System.Time.Month;
  Message.RainTT.Day = System.Time.Day;
  Message.RainTT.Hour = System.Time.Hour;
  Message.RainTT.Minute = System.Time.Minute;
  Message.PT = Rainfall.PT;
  Message.PJ = Rainfall.PJ;
  Message.PN05 = Rainfall.PN05;
  Message.PN10 = Rainfall.PN10;
  Message.PN30 = Rainfall.PN30;
  Message.ATT = Message.RainTT;

  ATcmdBasic.ate = RESET;
  AddMessengeTask(AFN.Heartbeat);
  AddCaiYangTask(0);
  DRPZ.sp = 0;
  
  if(FindFlash())    
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(Flash.Page,Flash.Bytes,DRP,40);
    SPI_End();
  }
  if(NULL==Search(DRP,4,(unsigned char*)(&System.Time),4))
  {
    InitDRP();
    Flashadd();
    SaveDRP();
    #if DEBUG>=1
    WriteLineStrUART1("NULL==Search(DRP,4,(unsigned char*)(&System.Time),4)");
    #endif
  }
   
}
  
void ICInit(void)
{
  if(PCF8563Ini())
    SysError.Bit.RTC = RESET;
  else
    SysError.Bit.RTC = SET;
  
  if(FindFlash())
    SysError.Bit.RTC = RESET;
  else
    SysError.Bit.RTC = SET;

} 

/*外设初始化*/
void PeripheralsInit(void)
{
  IniUart0(Com.bps);
  IniUart1(RS485Com.bps);

}

/*系统自检*/
void SystemSelfTest(void)
{

}





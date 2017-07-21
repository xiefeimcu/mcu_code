#include "include.h"
#include "RTU.h"

ClassRTUConfig RTU={0};
ClassAnalog Analog1={0};
ClassAnalog Analog2={0};
//ClassAnalog Analog3={0};
//ClassAnalog Analog4={0};
ClassCaiYang CaiYang={0};
unsigned int Relvtive=0XFFFF;

const ClassYaoSuInfo YaoSuInfo[YAOSULENG]={
  {"NONE",0X10,"FF"},
  {"气温",0X31,"AT"},
  {"水温",0X31,"C"},
  {"气压",0X50,"FL"},
  {"地温",0X31,"GTP"},
  {"湿度",0X31,"MST"},
  {"风向",0X20,"UC"},
  {"风速",0X41,"US"},
  {"河道水位",0X73,"Z"},
  {"库下水位",0X73,"ZB"},
  {"库上水位",0X73,"ZU"}
};

const ClassEEPROM EEPROM={
3,// PT
5,// PJ
7,// PN05
9,// PN10
11,// PN30
13,// RainJBFZ
15,//RainJBSD
16,//PDTime
17,//RainType
18,//RainFBL
19,//RTUtype
20,//CenterAdd
21,//ST
26,//workmode
27,//DingShiBaoTime
28,//JiaBaoTime
29,//caiyangjiange
33,//CunChuJianGe
35,//ShuiWeiJiZhi
39,//XiuZhengJiZhi
31,//ComPowerType
32,//ComPreheat
43,//ComCloseDelay
44,//ComBps
45,//ComTxdTime
46,//TxdMax
49,//WaterChaliang
51,//RS485bps
52,//RS485PowerType
53,//RS485Preheat
54,//RS485CgqType
55,//RS485Name
65,//TD1name
66,//TD1stype
67,//TD1max
71,//TD1min
79,//TD2name
80,//TD2stype
75,//TD2max
81,//TD2min
85,//TD3name
86,//TD3stype
87,//TD3max
91,//TD3min
95,//TD4name
96,//TD4stype
97,//TD4max
101,//TD4min
105,//Page
107,//Bytes
109//安装高度
};

/**************************************************************
函数名称：void TiTimeHandlers(void)
函数介绍：定时时间处理
输入参数：无
输出参数：相关寄存器
返回值  ：无
*************************************************************/
void TiTimeHandlers(void)
{
  if(DS18B20.Ti > 0)
    DS18B20.Ti--;
  
  if(RS485Com.TiPreheat > 0)                 //采样预热计时大于0
  {
    if(--RS485Com.TiPreheat == 0)
    {
      RS485Com.Status.Flag.Online = SET;    //采样设备在线标志置位
    }
  }
  if(RS485Com.TiTxdTime > 0)                 //发送超时定时大于0
  {
    if(--RS485Com.TiTxdTime == 0)
    {
      RS485Com.Status.Flag.Txd = RESET;      //发送状态复位
    }
  }
  if(RS485Com.TiClose > 0)//关闭采样电源
  {
    if(--RS485Com.TiClose==0)//允许关闭采样电源
    {
       CloseComPort(&RS485Com);
    }
  }
  
  if(Com.TiPreheat > 0)                 //通信预热计时大于0
  {
    if(--Com.TiPreheat == 0)
    {
      Com.Status.Flag.Online = True;    //通信设备在线标志置位
    }
  }
  
  if(Com.TiTxdTime > 0)                 //发送超时定时大于0
  {
    if(--Com.TiTxdTime == 0)
    {
      Com.Status.Flag.Txd = False;              //发送状态复位
    }
  }
  if(Com.TiClose > 0)//关闭通信电源
  {
    if(--Com.TiClose==0)//允许关闭通信电源
    {
       CloseComPort(&Com);
    }
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

void AnalogCaiYang(void)
{ 
  if(Analog1.Name!=0)
  {
    Analog1.ADValue = GetADC12(TONGDAO_1,SREF_0);   
    Analog1.Value = AnalogCalc(Analog1.SType,Analog1.ADValue,Analog1.Max,Analog1.Min);
    if((8==Analog1.Name)||(9==Analog1.Name)||(10==Analog1.Name))
    {
      Analog1.Value += RTU.ShuiWeiJiZhi+RTU.XiuZhengJiZhi;
    }
    if(Display.Status.Flag.Mon&&(3==Display.Mon))
    {
      DisplayNum(15-(YaoSuInfo[Analog1.Name].DataType>>4),0,Analog1.Value,YaoSuInfo[Analog1.Name].DataType,space,"",0);
    }
  }
  if(Analog2.Name!=0)
  {
    Analog2.ADValue = GetADC12(TONGDAO_2,SREF_0);
    Analog2.Value = AnalogCalc(Analog2.SType,Analog2.ADValue,Analog2.Max,Analog2.Min);
    if((8==Analog2.Name)||(9==Analog2.Name)||(10==Analog2.Name))
    {
      Analog2.Value += RTU.ShuiWeiJiZhi+RTU.XiuZhengJiZhi;
    }
    if(Display.Status.Flag.Mon&&(3==Display.Mon))
    {
      DisplayNum(15-(YaoSuInfo[Analog2.Name].DataType>>4),16,Analog2.Value,YaoSuInfo[Analog2.Name].DataType,space,"",0);
    }
  }
  /*if(Analog3.Name!=0)
  {
    Analog3.ADValue = GetADC12(TONGDAO_3,SREF_0);
    Analog3.Value = AnalogCalc(Analog3.SType,Analog3.ADValue,Analog3.Max,Analog3.Min);
    if((8==Analog3.Name)||(9==Analog3.Name)||(10==Analog3.Name))
    {
      Analog3.Value += RTU.ShuiWeiJiZhi+RTU.XiuZhengJiZhi;
    }
    if(Display.Status.Flag.Mon&&(3==Display.Mon))
    {
      DisplayNum(15-(YaoSuInfo[Analog3.Name].DataType>>4),32,Analog3.Value,YaoSuInfo[Analog3.Name].DataType,space,"",0);
    }
  }
  if(Analog4.Name!=0)
  {
    Analog4.ADValue = GetADC12(TONGDAO_4,SREF_0);
    Analog4.Value = AnalogCalc(Analog4.SType,Analog4.ADValue,Analog4.Max,Analog4.Min);
    if((8==Analog4.Name)||(9==Analog4.Name)||(10==Analog4.Name))
    {
      Analog4.Value += RTU.ShuiWeiJiZhi+RTU.XiuZhengJiZhi;
    }
    if(Display.Status.Flag.Mon&&(4==Display.Mon))
    {
      DisplayNum(15-(YaoSuInfo[Analog4.Name].DataType>>4),0,Analog4.Value,YaoSuInfo[Analog4.Name].DataType,space,"",0);
    }
  }*/
  
  Message.Analog1 = Analog1.Value;
  Message.Analog2 = Analog2.Value;
 /* Message.Analog3 = Analog3.Value;
  Message.Analog4 = Analog3.Value;*/
  
 /* sendtime();
  WriteLineStrUART1("ADCVALUE:");
  SendNumUART1(Analog2.ADValue,0x50,0);
  WriteLineStrUART1("Analog2.Value:");
  SendNumUART1(Analog2.Value,0x80,0);*/
}

/*******************************************************************************
函数名称：unsigned long int AnalogCalc(unsigned char type,unsigned int advalue,
                                        unsigned long int max,unsigned long int min)
函数介绍：模拟量测值计算
输入参数：unsigned char type---->模拟量类型 0（4-20ma） 1（0-2V）
          unsigned int advalue-->AD测值
          unsigned long int max->实际值最大值
          unsigned long int min->实际值最小值
输出参数：无
返回值  ：实际测量值
*******************************************************************************/
long int AnalogCalc(unsigned char type,unsigned int advalue,long int max,long int min)
{
  double temp=0;
    
  if(STYPE_4_20MA==type)//4-20ma
  {
    if(advalue<=Analog_4ma)
    {
      return min;
    }
    if(advalue>=Analog_20ma)
    {
      return max;
    }
    temp = min+((double)(advalue-Analog_4ma)*(max-min)/(Analog_20ma-Analog_4ma));
  }
  else if(STYPE_0_2V==type)
  {  
    if(advalue<=Analog_0V)
    {
      return min;
    }
    if(advalue>=Analog_2V)
    {
      return max;
    }
    
    temp = min+((double)(advalue-Analog_0V)*(max-min)/(Analog_2V-Analog_0V));
  }
  //最后一位精度四舍五入
  temp *= 10;
  
  if(temp > 0)
  {
    if(((unsigned long int)temp%10)>=5)
      temp += 10;
  }
  else if(temp < 0)
  {
    if(((unsigned long int)(0-temp)%10)>=5)
      temp += 10;
  }
  
  temp /= 10;
      
  return (long)temp;
}

/*******************************************************************************
函数名称：void OperComPort(void) 
函数介绍：打开端口电源
输入参数：ComPortClass *Pcom----->端口地址
输出参数：端口数据、状态标志
返回值  ：无
*******************************************************************************/
void OperComPort(ComPortClass *Pcom) 
{
  unsigned char k=False;                    
  
  if(Pcom->PowerType == PNC)            //常闭
  {
    k = False;
    Pcom->TiClose = 0;                  //关闭电源计时清零
    Pcom->TiPreheat = 0;                //预热计时清零
    Pcom->Status.Flag.Online = True;    //在线标志置位
    Pcom->Status.Flag.Close = True;     //允许关闭电源
  }
  else if(Pcom->PowerType == PAUTO)     //自动
  {
    k = True;
    Pcom->TiClose = 0;                  //关闭电源计时清零
    Pcom->Status.Flag.Close = True;     //允许关闭电源
  }
  else                                  //常开
  {
    k = True;
    Pcom->TiClose = 0;                  //关闭电源计时清零
    Pcom->Status.Flag.Close = False;    //禁止关闭电源
  }
  if(k == True)
  {
    if(Pcom->Com == 0)                  //端口0
    {
      JD2_ON;                           //打开继电器2
    }
    else                                //端口1
    {
      JD1_ON;                           //打开继电器1
    }
    Pcom->Status.Flag.Power = True;     //端口电源状态置位
    if(0 == Pcom->TiPreheat)            //预热时间计时为0
    {
      Pcom->TiPreheat = Pcom->Preheat;  //预热计时初始化
    }
  }
  else
  {
    if(Pcom->Com == 0)                  //端口0
    {
      JD2_OFF;                          //关闭继电器2
    }
    else                                //端口1
    {
      JD1_OFF;                          //关闭继电器2
    }
    Pcom->Status.Flag.Power = False;    //端口电源状态复位
  }
} 

/*******************************************************************************
函数名称：void CloseComPort(void) 
函数介绍：关闭端口电源
输入参数：ComPortClass *Pcom----->端口地址
输出参数：端口数据、状态标志
返回值  ：无
*******************************************************************************/
void CloseComPort(ComPortClass *Pcom) 
{
  Pcom->TiClose = 0;                    //关闭电源计时清零
  if(Pcom->Status.Flag.Close&&(Pcom->PowerType!=PNO)) //允许关闭且非常开
  {
    if(Pcom->Com == 0)                  //端口0
    {
      JD2_OFF;                          //关闭继电器2
    }
    else                                //端口1
    {
      JD1_OFF;                          //关闭继电器1
    }
    Pcom->Status.Flag.Power = False;    //端口电源状态复位
    Pcom->TiPreheat = 0;                //预热计时清零
    if(Pcom->PowerType == PAUTO)        //自动
    {
      Pcom->Status.Flag.Online = False; //在线标志复位
    }    
  }
}

void AddCaiYangTask(unsigned char afn)
{
  if(afn!=0)
  {
    CaiYang.AFN = afn;
  } 

  CaiYang.Event = 1;
  RS485Com.Status.Flag.ComEvent = SET;
  Message.ATT.Year = System.Time.Year;
  Message.ATT.Month = System.Time.Month;
  Message.ATT.Day = System.Time.Day;
  Message.ATT.Hour = System.Time.Hour;
  Message.ATT.Minute = System.Time.Minute;
  
}

void CaiYangHandlers(void)
{ 
  CaiYang.Event = 0;

  AnalogCaiYang();
  if(0==RS485.Name)
  {
    CaiYangOk();
  }
  
}

void CaiYangOk(void)
{
  //unsigned int minutes=0;
  CaiYang.Event = 0;
   
  if(Display.Status.Flag.Mon&&(3==Display.Mon))
  {
    DisplayAnalogData();
  }
  
  //minutes = Message.ATT.Hour*60u+Message.ATT.Minute;
  
  if((Analog1.Name>=8)&&(Analog1.Name<=10))
  {
    if(labs(Analog1.Value - RTU.qwater)>=RTU.WaterChaLiang)
    {
      AddMessengeTask(AFN.JiaBao);
    }
    RTU.qwater = Analog1.Value;
  }
  else if((Analog2.Name>=8)&&(Analog2.Name<=10))
  {
    if(labs(Analog2.Value - RTU.qwater)>=RTU.WaterChaLiang)
    {
      AddMessengeTask(AFN.JiaBao);
    }
    RTU.qwater = Analog2.Value;
  }
  else if((RS485.Name>=8)&&(RS485.Name<=10))
  {
    if(labs(RS485.Value - RTU.qwater)>=RTU.WaterChaLiang)
    {
      AddMessengeTask(AFN.JiaBao);
    }
    RTU.qwater = RS485.Value;
  }
  
  if(CaiYang.AFN!=0)
  {
    AddMessengeTask(CaiYang.AFN);
    CaiYang.AFN = 0;    
  }
  RS485Com.Status.Flag.ComEvent = False;   //读取事件复位
  RS485Com.TiClose = RS485Com.CloseDelay;  
  
  Relvtive = (RTU.qwater - RTU.ShuiWeiJiZhi)/10;//相对水位
  
}

void DefaultConfig(void)
{
  Rainfall.JBFZ = 0;
  Rainfall.PDTime = 8;
  Rainfall.Resolution = 2;
  Rainfall.Type = 1;
  Rainfall.YLJBSD = 5;
  RTU.RTUType = 'K';
  RTU.CenterAdd =0X01;
  RTU.ST[0] = 0X00;
  RTU.ST[1] = 0X00;
  RTU.ST[2] = 0X00;
  RTU.ST[3] = 0X00;
  RTU.ST[4] = 0X00;
  RTU.WorkMode = WMODE_3;
  RTU.DingShiBaoTime = 1;
  RTU.JiaBaoTime = 0;
  RTU.CaiYangJianGe = 10;
  RTU.CunChuJianGe = 60;
  RTU.ShuiWeiJiZhi = 0;
  RTU.XiuZhengJiZhi = 0;
  RTU.WaterChaLiang = 50;
  Com.PowerType = PAUTO;
  Com.bps = BPS_9600;
  Com.Preheat = 120;
  Com.CloseDelay = 20;
  Com.TxdTime = 10;
  Com.TxdMax = 2;
  RS485Com.bps = BPS_4800;
  RS485Com.PowerType = PAUTO;
  RS485Com.CloseDelay = 10;
  RS485Com.Preheat = 60;
  RS485Com.TxdMax = 10;
  RS485Com.TxdTime = 2;
  RS485.Name = 10;
  RS485.CgqType = 1;
  
  Analog1.Name = 0;
  Analog1.SType = STYPE_4_20MA;
  Analog1.Max = 0;
  Analog1.Min = 0;
  Analog2.Name = 0;
  Analog2.SType = STYPE_4_20MA;
  Analog2.Max = 0;
  Analog2.Min = 0;
  Flash.Bytes = 12;
  Flash.Page = 1;
 /* Analog3.Name = 0;
  Analog3.SType = STYPE_4_20MA;
  Analog3.Max = 0;
  Analog3.Min = 0;
  Analog4.Name = 0;
  Analog4.SType = STYPE_4_20MA;
  Analog4.Max = 0;
  Analog4.Min = 0;*/
  Height = 15000;
}

void WriteConfig(void)
{
  WriteUInt8(Rainfall.Resolution,EEPROM.RainFBL);
  WriteUInt8(Rainfall.PDTime,EEPROM.PDTime);
  WriteUInt8(Rainfall.Type,EEPROM.RainType); 
  WriteUInt8(Rainfall.YLJBSD,EEPROM.RainJBSD);
  WriteUInt16(Rainfall.JBFZ,EEPROM.RainJBFZ);
  WriteUInt8(RTU.RTUType,EEPROM.RTUtype);
  WriteUInt8(RTU.CenterAdd,EEPROM.CenterAdd);
  Write_NByte(RTU.ST,5,EEPROM.ST);
  WriteUInt8(RTU.WorkMode,EEPROM.WorkMode);
  WriteUInt8(RTU.DingShiBaoTime,EEPROM.DingShiBaoTime);
  WriteUInt8(RTU.JiaBaoTime,EEPROM.JiaBaoTime);
  WriteUInt16(RTU.CaiYangJianGe,EEPROM.CaiYangJianGe);
  WriteUInt16(RTU.CunChuJianGe,EEPROM.CunChuJianGe);
  WriteSInt32(RTU.ShuiWeiJiZhi,EEPROM.ShuiWeiJiZhi); 
  WriteSInt32(RTU.XiuZhengJiZhi,EEPROM.XiuZhengJiZhi);
  WriteUInt16(RTU.WaterChaLiang,EEPROM.WaterChaLiang);
  WriteUInt8(Com.PowerType,EEPROM.ComPowerType);
  WriteUInt8(Com.Preheat,EEPROM.ComPreheat);
  WriteUInt8(Com.CloseDelay,EEPROM.ComCloseDelay);
  WriteUInt8(Com.TxdTime,EEPROM.ComTxdTime);
  WriteUInt8(Com.TxdMax,EEPROM.TxdMax);
  WriteUInt8(Com.bps,EEPROM.ComBps);
  WriteUInt8(RS485Com.bps,EEPROM.RS485Bps);
  WriteUInt8(RS485Com.PowerType,EEPROM.RS485PowerType);
  WriteUInt8(RS485Com.Preheat,EEPROM.RS485Preheat);
  WriteUInt8(RS485.CgqType,EEPROM.RS485CgqType);
  WriteUInt8(RS485.Name,EEPROM.RS485Name);
  WriteUInt8(Analog1.Name,EEPROM.TD1name);
  WriteUInt8(Analog1.SType,EEPROM.TD1stype);
  WriteSInt32(Analog1.Max,EEPROM.TD1max);
  WriteSInt32(Analog1.Min,EEPROM.TD1min);
  WriteUInt8(Analog2.Name,EEPROM.TD2name);
  WriteUInt8(Analog2.SType,EEPROM.TD2stype);
  WriteSInt32(Analog2.Max,EEPROM.TD2max);
  WriteSInt32(Analog2.Min,EEPROM.TD2min);
  /*WriteUInt8(Analog3.Name,EEPROM.TD3name);
  WriteUInt8(Analog3.SType,EEPROM.TD3stype);
  WriteSInt32(Analog3.Max,EEPROM.TD3max);
  WriteSInt32(Analog3.Min,EEPROM.TD3min);
  WriteUInt8(Analog4.Name,EEPROM.TD4name);
  WriteUInt8(Analog4.SType,EEPROM.TD4stype);
  WriteSInt32(Analog4.Max,EEPROM.TD4max);
  WriteSInt32(Analog4.Min,EEPROM.TD4min);*/
  WriteUInt16(Flash.Page,EEPROM.Page);
  WriteUInt16(Flash.Bytes,EEPROM.Bytes);
  WriteUInt16(Height,EEPROM.AnZhuangGaoDu);
}



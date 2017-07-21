#include "RS485.h"

ComPortClass RS485Com={0};
ClassRs485 RS485={0};
ClassModbus Modbus={0};
unsigned int Height=15000;  //水位计安装高度

/*******************************************************************************
函数名称：void RS485Read(void)
函数介绍：读取RS485
输入参数：无                             
输出参数：无
返回值  ：无
*******************************************************************************/
void RS485Read(void)
{
  if(RS485Com.TxdCount > RS485Com.TxdMax)    //读取计数大于最大读取次数
  {
    RS485Com.Status.Flag.ComEvent = False;   //读取事件复位
    RS485Com.Status.Flag.Txd = False;        //发送状态标志复位    
    RS485Com.TxdCount = 0;                   //发送计数清零
    RS485Com.TiTxdTime = 0;                  //发送超时计时清零

    DisplayRunInfo("Failure");
    DELETE_UART1RXD;
    CaiYangOk();
    return;
  }

  DisplayRunInfo("Read");
  if(Display.Status.Flag.Mon)
  {
    DisplayNum(6,48,RS485Com.TxdCount,20,space,"",0);
  }
  
  RS485SendReadCmd(0X01);                   //发送读取命令
}

/*******************************************************************************
函数名称：void RS485EventHandlers(void)
函数介绍：RS485采样事件处理
输入参数：无                             
输出参数：无
返回值  ：无
*******************************************************************************/
void RS485EventHandlers(void)
{
  if(RS485Com.Status.Flag.Online)            //设备在线
  {    
    if(RS485.Name&&(RS485Com.Status.Flag.Txd == False))    //不在发送状态中
    {
      RS485Read();                           //读取数据
    }
    if(CaiYang.Event)
    {
      CaiYangHandlers();
    }
  }
  else                                       //设备不在线
  {
    if(RS485Com.Status.Flag.Power==False)    //设备电源为关闭状态
    {
      OperComPort(&RS485Com);                //打开设备电源
    }
  }
}

/*******************************************
函数名称：void RS485SendReadCmd(unsigned char st,unsigned char startadd)
功    能：RS485接口发送读命令
参    数：unsigned char st---->设备地址
返回值  ：无
********************************************/
void RS485SendReadCmd(unsigned char st)
{
  DELETE_UART1RXD;                      //完全清除接收缓存
  switch(RS485.CgqType)
  {
    case 1:                             //南控超声波液位计 
      {
        Modbus.SN = 0X01;
        Modbus.AFN = 0X03;
        Modbus.StartSN.num = 0X02;
        Modbus.Lenght.num = 0X03;
        SendModbusCmd();
      }break;
    case 2:                             //安布雷拉超声波
      {
        Modbus.SN = 0X01;
        Modbus.AFN = 0X03;
        Modbus.StartSN.num = 0X00;
        Modbus.Lenght.num = 0X02;
        SendModbusCmd();
      }break; 
    case 3:                             //雷达液位计
      {
        Modbus.SN = 0X01;
        Modbus.AFN = 0X03;
        Modbus.StartSN.num = 0X10;
        Modbus.Lenght.num = 0X01;
        SendModbusCmd();
      }break;     
    case 4:                             //安布雷拉超声波 V1.4 潜山红旗水库
      {
        Modbus.SN = 0X01;
        Modbus.AFN = 0X03;
        Modbus.StartSN.num = 0X00;
        Modbus.Lenght.num = 0X01;
        SendModbusCmd();
      }break;   
    case 5:                             //河海浮子
      {
        Modbus.SN = 0X01;
        Modbus.AFN = 0X03;
        Modbus.StartSN.num = 0;
        Modbus.Lenght.num = 1;
        SendModbusCmd();
      } break;   
    case 6:                         //海川博通雷达液位计 读取液位
        {
            Modbus.SN = 0X01;
            Modbus.AFN = 0X03;
            Modbus.StartSN.num = 3;
            Modbus.Lenght.num = 2;
            SendModbusCmd();
        }break;
      case 7:
      {
            Modbus.SN = 0X01;
            Modbus.AFN = 0X03;
            Modbus.StartSN.num = 0;
            Modbus.Lenght.num = 1;
            SendModbusCmd();
      }break;
      
  }
  RS485Com.Status.Flag.Txd = True;           //发送状态标志置位
  RS485Com.TiTxdTime = RS485Com.TxdTime;     //发送超时计时初始化
  RS485Com.TxdCount++;                       //发送计数
  RS485Com.TiClose = RS485Com.CloseDelay;    //初始化设备掉电延时
  
}

void SendModbusCmd(void)
{
  unsigned char modarray[8]={0};
  Uint16 crccode={0};
  
  modarray[0] = Modbus.SN;
  modarray[1] = Modbus.AFN;
  modarray[2] = Modbus.StartSN.bit16.nHigh;
  modarray[3] = Modbus.StartSN.bit16.nLow;
  modarray[4] = Modbus.Lenght.bit16.nHigh;
  modarray[5] = Modbus.Lenght.bit16.nLow;
  crccode.num = CRC16(modarray,6);
  modarray[6] = crccode.bit16.nHigh;
  modarray[7] = crccode.bit16.nLow;
  WriteUART1(modarray,8);
  
}

/*******************************************
函数名称：unsigned char GetModbusData(unsigned char datalen,unsigned char nbyte,unsigned char minlen)
功    能：从接收缓冲中提取Modbus协议数据
参    数：unsigned int max      ---->最大允许值
          unsigned char nbyte   ---->数据高字节相对设备地址偏移量
          unsigned char minlen  ---->完整帧长度
输出参数：校验成功时将修改RS485通道测值RS485.Value
返回值  ：1----> 成功
          2----> 校验错误
          3----> 非法测值
          0----> 其它错误
********************************************/
unsigned char GetModbusData(unsigned int max,unsigned char nbyte,unsigned char minlen)
{
  Uint16 data16={0};
  unsigned char array[2]={0};
  unsigned char *p_0;  
  
  if(UART1_RxdLeng < minlen)
    return 0;
  
  array[0] = Modbus.SN;
  array[1] = Modbus.AFN;
  
  p_0 = Search(UART1_RxdBuff,20,array,2);
  if(p_0 != NULL)
  {
    data16.num = CRC16(p_0,p_0[2]+3);
    if((data16.bit16.nHigh!=p_0[p_0[2]+3])||(data16.bit16.nLow!=p_0[p_0[2]+4]))
      return 2;
    data16.bit16.nHigh = p_0[nbyte];                //数据获取
    data16.bit16.nLow = p_0[nbyte+1];
    if(data16.num > max)
      return 3;
    
    if(1==RS485.CgqType)//南控超声波 水位=安装高度-测值
    {
      RS485.Value = Height-data16.num;
    }
    else
    {
      RS485.Value = data16.num;
    }
    
  }
  else
  {
    return 0;
  }
  return 1;
}

/*成功收到发回数据处理*/
void RS485RxdDataHandlers(void)
{
  //RS485Com.Status.Flag.Online = True;              //在线标志置位
  RS485Com.TiClose = RS485Com.CloseDelay;            //初始化掉电延时计时

  if(1==RS485GetData())
  {
    DisplayRunInfo("ReadOk!");
    RS485Com.Status.Flag.Txd = False;                //发送状态复位
    RS485Com.TiTxdTime = 0;                          //超时计时清零
    RS485Com.Status.Flag.ComEvent = False;           //读取事件复位
    RS485Com.TxdCount = 0;                           //发送计数清零
    
    if((8==RS485.Name)||(9==RS485.Name)||(10==RS485.Name))
    {
      RS485.Value += RTU.ShuiWeiJiZhi + RTU.XiuZhengJiZhi;
    }
    
    /*if((3==Display.Mon)&&(Display.Status.Flag.Mon))
    {
      DisplayNum(15-(YaoSuInfo[RS485.Name].DataType>>4),32,RS485.Value,YaoSuInfo[RS485.Name].DataType,space,"",0);
    }*/
    
    Message.RS485 = RS485.Value;
    CaiYangOk();
  }
}

/*******************************************
函数名称：unsigned char GetWater(unsigned int *pwater)
功    能：从接收缓冲中提取数据
参    数：无
返回值  ：1----> 成功
          2----> 校验错误
          3----> 非法测值
          0----> 其它错误
********************************************/
unsigned char RS485GetData(void)
{
  unsigned char i=0;
  switch(RS485.CgqType)
  {
    case 1:
      {
       i = GetModbusData(Height,3,11);
      }break; 
    case 2:
      {
        i = GetModbusData(Height/10,3,7);
        if(1==i)
        {
          RS485.Value *=10;
        }
      }break;
    case 3:
      {
        i = GetModbusData(Height/10,3,7);
        if(1==i)
        {
          RS485.Value *=10;
        }
      }break;
    case 4:
      {
        i = GetModbusData(Height/10,3,7);
        if(1==i)
        {
          RS485.Value *=10;
        }
      }break;
      case 5:
        {
           i = GetModbusData(8000,3,7);
            if(1==i)
            {
              RS485.Value *=10;
            }
        }break;
      case 6:
        {
           i = GetModbusData(Height,3,7);
           
        }break;
      case 7:
        {
           i = GetModbusData(Height,3,7);
        }break;
        
  } 
  return i;
}



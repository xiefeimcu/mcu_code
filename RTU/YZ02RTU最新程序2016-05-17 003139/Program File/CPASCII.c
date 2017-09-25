/*********************************************************
*文件名称：CPASCII.c
*摘    要：通信协议编辑报文
*
*当前版本：1.0
*作    者：赵阳
*开始日期：2013年09月02日
*完成日期：
**********************************************************/
#include "include.h"

ClassMessage Message={0};
ClassMessage *pM=&Message;
ComPortClass Com={0}; 
ClassRxdMessage *pRM;
ClassDRPZ DRPZ={0};

const ClassAFN AFN={
0X2F,           //链路维持报
0X30,           //测试报
0X31,           //均有时段水文信息报
0X32,           //遥测站定时报
0X33,           //遥测站加报报
0X34,           //遥测站小时报
0X35,           //遥测站人工置数报
0X36,           //遥测图片报或中心站查询遥测站图片采集信息
0X37,           //中心站查询遥测站实时数据
0X38,           //中心站查询遥测站时段数据
0X39,           //中心站查询遥测站人工置数
0X3A,           //中心站查询遥测指定要素数据
0X40,           //中心站修改遥测站基本配置表
0X41,           //中心站读取遥测站基本配置表
0X42,           //中心站修改遥测站运行参数配置表
0X43,           //中心站读取遥测站运行参数配置表
0X44,           //查询水泵电机实时工作数据
0X45,           //查询遥测终端软件版本
0X46,           //查询遥测站状态和报警信息
0X47,           //初始化固态存储数据
0X48,           //恢复终端机出厂设置
0X49,           //修改密码
0X4A,           //设置遥测站时钟
0XE0,           //读取自定义配置
0XE1            //修改自定义配置
};

void AddDRPZ(unsigned int page,unsigned int bytes)
{
  if(DRPZ.sp >= DRPZLEN)
  {
    UpdateDRPZ();
  }
  DRPZ.Array[DRPZ.sp].Page = page;
  DRPZ.Array[DRPZ.sp].Bytes = bytes;
  DRPZ.sp++;
 
}

void UpdateDRPZ(void)
`{ 
  unsigned char i=0;

  if(0==DRPZ.sp)
    return;
  
  DRPZ.sp--;
  for(i=0;i<DRPZ.sp;i++)
  {
    DRPZ.Array[i].Page = DRPZ.Array[i+1].Page;
    DRPZ.Array[i].Bytes = DRPZ.Array[i+1].Bytes;
  }
  DRPZ.Array[DRPZ.sp].Page = 0;
  DRPZ.Array[DRPZ.sp].Bytes = 0;
}

/*******************************************************************************
函数名称：void ComEventHandlers(void) 
函数介绍：通信事件处理
输入参数：无                             
输出参数：
返回值  ：无
*******************************************************************************/
void ComEventHandlers(void) 
{ 
  if(Com.Status.Flag.Online)            //通信设备在线
  {    
    if(Com.Status.Flag.Txd == False)    //不在发送状态中
    {
      ComSendMessage();                 //发送报文
    }
  }
  else                                  //通信设备不在线
  {
    if(Com.Status.Flag.Power==False)    //通信设备电源为关闭状态
    {
      OperComPort(&Com);                        //打开通信设备
    }
  }
}

/*******************************************************************************
函数名称：void ComSendMessage(void) 
函数介绍：通信设备发送报文
输入参数：无                             
输出参数：通信设备数据、状态标志
返回值  ：无
*******************************************************************************/
void ComSendMessage(void)
{
  if(Com.TxdCount > Com.TxdMax)         //发报计数大于最大重发次数
  {
    Com.Status.Flag.ComEvent = False;   //通信事件复位
    Com.Status.Flag.Txd = False;        //发送状态标志复位    
    Com.TxdCount = 0;                   //发送计数清零
    Com.TiTxdTime = 0;                  //发送超时计时清零
    
    #if DEBUG>=1
    WriteLineStrUART1("发报失败");
    #endif
        
    DisplayRunInfo("发报失败");
    
    return;
  }

  DisplayRunInfo("发报");
  if(Display.Status.Flag.Mon)
  {
    DisplayNum(6,48,Com.TxdCount,20,space,"",0);
  }
  
  SendMessenge();  
    
  Com.Status.Flag.Txd = True;   //发送状态标志置位
  Com.TiTxdTime = Com.TxdTime;  //发送超时计时初始化
  Com.TxdCount++;               //发送计数
  Com.TiClose = Com.CloseDelay; //初始化通信设备掉电延时

}

void AddMessengeTask(unsigned char gnm)
{
  if(Com.Status.Flag.ComEvent)
  {
    if(AFN.Heartbeat==Message.AFN)
    {
      Message.AFN = gnm;
    }
    Com.TxdCount = 0;
  }
  else
  {
    Message.AFN = gnm;
    Com.Status.Flag.ComEvent = SET;
  }
  if((AFN.ChaXunShiShi==Message.AFN)||(AFN.CeShi==Message.AFN))
  {
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
  }
}

void SendMessenge(void)
{
  unsigned int lenght=0;
  
  IE1 &= ~URXIE0;       //关闭接收中断
  DELETE_UART0RXD;      //清除缓存
  lenght = WriteMessage(UART0_RxdBuff); //编辑消息
  WriteUART0(UART0_RxdBuff,lenght);     //发送
  IE1 |= URXIE0;
}


/**************************************************************
函数名称：unsigned int WriteMessage(unsigned char *p)                                    
函数介绍：编辑报文
输入参数：unsigned char *p-------->报文发送缓冲区首地址
输出参数：报文内容
返回值  ：报文长度
*************************************************************/
unsigned int WriteMessage(unsigned char *p)
{
  unsigned int AddNbyte=0;
  
  AddNbyte = WriteMessageHead(p,AddNbyte);      //编辑报头
  AddNbyte = WriteHeartbeatPackets(p,AddNbyte);
  
  if((AFN.CeShi==pM->AFN)||
     (AFN.DingShi==pM->AFN)||
     (AFN.ChaXunShiShi==pM->AFN)||
     (AFN.JiaBao==pM->AFN))
  {
    AddNbyte = WriteMessageTextPublic(p,AddNbyte);
    AddNbyte = WriteMessageTextData(p,AddNbyte);
  }
  else if(AFN.GetRunConfig==pM->AFN)    //上报配置信息
  {
    AddNbyte = WriteMessageTextPublic(p,AddNbyte);
    AddNbyte = WriteMessageConfig(p,AddNbyte);
  }
  else if(AFN.ChaXunShiDuan==pM->AFN)
  {
    AddNbyte = WriteMessageTextPublic(p,AddNbyte);
    AddNbyte = WriteMessageTextLiShiDada(p,AddNbyte);
  }
  else if(AFN.XiaoShi==pM->AFN)
  {
    AddNbyte = WriteMessageTextPublic(p,AddNbyte);
    AddNbyte = WriteMessageTextDRPZ(p,AddNbyte);
  }

  AddNbyte = WriteMessageEnd(p,AddNbyte,ETX);     //编辑报文结束部分*/

  return AddNbyte;
}  


/**************************************************************
函数名称：unsigned int WriteMessageHead(unsigned char *p,unsigned int  AddNbyte)                                    
函数介绍：编辑报头
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
输出参数：报文报头内容
返回值  ：返回报文发送缓冲操作位置
**************************************************************/
unsigned int WriteMessageHead(unsigned char *p,unsigned int  AddNbyte)
{
  unsigned char temp; 
  
  /********************加入帧起始符*************************/
  p[AddNbyte++] = SOH;                                         //帧起始符
  
  /********************加入中心站地址**********************/ 
  p[AddNbyte++] = HEXASCII[RTU.CenterAdd >> 4];     //中心站地址高4位ASCⅡ码
  p[AddNbyte++] = HEXASCII[RTU.CenterAdd & 0X0F];   //中心站地址低4位ASCⅡ码
  
  /********************加入遥测站地址***********************/
  p[AddNbyte++] = HEXASCII[RTU.ST[0] >> 4];         //遥测站地址变量格式为BCD
  p[AddNbyte++] = HEXASCII[RTU.ST[0] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[1] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[1] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[2] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[2] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[3] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[3] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[4] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[4] & 0X0F]; 
  
  /************************加入密码**************************/
  temp = (unsigned char)(RTU.Password >> 8);            //密码高8位  
  p[AddNbyte++] = HEXASCII[temp >> 4];
  p[AddNbyte++] = HEXASCII[temp & 0X0F];
  
  temp = (unsigned char)(RTU.Password & 0X00FF);        //密码低8位  
  p[AddNbyte++] = HEXASCII[temp >> 4];
  p[AddNbyte++] = HEXASCII[temp & 0X0F];
  
  /**********************加入功能码**************************/
  p[AddNbyte++] = HEXASCII[Message.AFN >> 4];                   //功能码高4位ASCⅡ码
  p[AddNbyte++] = HEXASCII[Message.AFN & 0X0F];                 //功能码低4位ASCⅡ码
  
  /*********加入上下行标识符与报文正文长度*******************/
  p[AddNbyte++] = '0';                                          //上行标识符
            
  p[AddNbyte++] = '0';                                          //正文长度 报文正文编辑完后修改
  p[AddNbyte++] = '0';
  p[AddNbyte++] = '0';
  
  /********************加入正文起始符*************************/
  p[AddNbyte++] = STX;                              //传输正文起始符 0X02
  
  return AddNbyte;
}


/****************************************************************************************
函数名称：unsigned int WriteHeartbeatPackets(unsigned char *p,unsigned int  AddNbyte)
函数介绍：编辑心跳包报文正文
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
输出参数：报文正文中的 流水号、发报时间
返回值  ：返回报文发送缓冲操作位置
*****************************************************************************************/
unsigned int WriteHeartbeatPackets(unsigned char *p,unsigned int  AddNbyte)
{
  unsigned char temp;
  static unsigned int snumber=0;                        //流水号初始化为0
    
  if(0 == ++snumber)                                    //计算流水号
    snumber = 1;
  
  /********************加入流水号*************************/
  temp = (unsigned char)(snumber >> 8);                 //流水号转换为ASCⅡ
  p[AddNbyte++] = HEXASCII[temp >> 4];
  p[AddNbyte++] = HEXASCII[temp & 0X0F];
  temp = (unsigned char)(snumber & 0X00FF);
  p[AddNbyte++] = HEXASCII[temp >> 4];
  p[AddNbyte++] = HEXASCII[temp & 0X0F];            
  /********************加入发报时间*************************/
  p[AddNbyte++] = HEXASCII[System.Time.Year / 10]; //年 十进制转换为ASCⅡ
  p[AddNbyte++] = HEXASCII[System.Time.Year % 10];
  p[AddNbyte++] = HEXASCII[System.Time.Month / 10];
  p[AddNbyte++] = HEXASCII[System.Time.Month % 10];
  p[AddNbyte++] = HEXASCII[System.Time.Day / 10];
  p[AddNbyte++] = HEXASCII[System.Time.Day % 10];
  p[AddNbyte++] = HEXASCII[System.Time.Hour / 10];
  p[AddNbyte++] = HEXASCII[System.Time.Hour % 10];
  p[AddNbyte++] = HEXASCII[System.Time.Minute / 10];
  p[AddNbyte++] = HEXASCII[System.Time.Minute % 10];
  p[AddNbyte++] = HEXASCII[System.Time.Second / 10];
  p[AddNbyte++] = HEXASCII[System.Time.Second % 10];
  
  return AddNbyte;  
}

/***********************************************************
函数名称：unsigned int WriteMessageTextPublic(unsigned char *p,unsigned int  AddNbyte)
函数介绍：编辑报文正文公共部分
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
          Time *PT ----->观测时间
输出参数：报文正文公共部分
返回值  ：返回报文发送缓冲操作位置
***********************************************************/
unsigned int WriteMessageTextPublic(unsigned char *p,unsigned int  AddNbyte)
{    
  /********************加入遥测站地址***********************/
  p[AddNbyte++] = 'S';                                          //地址标识符                               
  p[AddNbyte++] = 'T';
  p[AddNbyte++] = ' ';                          
  p[AddNbyte++] = HEXASCII[RTU.ST[0] >> 4];             //遥测站地址变量格式为BCD
  p[AddNbyte++] = HEXASCII[RTU.ST[0] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[1] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[1] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[2] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[2] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[3] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[3] & 0X0F]; 
  p[AddNbyte++] = HEXASCII[RTU.ST[4] >> 4];     
  p[AddNbyte++] = HEXASCII[RTU.ST[4] & 0X0F];    
  /********************加入遥测站分类码***********************/
  p[AddNbyte++] = ' ';
  p[AddNbyte++] = RTU.RTUType;
    
  return AddNbyte;  
}

/***********************************************************
函数名称：unsigned int WriteMessageEnd(unsigned char *p,unsigned int  AddNbyte,unsigned char endcod)
函数介绍：编辑报文结束部分
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
          unsigned char endcod---->ETX(03H)后续无报文  ETB(17H)后续有报文
输出参数：报文结束部分
返回值  ：报文总长度
***********************************************************/
unsigned int WriteMessageEnd(unsigned char *p,unsigned int  AddNbyte,unsigned char endcod)
{
  unsigned int i=0;
  unsigned char temp=0; 
   
  i = AddNbyte - 24;                    //报文正文长度
  
  temp = (unsigned char)(i >> 8);       //更新报头中报文长度
  p[20] = HEXASCII[temp & 0X0F];
  temp = (unsigned char)(i & 0X00FF);
  p[21] = HEXASCII[temp >> 4];
  p[22] = HEXASCII[temp & 0X0F];
  
  p[AddNbyte++] = endcod;               //加入报文结束符
  
  i = CRC16(p,AddNbyte);                //查表法计算校验码
  
  temp = (unsigned char)(i & 0X00FF);
  p[AddNbyte++] = HEXASCII[temp >> 4];
  p[AddNbyte++] = HEXASCII[temp & 0X0F]; 
  temp = (unsigned char)(i >> 8);
  p[AddNbyte++] = HEXASCII[temp >> 4];
  p[AddNbyte++] = HEXASCII[temp & 0X0F];
  
  //p[AddNbyte++] = 0X0D;
  //p[AddNbyte++] = 0X0A;
  if(AFN.ChaXunShiDuan==Message.AFN)
  {
    p[AddNbyte++] = 0X0D;
    p[AddNbyte++] = 0X0A;
  }
     
  return AddNbyte;
}

/*******************************************************************************
函数名称：unsigned int WriteMessageVariable(unsigned char *p,unsigned int  AddNbyte,E_data *pE)
函数介绍：编辑上报要素
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置        
          E_data *pE ----------->上报要素存储地址
输出参数：要素数据
返回值  ：返回报文发送缓冲操作位置
********************************************************************************/
unsigned int WriteMessageVariable(unsigned char *p,unsigned int  AddNbyte,E_data *pE)
{
  unsigned char i=0;
                                              //加入标识符
  p[AddNbyte++] = ' ';                
  for(i=0;i<8;i++)
  {
    if((*pE).keyword[i] != '\0')
      p[AddNbyte++] = pE->keyword[i];
    else
      break;      
  }   

  p[AddNbyte++] = ' ';
  i = SInt32_TO_String(&p[AddNbyte],pE->value,pE->DType,DEZero);//加入要素数据
  
  AddNbyte += i;
  return AddNbyte;
}

/*******************************************************************************
函数名称：unsigned int WriteMessageTextRain(unsigned char *p,unsigned int  AddNbyte)
函数介绍：编辑报文正文降水量部分
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
输出参数：报文正文降水量部分
返回值  ：返回报文发送缓冲操作位置
*******************************************************************************/
unsigned int WriteMessageTextRain(unsigned char *p,unsigned int  AddNbyte)
{
  E_data element={0};                   //上报要素
  
  p[AddNbyte++] = ' '; 
  p[AddNbyte++] = 'T';                                  //观测时间标识符                               
  p[AddNbyte++] = 'T';    
  p[AddNbyte++] = ' ';  
  p[AddNbyte++] = HEXASCII[pM->RainTT.Year / 10];        //年 十进制转换为ASCⅡ
  p[AddNbyte++] = HEXASCII[pM->RainTT.Year % 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Month / 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Month % 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Day / 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Day % 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Hour / 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Hour % 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Minute / 10];
  p[AddNbyte++] = HEXASCII[pM->RainTT.Minute % 10];
  
  element.DType = 0X51;
  element.keyword = "PN05";
  element.value = pM->PN05;
  if(pM->PN05 != 0)
  {
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }  
  element.keyword = "PN10";
  element.value = pM->PN10;
  if(pM->PN10 != 0)
  {
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }  
  element.keyword = "PN30";
  element.value = pM->PN30;
  if(pM->PN30 != 0)
  {
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  element.keyword = "PJ";
  element.value = pM->PJ;
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);

  element.keyword = "PT";
  element.value = pM->PT;
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  return AddNbyte;
}

/*******************************************************************************
函数名称：unsigned int WriteMessageTextRain(unsigned char *p,unsigned int  AddNbyte)
函数介绍：编辑报文正文通道数据部分
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
输出参数：报文正文通道数据部分
返回值  ：返回报文发送缓冲操作位置
*******************************************************************************/
unsigned int WriteMessageTextTongDao(unsigned char *p,unsigned int  AddNbyte)
{
  E_data element={0};                   //上报要素
  
  /*if(0==(Analog1.Name+Analog2.Name+Analog3.Name+Analog4.Name+RS485.Name))//所有通道关闭
  {
    return AddNbyte;
  }*/
  if(0==(Analog1.Name+Analog2.Name+RS485.Name))//所有通道关闭
  {
    return AddNbyte;
  }
  
  p[AddNbyte++] = ' '; 
  p[AddNbyte++] = 'T';                                  //观测时间标识符                               
  p[AddNbyte++] = 'T';    
  p[AddNbyte++] = ' ';  
  p[AddNbyte++] = HEXASCII[pM->ATT.Year / 10];        //年 十进制转换为ASCⅡ
  p[AddNbyte++] = HEXASCII[pM->ATT.Year % 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Month / 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Month % 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Day / 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Day % 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Hour / 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Hour % 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Minute / 10];
  p[AddNbyte++] = HEXASCII[pM->ATT.Minute % 10];
  

  if(Analog1.Name != 0)
  {
    element.DType = YaoSuInfo[Analog1.Name].DataType;
    element.keyword = YaoSuInfo[Analog1.Name].Bzf;
    element.value = pM->Analog1;
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  if(Analog2.Name != 0)
  {
    element.DType = YaoSuInfo[Analog2.Name].DataType;
    element.keyword = YaoSuInfo[Analog2.Name].Bzf;
    element.value = pM->Analog2;
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  /*if(Analog3.Name != 0)
  {
    element.DType = YaoSuInfo[Analog3.Name].DataType;
    element.keyword = YaoSuInfo[Analog3.Name].Bzf;
    element.value = pM->Analog3;
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  if(Analog4.Name != 0)
  {
    element.DType = YaoSuInfo[Analog4.Name].DataType;
    element.keyword = YaoSuInfo[Analog4.Name].Bzf;
    element.value = pM->Analog4;
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }*/
  if(RS485.Name != 0)
  {
    element.DType = YaoSuInfo[RS485.Name].DataType;
    element.keyword = YaoSuInfo[RS485.Name].Bzf;
    element.value = pM->RS485;
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
 
  return AddNbyte;
}

unsigned int WriteMessageTextData(unsigned char *p,unsigned int  AddNbyte)
{
  E_data element={0};                   //上报要素
  
  if(Rainfall.Type!=0)
  {
    AddNbyte = WriteMessageTextRain(p,AddNbyte);
  }
  
  AddNbyte = WriteMessageTextTongDao(p,AddNbyte);
  
  element.keyword = "AI";                               //编辑设备温度
  element.DType = 0X31;
  element.value = RTU.Temp;
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "VT";                               //编辑瞬时气温
  element.DType = 0X42;
  element.value = RTU.Voltage;
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  p[AddNbyte++] = ' ';
  
  return AddNbyte;
}

/*******************************************************************************
函数名称：unsigned int WriteMessageBasicConfig(unsigned char *p,unsigned int  AddNbyte)
函数介绍：编辑自定义配置表正文
输入参数：unsigned char *p-------->报文发送缓冲区首地址
          unsigned int  AddNbyte-->报文发送缓冲操作位置
输出参数：自定义配置正文
返回值  ：返回报文发送缓冲操作位置
*******************************************************************************/
unsigned int WriteMessageConfig(unsigned char *p,unsigned int  AddNbyte)
{
  E_data element={0};                   //上报要素
  
  AddNbyte -= 2;                           //覆盖已经编辑完成的遥测站分类码
  
  element.keyword = "20";               //定时报间隔
  element.DType = 0X20;
  element.value = ReadUInt8(EEPROM.DingShiBaoTime);
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "21";               //加报报间隔
  element.DType = 0X30;
  element.value = ReadUInt8(EEPROM.JiaBaoTime); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "22";               //降水量日起始时间
  element.DType = 0X20;
  element.value = ReadUInt8(EEPROM.PDTime); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "23";               //采样间隔
  element.DType = 0X50;
  element.value = ReadUInt16(EEPROM.CaiYangJianGe); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "24";               //存储间隔
  element.DType = 0X50;
  element.value = ReadUInt16(EEPROM.CunChuJianGe); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "25";               //雨量计分辨力
  element.DType = 0X21;
  element.value = ReadUInt8(EEPROM.RainFBL); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "26";               //雨量加报时段 TODO XF 和标准不符
  element.DType = 0X20;
  element.value = ReadUInt8(EEPROM.RainJBSD); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "27";               //雨量加报阀值
  element.DType = 0X51;
  element.value = ReadUInt16(EEPROM.RainJBFZ); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "28";               //水位基值
  element.DType = 0X73;
  element.value = ReadSInt32(EEPROM.ShuiWeiJiZhi);
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "29";               //水位修正基值
  element.DType = 0X73;
  element.value = ReadSInt32(EEPROM.XiuZhengJiZhi);
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  element.keyword = "30";               //水位加报变率
  element.DType = 0X53;
  element.value = ReadUInt16(EEPROM.WaterChaLiang); 
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  
  p[AddNbyte++] = ' ';
  
  return AddNbyte;
}



/*消息校验，成功返回 帧起始符地址，失败返回 NULL*/
unsigned char* RxdMessageCrc(void)
{
  unsigned int k=0;
  unsigned int i=0;
  unsigned int temp=0;

  for(i=0;i<UART0_RxdLeng;i++)
  {
    if((UART0_RxdLeng-i)<40)
      break;
    
    if(SOH==UART0_RxdBuff[i])
    {
      if((STX==UART0_RxdBuff[i+23])&&((0X38==UART0_RxdBuff[i+19])))
      {
        temp = 0;
        temp = ASCII_Hex(UART0_RxdBuff[i+20]);
        temp <<= 4;
        temp += ASCII_Hex(UART0_RxdBuff[i+21]);
        temp <<= 4;
        temp += ASCII_Hex(UART0_RxdBuff[i+22]);
    
        if((temp+24+i)<(UART0_RxdLeng-4))
        {
          k = UART0_RxdBuff[temp+24+i];
          if((k==ENQ)||(k==EOT)||(k==ACK)||(k==NAK)||(k==ESC))
          {
            k = temp+25+i;
            temp = 0;
            temp = ASCII_Hex(UART0_RxdBuff[k]);
            temp <<= 4;
            temp += ASCII_Hex(UART0_RxdBuff[k+1]);
            temp <<= 4;
            temp += ASCII_Hex(UART0_RxdBuff[k+2]);
            temp <<= 4;
            temp += ASCII_Hex(UART0_RxdBuff[k+3]);
            temp = (temp>>8)|(temp<<8); 
            
            k -= i;
            if(temp==CRC16(&UART0_RxdBuff[i],k))
            {
              return &UART0_RxdBuff[i];
            }
            else
            {
              #if  DEBUG>=3
              WriteLineStrUART1("CRC ERROR!");
              #endif
              return NULL;
            }
          }
        }       
      }
    }
  }
  
  #if  DEBUG>=3
  WriteLineStrUART1("ACK ERROR");
  #endif
  
  return NULL;
}

/*******************************************************************************
函数名称：void MessageHandlers(void)
函数介绍：收到消息处理
输入参数：无
输出参数：无
返回值  ：无
*******************************************************************************/
void MessageHandlers(unsigned char *p)
{
  ClassRxdMessage Messageinfo={0};
  unsigned char i=0;
  
  pRM = &Messageinfo;
  
  Com.Status.Flag.Online = True;                //在线标志置位
  Com.TiClose = Com.CloseDelay;                 //初始化掉电延时计时
    
  pRM->ST[0] = ToAscii_Hex(p[1],p[2]);          //遥测站地址
  pRM->ST[1] = ToAscii_Hex(p[3],p[4]);
  pRM->ST[2] = ToAscii_Hex(p[5],p[6]);
  pRM->ST[3] = ToAscii_Hex(p[7],p[8]);
  pRM->ST[4] = ToAscii_Hex(p[9],p[10]);
  
  for(i=0;i<5;i++)
  {
    if(pRM->ST[i]!=RTU.ST[i])
    {
      #if DEBUG >= 3
      WriteLineStrUART1("ST ERROR");
      #endif
      return;
    }    
  }
  
  pRM->CenterAdd = ToAscii_Hex(p[11],p[12]);//中心站地址
  pRM->PassWord = ToAscii_Hex(p[13],p[14]); //密码
  pRM->PassWord <<= 8;
  pRM->PassWord += ToAscii_Hex(p[15],p[16]);
  pRM->AFN = ToAscii_Hex(p[17],p[18]);          //功能码
  pRM->TextLeng = ToAscii_Hex(p[21],p[22]);     //正文长度
  
  if(pRM->PassWord != RTU.Password)
  {
    #if DEBUG >= 3
    WriteLineStrUART1("Password ERROR");
    #endif
    return;
  }
  
  pRM->RxdTime.Year = ToAscii_U8(p[28],p[29]);
  pRM->RxdTime.Month = ToAscii_U8(p[30],p[31]);
  pRM->RxdTime.Day = ToAscii_U8(p[32],p[33]);
  pRM->RxdTime.Hour = ToAscii_U8(p[34],p[35]);
  pRM->RxdTime.Minute = ToAscii_U8(p[36],p[37]);
  pRM->RxdTime.Second = ToAscii_U8(p[38],p[39]);
  
  if(False == TimeCheck(pRM->RxdTime))   //时间数据非法
  {
    #if DEBUG >= 3
    WriteLineStrUART1("Time ERROR");
    #endif
    return;
  }
  
  pRM->End = p[pRM->TextLeng+24];
  
  Com.Status.Flag.ComEvent = RESET;   //通信事件复位
  Com.Status.Flag.Txd = RESET;        //发送状态标志复位    
  Com.TxdCount = 0;                   //发送计数清零
  Com.TiTxdTime = 0;                  //发送超时计时清零
  
  if(ESC==pRM->End)                     //传输结束，终端保持在线
  {
    Com.Status.Flag.Close = RESET;      //禁止关闭通信电源
    #if DEBUG >= 3
    WriteLineStrUART1("ESC");
    #endif
  }
  else if(EOT==pRM->End)                //传输结束，退出
  {
    Com.Status.Flag.Close = SET;        //允许关闭通信电源  
    #if DEBUG >= 3
    WriteLineStrUART1("EOT");
    #endif
  }
  else if(ENQ==pRM->End)                //中心站远程功能
  {
    Com.Status.Flag.Close = RESET;      //禁止关闭通信电源
    #if DEBUG >= 3
    WriteLineStrUART1("ENQ");
    #endif
    if(AFN.ChaXunShiShi==pRM->AFN)      //读取实时数据
    {
      AddCaiYangTask(AFN.ChaXunShiShi); 
    }
    else if(AFN.GetRunConfig==pRM->AFN) //读取配置信息
    {
      AddMessengeTask(AFN.GetRunConfig); 
    }
    else if(AFN.SetRunConfig==pRM->AFN)//修改配置信息
    {
      SetRunConfig(&p[40]);
      AddMessengeTask(AFN.GetRunConfig); 
    }
    /*else if(AFN.ChaXunShiDuan==pRM->AFN)
    {
      Flash.Satrtime.Year = ToAscii_U8(p[40],p[41]);
      Flash.Satrtime.Month = ToAscii_U8(p[42],p[43]);
      Flash.Satrtime.Day = ToAscii_U8(p[44],p[45]);
      Flash.Satrtime.Hour = ToAscii_U8(p[46],p[47]);
      Flash.Satrtime.Minute = ToAscii_U8(p[48],p[49]);
      Flash.Endtime.Year = ToAscii_U8(p[50],p[51]);
      Flash.Endtime.Month = ToAscii_U8(p[52],p[53]);
      Flash.Endtime.Day = ToAscii_U8(p[54],p[55]);
      Flash.Endtime.Hour = ToAscii_U8(p[56],p[57]);
      Flash.Endtime.Minute = ToAscii_U8(p[58],p[59]);
      
      if(TimeCheck(Flash.Satrtime)&&TimeCheck(Flash.Endtime))
      {
        Message.AFN = AFN.ChaXunShiDuan;
        System.Event.Bit.Flash = SET;
      }           
    }*/
    else if(0XDF==pRM->AFN)             //复位重启
    {
      WriteLineStrUART0("Reset...\x0d\x0a");
      delayms(5000);
      WDTCTL = 0;  
    }
  }
  if(pRM->End!=ENQ)           //同步时钟
  {
    System.Time = pRM->RxdTime;
    SetTime(&System.Time);
    #if DEBUG >= 3
    WriteLineStrUART1("Set Time");
    #endif
    if((AFN.XiaoShi==pRM->AFN)&&(AFN.XiaoShi==pM->AFN))//应答报文功能码为小时报 且 前一次发送的报文为小时报
    {
      UpdateDRPZ();           //更新小时报缓存
    }
    if(DRPZ.sp)                 //小时报缓存不为空 
    { 
      AddMessengeTask(AFN.XiaoShi);
      DRPZ.Now = DRPZ.Array[0];
    }
  }
  DisplayRunInfo("发报成功");
}

unsigned char* SetConfiginfo(unsigned char *p)
{
  unsigned char id=0;
  long value=0;
  unsigned char i=0;
  
  for(i=0;i<5;i++)
  {
    if(*p < 0X1C)
      return NULL;
    if(*p != ' ') 
      break;
    
    p++;
  }
  if(i >= 5)
    return NULL;
  
  id = ToAscii_Hex(p[0],p[1]);
  p += 2;
  
  for(i=0;i<5;i++)
  {
    if(*p < 0X1C)
      return NULL;
    if(*p != ' ') 
      break;
    
    p++;
  }
  if(i >= 5)
    return NULL;
  
  for(i=0;i<10;i++)
  {
    if(p[i] < 0X1C)
      return NULL;
    if(' '==p[i])
      break;
  }
  if(i >= 10)
    return NULL;

  switch(id)
  {
    case 0X20:
      {
        value = AtoLong(p,i,0X20);
        WriteUInt8((unsigned char)value,EEPROM.DingShiBaoTime);

      }break;
    case 0X21:
      {
        value = AtoLong(p,i,0X30);
        WriteUInt8((unsigned char)value,EEPROM.JiaBaoTime);

      }break;
    case 0X22:
      {
        value = AtoLong(p,i,0X20);
        WriteUInt8((unsigned char)value,EEPROM.PDTime);
      }break;
    case 0X23:
      {
        value = AtoLong(p,i,0X50);
        WriteUInt16((unsigned int)value,EEPROM.CaiYangJianGe);
      }break;
    case 0X24:
      {
        value = AtoLong(p,i,0X50);
        WriteUInt16((unsigned int)value,EEPROM.CunChuJianGe);
      }break;
    case 0X25:
      {
        value = AtoLong(p,i,0X21);
        WriteUInt8((unsigned char)value,EEPROM.RainFBL);
      }break;
    case 0X26:
      {
        value = AtoLong(p,i,0X20);
        WriteUInt8((unsigned char)value,EEPROM.RainJBSD);
      }break;
    case 0X27:
      {
        value = AtoLong(p,i,0X51);
        WriteUInt16((unsigned int)value,EEPROM.RainJBFZ);
      }break;
    case 0X28:
      {
        value = AtoLong(p,i,0X73);
        WriteSInt32(value,EEPROM.ShuiWeiJiZhi); 
      }break;
    case 0X29:
      {
        value = AtoLong(p,i,0X73);
        WriteSInt32(value,EEPROM.XiuZhengJiZhi);
      }break;
    case 0X30:
      {
        value = AtoLong(p,i,0X53);
        WriteUInt16((unsigned int)value,EEPROM.WaterChaLiang);
      }break;
  }
  
  p += i;
  return p;
}

void SetRunConfig(unsigned char *p)
{
  unsigned char i=0;
  
  for(i=0;i<12;i++)
  {
    p = SetConfiginfo(p);
    if(NULL==p)
    {
      #if DEBUG>=3
        WriteLineStrUART1("NULL==p");
      #endif
        return;
    }     
  }
}


unsigned int WriteMessageTextLiShiDada(unsigned char *p,unsigned int  AddNbyte)
{
  E_data element={0};
  
  p[AddNbyte++] = ' '; 
  p[AddNbyte++] = 'T';                                  //观测时间标识符                               
  p[AddNbyte++] = 'T';    
  p[AddNbyte++] = ' ';  
  p[AddNbyte++] = HEXASCII[LsData.TT.Year / 10];        //年 十进制转换为ASCⅡ
  p[AddNbyte++] = HEXASCII[LsData.TT.Year % 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Month / 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Month % 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Day / 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Day % 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Hour / 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Hour % 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Minute / 10];
  p[AddNbyte++] = HEXASCII[LsData.TT.Minute % 10];
   
  element.keyword = "PT";
  element.value = LsData.PT;
  element.DType = 0X51;
  AddNbyte = WriteMessageVariable(p,AddNbyte,&element);  
  
  if(Analog1.Name != 0)
  {
    element.DType = YaoSuInfo[Analog1.Name].DataType;
    element.keyword = YaoSuInfo[Analog1.Name].Bzf;
    element.value = AnalogCalc(Analog1.SType,LsData.A1,Analog1.Max,Analog1.Min);
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  if(Analog2.Name != 0)
  {
    element.DType = YaoSuInfo[Analog2.Name].DataType;
    element.keyword = YaoSuInfo[Analog2.Name].Bzf;
    element.value = AnalogCalc(Analog2.SType,LsData.A2,Analog2.Max,Analog2.Min);
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  if(RS485.Name != 0)
  {
    element.DType = YaoSuInfo[RS485.Name].DataType;
    element.keyword = YaoSuInfo[RS485.Name].Bzf;
    element.value = LsData.Rs485;
    AddNbyte = WriteMessageVariable(p,AddNbyte,&element);
  }
  
  return AddNbyte;
}

unsigned int WriteMessageTextDRPZ(unsigned char *p,unsigned int  AddNbyte)
{
  unsigned char a[40]={0XFF};

  unsigned char i=0;
  
  if((0==DRPZ.Now.Page)||(0==DRPZ.Now.Bytes))
    return AddNbyte;
  
  memset(a,0XFF,sizeof(a));
  
  a[0] = System.Time.Year;
  a[1] = System.Time.Month;
  a[2] = System.Time.Day;
  a[3] = System.Time.Hour;
  
  if(FindFlash())
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(DRPZ.Now.Page,DRPZ.Now.Bytes,a,40);
    SPI_End();
  }
  else
  {
    return AddNbyte;
  }
  
  p[AddNbyte++] = ' '; 
  p[AddNbyte++] = 'T';                        //观测时间标识符                               
  p[AddNbyte++] = 'T';    
  p[AddNbyte++] = ' ';  
  p[AddNbyte++] = HEXASCII[a[0] / 10];        //年 十进制转换为ASCⅡ
  p[AddNbyte++] = HEXASCII[a[0] % 10];
  p[AddNbyte++] = HEXASCII[a[1] / 10];
  p[AddNbyte++] = HEXASCII[a[1] % 10];
  p[AddNbyte++] = HEXASCII[a[2] / 10];
  p[AddNbyte++] = HEXASCII[a[2] % 10];
  p[AddNbyte++] = HEXASCII[a[3] / 10];
  p[AddNbyte++] = HEXASCII[a[3] % 10];
  p[AddNbyte++] = HEXASCII[0];
  p[AddNbyte++] = HEXASCII[0];
  if(Rainfall.Type)             //雨量计接入
  {
    p[AddNbyte++] = ' '; 
    p[AddNbyte++] = 'D'; 
    p[AddNbyte++] = 'R'; 
    p[AddNbyte++] = 'P'; 
    p[AddNbyte++] = ' '; 
    for(i=4;i<16;i++)
    {
      p[AddNbyte++] = HEXASCII[a[i]>>4];
      p[AddNbyte++] = HEXASCII[a[i]&0X0F];
    }
  }
  if(RTU.RTUType!=RTUTYPE_JS)
  {
    p[AddNbyte++] = ' '; 
    p[AddNbyte++] = 'D'; 
    p[AddNbyte++] = 'R'; 
    p[AddNbyte++] = 'Z'; 
    p[AddNbyte++] = '1'; 
    p[AddNbyte++] = ' ';
    for(i=16;i<40;i++)
    {
      p[AddNbyte++] = HEXASCII[a[i]>>4];
      p[AddNbyte++] = HEXASCII[a[i]&0X0F];
    }
  }
  
  p[AddNbyte++] = ' ';
  return AddNbyte;
}


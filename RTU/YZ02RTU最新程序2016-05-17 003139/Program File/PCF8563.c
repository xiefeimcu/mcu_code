//====================================================================================
//                       PCF8563.C With MSP430F149 Driver
//Platform: IAR Embedded Workbench Version: 3.42A
//版本: 1.0.0
//作者:  李怀良
//Email:   li-huai-liang@163.com
//QQ:      769996244
//日期:    2009-03-23
//修改:    赵阳 2013-04-17
///***********************************************************************************/


#include "IIC.h"
#include "PCF8563.h" 
#include "Public.h"
#include "include.h"

/********************************************
输出数据->pcf8563
********************************************/
void PCF8563writeData(unsigned char address,unsigned char mdata)
{
   P1DIR |= IICSCL;  //define to output 
   IICSDAOut(); //define to output
   IICStart();
   IICWriteByte(0xa2); //写命令
   IICWriteByte(address); //写地址
   IICWriteByte(mdata); //写数据
   IICStop();

}

/********************************************
输入数据<-pcf8563
********************************************/
/*unsigned char PCF8563ReadData(unsigned char address) //单字节
{ 
   unsigned char rdata;
   P1DIR |= IICSCL;  //define to output 
   IICSDAOut(); //define to output
   IICStart();
   IICWriteByte(0xa2); //写命令
   IICWriteByte(address);//写地址
   IICStart();
   IICWriteByte(0xa3); //读命令
   rdata=IICReadByteWithAck();
   //WriteACK(1);
   IICStop();
   return(rdata);
}*/

/********************************************************************************************************
** 函数名称: PCF8563WriteData
** 功能描述: 
** 输　入: 无
** 输　出: 无
** 全局变量: 无
*-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
/*unsigned char PCF8563WriteData(unsigned char address,unsigned char *p,unsigned char count)
{
    unsigned char i;
    
    P1DIR |= IICSCL;  //define to output 
    IICSDAOut(); //define to output
  
    IICStart();
    
    if(IICWriteByte(0XA2))return(1);
    if(IICWriteByte(address))return(1);
    
    for(i=0;i<count;i++)
    {
        if(IICWriteByte(*p))return(1);
        p++;
    };
    
    IICStop();
    return(0);
}*/

/********************************************************************************************************
** 函数名称: PCF8563ReadData
** 功能描述: 
** 输　入: 无
** 输　出: 无
** 全局变量: 无
*-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
unsigned char PCF8563ReadDatas(unsigned char address,unsigned char  *p,unsigned char count)
{
    unsigned char i;

    P1DIR |= IICSCL;  //define to output 
    IICSDAOut(); //define to output
    PCF8563delayus(20);
    IICStart();
    PCF8563delayus(20);
    if(IICWriteByte(0xA2)) return(1);
    if(IICWriteByte(address)) return(1);
    PCF8563delayus(20);
    IICStart();
    if(IICWriteByte(0xA3))return(1);
    PCF8563delayus(20);
    for(i=1;i<count;i++)
    {
        *p=IICReadByteWithAck();PCF8563delayus(20);
        p++;  
    };
    *p=IICReadByteWithoutAck();
    PCF8563delayus(20);
    IICStop();
    PCF8563delayus(20);
      
    return(0); 
}

/***************************************************
PCF8563初始化，关闭所有报警
成功检测到器件返回1，
未检测到器件返回0.
**************************************************/
unsigned char PCF8563Ini(void)
{
  unsigned char timebcd[7]={0};
  
  PCF8563writeData(0x01,0x02);		 //关闭报警中断
  PCF8563writeData(0x09,0x00);		 //00分报警 
  PCF8563writeData(0x0A,0x80);		 
  PCF8563writeData(0x0B,0x80);
  PCF8563writeData(0x0C,0x80);

  PCF8563writeData(week,0X01);  //设置星期为 1
  PCF8563delayus(1000);         //延时1ms
  PCF8563ReadDatas(second,timebcd,7);   //读取时间
  timebcd[4] &= 0X07;                   //星期
  
  if(timebcd[4]==1)
    return 1;                   //返回成功
  else
    return 0;                   //返回失败
  
}


/*******************************************
函数名称：unsigned char ReadTime(DataTime *t)
功    能：读取系统时间，把时间t设置为RTC当前时间
参    数：struct Timestamp *t---->时间
返回值  ：成功返回1，失败返回0.
********************************************/
unsigned char ReadTime(DataTime *t)
{
  unsigned char timebcd[7]={0};
  unsigned char i=0;            
   
  do
  {
    i++;
    PCF8563ReadDatas(second,timebcd,7);   //读取时间
    timebcd[0] &= 0X7F;                   //秒             无效位处理
    timebcd[1] &= 0X7F;                   //分钟
    timebcd[2] &= 0X3F;                   //小时
    timebcd[3] &= 0X3F;                   //日
    timebcd[4] &= 0X07;                   //星期
    timebcd[5] &= 0X1F;                   //月
     
/*************将时钟BCD码转换为8bit整型************/
    (*t).Year = BCD_Byte(timebcd[6]); //年
    (*t).Month = BCD_Byte(timebcd[5]);//月
    (*t).Day = BCD_Byte(timebcd[3]);         
    (*t).Hour = BCD_Byte(timebcd[2]);
    (*t).Minute = BCD_Byte(timebcd[1]);
    (*t).Second = BCD_Byte(timebcd[0]);
    
    if(1==TimeCheck(*t))                  //读出的时间合法
      return 1;                          //返回读取成功
       
  }while(i < 5);                        //读出的时间不合法 最大重读5次
  return 0;                             //返回读取失败
  
}

/*******************************************
函数名称：unsigned char SetTime(DataTime *t)
功    能：设置系统时间，把时间t设置为系统当前时间
参    数：DataTime *t---->时间
返回值  ：成功返回1，失败返回0.
********************************************/
void SetTime(DataTime *t)
{
  PCF8563writeData(year,Byte_BCD(t->Year));
  PCF8563writeData(month,Byte_BCD(t->Month));
  PCF8563writeData(day,Byte_BCD(t->Day));
  PCF8563writeData(hour,Byte_BCD(t->Hour));
  PCF8563writeData(minute,Byte_BCD(t->Minute));
  PCF8563writeData(second,Byte_BCD(t->Second));
  
}

/*******************************************
函数名称：unsigned int TimeInterval(struct Timestamp *t0,struct Timestamp *t1)
功    能：时间间隔计算
参    数：struct Timestamp *t0 ----> 当前时间
          struct Timestamp *t1 ----> 计时起点时间
返回值  ：时间间隔 单位秒
********************************************/
/*unsigned int TimeInterval(Time *t0,Time *t1)
{
  unsigned int timevalue;
 
  //小时计算
  if((*t0).Hour < (*t1).Hour)           //不在同一天 相隔小时数
  {
    timevalue = (24-(*t1).Hour) + (*t0).Hour;
  }
  else          //在同一天 相隔小时数
  {
    timevalue = (*t0).Hour - (*t1).Hour;
  }
  timevalue *= 60;
  //分钟计算
  if((*t0).Minute < (*t1).Minute)
  {
    timevalue -= (*t1).Minute - (*t0).Minute;		
  }
  else
  {
    timevalue += (*t0).Minute - (*t1).Minute;  
  }
  timevalue *= 60;
  //秒钟计算
  if((*t0).Second < (*t1).Second)
  {
    timevalue -= (*t1).Second - (*t0).Second;		
  }
  else
  {
    timevalue += (*t0).Second - (*t1).Second;  
  }
  
  return timevalue;
}*/



/*void timeini(void)
{
  PCF8563writeData(year,0X13);  
  PCF8563writeData(month,0X05); 
  PCF8563writeData(day,0X13); 
  PCF8563writeData(week,0X05); 
  PCF8563writeData(hour,0X10); 
  PCF8563writeData(minute,0X05); 
}*/




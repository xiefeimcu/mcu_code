/*********************************************************
*文件名称：Display.c
*摘    要：显示功能函数
*
*作    者：赵阳
*开始日期：2013年4月16日
*完成日期：
**********************************************************/
#include "Display.h"
#include "include.h"

DisplayClass Display={0};

void LCDOFF(void)
{
  ClearScreen;                  //清屏 
  LCD_POWER_OFF;                
  LCD_LIGHT_OFF;                //LCD背光关
  Display.Status.Register = 0;  //显示状态寄存器清零
  LCD_DataIn;
  LCD_CMIn;
  delayms(500);
  System.Key = 0;
  System.Event.Bit.Key = RESET;
  StopU0();
}
 
void LCDON(void)
{
  Ini_Lcd();                    //显示设备初始化  
  DisplayMon();                 //显示监测界面
  Display.TiOFF = OFFLCDDELAY;  //初始化关闭计时
}

void DisplayRunInfo(char *infostr)
{
  if(Display.Status.Flag.Mon)
  {
    Draw(0,48,8,16,0,0);
    DisplayString(0,48,infostr,0);
  }
}


/*******************************************************************************
函数名称：void DisplayMon(void) 
函数介绍：显示监测界面
输入参数：无                             
输出参数：无
返回值  ：无
*******************************************************************************/
void DisplayMon(void)
{ 
  if(Display.Mon > 5)
    Display.Mon = 1;
  if(Display.Mon < 1)
    Display.Mon = 5;
  
  Display.Status.Flag.Mon = SET;         //监测界面标志置位
  Display.Status.Flag.Menu = RESET;      //菜单界面标志复位
  //Draw(0,0,16,48,0,0);                        //清除监测信息显示区
  ClearScreen;
  if(0==Rainfall.PN30)
  {
    Draw(0,0,2,16,0,qingtian);
  }
  else
  {
    DisplayString(0,0,"将",0);
  } 
  if(AlarmMode.Status.Flag.Run)
  {
    Draw(2,0,2,16,0,baojing);
  }
  if(LI==AlarmLevel.Level)
  {
    DisplayString(4,0,"立即转移",0);
  }
  else if(LII==AlarmLevel.Level)
  {
    DisplayString(4,0,"准备转移",0);
  }
  else if(LIII==AlarmLevel.Level)
  {
    DisplayString(4,0,"警戒报警",0);
  }

  if(1 == Display.Mon)
  {
    DisplayString(0,16," 5分钟将",0);     //显示文字提示             
    DisplayString(0,32,"10分钟将",0);  
    DisplayString(0,48,"30分钟将",0);
    DisplayRainfall();
  }
  else if(2 == Display.Mon)
  {
    DisplayString(0,16," 1小时将",0);
    DisplayString(0,32," 3小时将",0);
    DisplayString(0,48," 6小时将",0);
    DisplayRainfall();
  }
  else if(3 == Display.Mon)
  {     
    DisplayString(0,16,"12小时将",0);
    DisplayString(0,32,"24小时将",0);
    DisplayString(0,48,"  今日将",0);
    DisplayRainfall();
  }
  else if(4 == Display.Mon)
  {
    DisplayString(0,16,"本场将",0);
    DisplayString(0,32,"累计将",0);
    DisplayRainfall();
  }
  else if(5 == Display.Mon)
  {
    DisplayString(0,16,"电源电压",0);
    DisplayString(0,32,"设备温度",0);
    DisplayMonData();                   //显示监测数据
  }
  Battery(14,0,System.Voltage);            //显示电池
  DisplaySysTime();

}

/*显示降雨量*/
void DisplayRainfall(void)
{
  if(Display.Status.Flag.Mon&&Display.Mon<5)
  {
    if(0==Rainfall.PN30)
    {
      Draw(0,0,2,16,0,qingtian);
    }
    else
    {
      DisplayString(0,0,"将",0);
    } 
    if(1==Display.Mon)
    {
      DisplayNum(8,16,Rainfall.PN05,0X51,space,"mm",AlarmValueif(&Alarm.PN05,Rainfall.PN05));
      DisplayNum(8,32,Rainfall.PN10,0X51,space,"mm",AlarmValueif(&Alarm.PN10,Rainfall.PN10));
      DisplayNum(8,48,Rainfall.PN30,0X51,space,"mm",AlarmValueif(&Alarm.PN30,Rainfall.PN30));
    }
    else if(2==Display.Mon)
    {
      DisplayNum(8,16,Rainfall.P1,0X51,space,"mm",AlarmValueif(&Alarm.P1,Rainfall.P1));
      DisplayNum(8,32,Rainfall.P3,0X51,space,"mm",AlarmValueif(&Alarm.P3,Rainfall.P3));
      DisplayNum(8,48,Rainfall.P6,0X51,space,"mm",AlarmValueif(&Alarm.P6,Rainfall.P6));
    }
    else if(3==Display.Mon)
    {
      DisplayNum(8,16,Rainfall.P12,0X51,space,"mm",AlarmValueif(&Alarm.P12,Rainfall.P12));
      DisplayNum(8,32,Rainfall.P24,0X51,space,"mm",AlarmValueif(&Alarm.P24,Rainfall.P24));
      DisplayNum(8,48,Rainfall.PJ,0X51,space,"mm",AlarmValueif(&Alarm.PJ,Rainfall.PJ));
    }
    else if(4==Display.Mon)
    {
      DisplayNum(8,16,Rainfall.P,0X51,space,"mm",AlarmValueif(&Alarm.P,Rainfall.P));
      DisplayNum(8,32,Rainfall.PT,0X51,space,"mm",AlarmValueif(&Alarm.PT,Rainfall.PT));
    }
  }
}

void DisplayMonData(void)
{
  if(Display.Status.Flag.Mon&&(Display.Mon==5))
  {
    System.Voltage = GetVcc();
    DisplayNum(9,16,System.Voltage,0X42,space,"V",0);          //显示电源电压 
    DisplayNum(9,32,System.Temp,0X31,space,"℃",0);     //显示设备温度
  }
}

void DisplaySysTime(void)
{
  char timestring[6]={'0','0',':','0','0','\0'};
  
  timestring[0] = HEXASCII[System.Time.Hour/10];
  timestring[1] = HEXASCII[System.Time.Hour%10];
  timestring[3] = HEXASCII[System.Time.Minute/10];
  timestring[4] = HEXASCII[System.Time.Minute%10];

  Display_8x6(12,7,timestring);
}


void Battery(unsigned char H,unsigned char Ver, unsigned int dianya)
{
  unsigned char level=0;
  unsigned char i=0;
  unsigned int val=370;
  
  for(i=0;i<10;i++)                    
  {
    if(dianya > val)
    {
      val += 4;
    }
    else break;
  }
  level=i;   
  
  DisBattery(H,Ver,level);
}

/***************************************************************
                    电量显示
X的范围0-10；将总电量分为10等分显示。0显示空电量，10显示满格，
x>10显示故障。
****************************************************************/
void DisBattery(char H,char Ver,char power)
{
  unsigned int a[3]={0x1ffe,0x7002,0x5002};    //空电量显示数据
  unsigned int temp;
  char i;
  char xadd,yadd;
  
  temp=0x0002;
      
  for(i=0;i<power;i++)                                //电量显示数据处理
  { 
    temp <<= 1;                                       //电量每增加一级，显示数据从低位到高位依次置位
    temp |= 0x0002;                                   //显示右边界保留
  }
  
  a[1] |= temp;                                       //显示左边界保留
  a[2] |= temp;
  
 /* if(Sys.Voltage < 100 || Sys.Voltage > 700)                  //电压超出范围  1v < x < 7v 显示故障
  {
    a[1] = 0x7122;                                    //故障显示数据
    a[2] = 0x50C2;
  } */
  
  Coordinate(H,Ver,&xadd,&yadd);//获取绘图起始坐标
  
  for(i=0;i<6;i++)                                  //连续写入8行
  {
    ST7920WriteCmd(yadd);    			// 写Y坐标
    ST7920WriteCmd(xadd);       			// 写X坐标  
    
    if((H % 2) == 1 )                     // 水平坐标为奇数写入空白
    {         
      ST7920WriteData(0x00);
    }   
     yadd++;
        
    if(yadd > 0X9F)
    {
      xadd -= 8;
      yadd = 0X80;
    }        
    else if((yadd > 0X8F) && (xadd <= 0X8F))
    {
      xadd += 16;
    }                
    //写水平坐标
    if(i==0 || i==5)                                //写第0行与第7行时，写入相同数据（上下边界）
    {
      ST7920WriteData(a[0] >> 8);                   //显示数据高8位
      ST7920WriteData(a[0] & 0x00ff);               //显示数据低8位
      continue;                                     //结束本次循环
    }
    if(i < 3)                                       //显示上半部分（上下部分对称）
    {
      ST7920WriteData(a[i] >> 8);
      ST7920WriteData(a[i] & 0x00ff);
    }
    else                                            //显示下半部分
    {
      ST7920WriteData(a[5-i] >> 8);
      ST7920WriteData(a[5-i] & 0x00ff);
    }
  }
}
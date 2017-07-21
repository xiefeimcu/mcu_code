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
  
}

void LCDON(void)
{
  Ini_Lcd();                    //显示设备初始化  
  DisplayMon();      //显示监测界面
  Display.TiOFF = OFFLCDDELAY;  //初始化关闭计时
}

void DisplayRunInfo(unsigned char *infostr)
{
  if(Display.Status.Flag.Mon)
  {
    Draw(0,48,8,16,0,0);
    DisplayString(0,48,infostr,0);
  }
}

/**************************************************************
函数名称：void DisplayNum(unsigned char H,unsigned char Ver,long int value,
                unsigned char type,unsigned char cmd,unsigned char *s,unsigned char fb)
函数介绍：显示数字
输入参数：unsigned char H---->水平坐标
          unsigned char Ver--->垂直坐标
          long int value------>显示的数据
          unsigned char type-->数据类型
          unsigned char cmd--->数据转换控制
          unsigned char *s---->连接在显示数据尾部的字符串 最大长度为4
          unsigned int fb---->反显控制
输出参数：无
返回值  ：无           
***************************************************************/
void DisplayNum(unsigned char H,unsigned char Ver,long int value,
                unsigned char type,unsigned char cmd,unsigned char *s,unsigned int fb)
{
  unsigned char strtmep[16]={0};
  unsigned char i=0,j=0;
   
  i = SInt32_TO_String(strtmep,value,type,cmd);//显示数字转换为字符串
  for(j=0;j<10;j++)//显示数字的尾部加入需要显示的字符串
  {
    if(s[j]=='\0')
      break;
    strtmep[i++] = s[j];
  }
    
  DisplayString(H,Ver,strtmep,fb);
}

/**************************************************************
函数名称：void DisplayNum8X6(unsigned char H,unsigned char Ver,long int value,
                unsigned char type,unsigned char cmd,unsigned char *s)
函数介绍：显示数字8X6
输入参数：unsigned char H---->水平坐标
          unsigned char Ver--->垂直坐标
          long int value------>显示的数据
          unsigned char type-->数据类型
          unsigned char cmd--->数据转换控制
          unsigned char *s---->连接在显示数据尾部的字符串 最大长度为4
输出参数：无
返回值  ：无           
***************************************************************/
void DisplayNum8X6(unsigned char H,unsigned char Ver,long int value,
                unsigned char type,unsigned char cmd,unsigned char *s)
{
  unsigned char strtmep[16]={0};
  unsigned char i=0,j=0;
   
  i = SInt32_TO_String(strtmep,value,type,cmd);//显示数字转换为字符串
  for(j=0;j<4;j++)
  {
    if(s[j]=='\0')
      break;
    strtmep[i++] = s[j];
  } 
  Display_8x6(H,Ver,strtmep);
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
  if(Display.Mon > 4)
    Display.Mon = 1;
  if(Display.Mon < 1)
    Display.Mon = 4;
  
  Draw(0,0,16,48,0,0);                        //清除监测信息显示区

  if(1 == Display.Mon)
  {
    DisplayString(0,0," 5分钟将",0);
    DisplayString(0,16,"10分钟将",0);
    DisplayString(0,32,"30分钟将",0);
    DisplayRainfall();
  }
  else if(2 == Display.Mon)
  {
    DisplayString(0,0," 当前将",0);
    DisplayString(0,16," 累计将",0);
    //DisplayString(0,32,YaoSuInfo[RS485.Name].NameStr,0);
    DisplayRainfall();
    /*if(0==RS485.Name)
    {
      DisplayString(8,32,"-------",0);
    }
    else
    {
      DisplayNum(15-(YaoSuInfo[RS485.Name].DataType>>4),32,RS485.Value,YaoSuInfo[RS485.Name].DataType,space,"",0);
    }*/
  }
  else if(3 == Display.Mon)
  {
    DisplayString(0,0,YaoSuInfo[Analog1.Name].NameStr,0);
    DisplayString(0,16,YaoSuInfo[Analog2.Name].NameStr,0);
    //DisplayString(0,32,YaoSuInfo[Analog3.Name].NameStr,0);
    DisplayString(0,32,YaoSuInfo[RS485.Name].NameStr,0);
    DisplayAnalogData();
  }
  else if(4 == Display.Mon)
  {
    //DisplayString(0,0,YaoSuInfo[Analog4.Name].NameStr,0);
    DisplayString(0,16,"电源电压",0);
    DisplayString(0,32,"设备温度",0);
    DisplayMonData();                   //显示监测数据
  }
  DisplaySysTime();                      //显示时间
  Display.Status.Flag.Mon = SET;         //监测界面标志置位
  Display.Status.Flag.Menu = RESET;      //菜单界面标志复位
  
}

 

void DisplaySysTime(void)
{
  //unsigned char daystring[11]={"00-00-00"};
  unsigned char timestring[9]={"00:00:00"};
  
  /*daystring[0] = HEXASCII[System.Time.Year/10];
  daystring[1] = HEXASCII[System.Time.Year%10];
  daystring[3] = HEXASCII[System.Time.Month/10];
  daystring[4] = HEXASCII[System.Time.Month%10];
  daystring[6] = HEXASCII[System.Time.Day/10];
  daystring[7] = HEXASCII[System.Time.Day%10];*/
  
  timestring[0] = HEXASCII[System.Time.Hour/10];
  timestring[1] = HEXASCII[System.Time.Hour%10];
  timestring[3] = HEXASCII[System.Time.Minute/10];
  timestring[4] = HEXASCII[System.Time.Minute%10];
  timestring[6] = HEXASCII[System.Time.Second/10];
  timestring[7] = HEXASCII[System.Time.Second%10];
  //Display_8x6(10,48,daystring);
  Display_8x6(10,56,timestring);
}
/*显示降雨量*/
void DisplayRainfall(void)
{
  if(1==Display.Mon)
  {
    DisplayNum(9,0,Rainfall.PN05,0X51,space,"",0);
    DisplayNum(9,16,Rainfall.PN10,0X51,space,"",0);
    DisplayNum(9,32,Rainfall.PN30,0X51,space,"",0);
  }
  else if(2==Display.Mon)
  {
    DisplayNum(9,0,Rainfall.PJ,0X51,space,"",0);
    DisplayNum(9,16,Rainfall.PT,0X51,space,"",0);
  }
}

void DisplayAnalogData(void)
{
  if(0==Analog1.Name)
  {
    DisplayString(8,0,"-------",0);
  }
  else
  {
    DisplayNum(15-(YaoSuInfo[Analog1.Name].DataType>>4),0,Analog1.Value,YaoSuInfo[Analog1.Name].DataType,space,"",0);
  }
  if(0==Analog2.Name)
  {
    DisplayString(8,16,"-------",0);
  }
  else
  {
    DisplayNum(15-(YaoSuInfo[Analog2.Name].DataType>>4),16,Analog2.Value,YaoSuInfo[Analog2.Name].DataType,space,"",0);
  }
  /*if(0==Analog3.Name)
  {
    DisplayString(8,32,"-------",0);
  }
  else
  {
    DisplayNum(15-(YaoSuInfo[Analog3.Name].DataType>>4),32,Analog3.Value,YaoSuInfo[Analog3.Name].DataType,space,"",0);
  }*/
  if(0==RS485.Name)
  {
    DisplayString(8,32,"-------",0);
  }
  else
  {
    DisplayNum(15-(YaoSuInfo[RS485.Name].DataType>>4),32,RS485.Value,YaoSuInfo[RS485.Name].DataType,space,"",0);
  }
  
}
void DisplayMonData(void)
{
  /*if(0==Analog4.Name)
  {
    DisplayString(8,0,"-------",0);
  }
  else
  {
    DisplayNum(15-(YaoSuInfo[Analog4.Name].DataType>>4),0,Analog4.Value,YaoSuInfo[Analog4.Name].DataType,space,"",0);
  }*/
  DisplayNum(9,16,RTU.Voltage,0X42,space,"",0);    //显示电源电压 
  DisplayNum(9,32,RTU.Temp,0X31,space,"",0);       //显示设备温度
}


/***************************************************************
                    电量显示
X的范围0-10；将总电量分为10等分显示。0显示空电量，10显示满格，
x>10显示故障。
****************************************************************/
/*void DisBattery(unsigned char H,unsigned char Ver, unsigned char power)
{
  unsigned int a[3]={0x1ffe,0x7002,0x5002};    //空电量显示数据
  unsigned int temp;
  unsigned char i;
  unsigned char xadd,yadd;
  
  temp=0x0002;
      
  for(i=0;i<power;i++)                                //电量显示数据处理
  { 
    temp <<= 1;                                       //电量每增加一级，显示数据从低位到高位依次置位
    temp |= 0x0002;                                   //显示右边界保留
  }
  
  a[1] |= temp;                                       //显示左边界保留
  a[2] |= temp;
  
  if(Voltage < 100 || Voltage > 1500)                  //电压超出范围  1v < x < 15v 显示故障
  {
    a[1] = 0x7122;                                    //故障显示数据
    a[2] = 0x50C2;
  } 
  
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
}*/


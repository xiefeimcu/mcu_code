/*********************************************************
*文件名称：ST7920.c
*摘    要：ST7920绘图模式驱动(并口)
*
*作    者：赵阳
*开始日期：2012年11月26日
*          2014-03-24 从奇数水平地址开始写时，读出原显示GDRAM的
内容，再重新写入，原来的版本是写入0X00。
          2014年4月23日 将显示数字等函数从其他文件中加入
**********************************************************/
#include "ST7920.h"
#include "Public.h"
#include "include.h"
/*******************************************
函数名称：ST7920WriteCmd
功    能：向液晶中写控制命令
参    数：cmd--控制命令
返回值  ：无
********************************************/
void ST7920WriteCmd(char cmd)
{
  char lcdtemp = 0;
  unsigned int error=0XFFFF;

  LCD_RS_L;
  LCD_RW_H;
  LCD_DataIn;  
  do                       //判忙
  {    
    LCD_EN_H;
    _NOP();					
    lcdtemp = LCD2MCU_Data; 
    LCD_EN_L;
    error--;  
  }
  while((lcdtemp & 0x80) && error); 
    
    LCD_DataOut;    
    LCD_RW_L;  		
    MCU2LCD_Data = cmd; 
    LCD_EN_H;
    _NOP();						  			
    LCD_EN_L;  
}

/*******************************************
函数名称：ST7920WriteData
功    能：向液晶中写显示数据
参    数：dat--显示数据
返回值  ：无
********************************************/
void  ST7920WriteData(char dat)
{
    char lcdtemp = 0; 
    unsigned int error=0XFFFF;
       
    LCD_RS_L;
    LCD_RW_H;  
    LCD_DataIn;   
    do                       //判忙
    {    
        LCD_EN_H;
        _NOP();						
        lcdtemp = LCD2MCU_Data; 
        LCD_EN_L;  
        error--;
    }
    while((lcdtemp & 0x80) && error);  
    
    LCD_DataOut; 
    LCD_RS_H;
    LCD_RW_L;  
        
    MCU2LCD_Data = dat;
    LCD_EN_H;
    _NOP();
    LCD_EN_L;
} 

/*******************************************
函数名称：unsigned char ST7920ReadData(void)
功    能：读液晶中显示数据
参    数：无
返回值  ：当前地址显示数据
********************************************/
char ST7920ReadData(void)
{
    char lcdtemp = 0; 
    unsigned int error=0XFFFF;
       
    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;   
    do                       //判忙
    {    
        LCD_EN_H;
        _NOP();				
        lcdtemp = LCD2MCU_Data;
        LCD_EN_L;
        error--;
    }
    while((lcdtemp & 0x80) && error);  
    
    LCD_RS_H;
    LCD_RW_H;  
    LCD_EN_H;	
    lcdtemp = 0;  
    _NOP(); 
    lcdtemp = LCD2MCU_Data;

    LCD_EN_L;
    return lcdtemp;
} 
/*******************************************
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Ini_Lcd(void)
{     
    LCD_CMDOut;                                 //液晶控制端口设置为输出   
    LCD_LIGHT_ON;                               //背光开启         
    Display.Status.Flag.Light = True;           //LCD背光标志
    ST7920delayms(50);
    LCD_POWER_ON;                               //LCD电源开启
    Display.Status.Flag.Power = True;           //LCD电源标志
    //ST7920delayms(500);
    System.Temp = Readtemp(Do1Convert());          //读温度
    ST7920WriteCmd(0x34);                       //扩展指令集
    ClearScreen;                                //清屏 
    DISPLAYON;                                  //显示功能开
    Display.TiOFF = OFFLCDDELAY;
    IniUart0(BPS_9600);
}

/*********************************************************************
                    显示字符串 
H----------- 水平坐标
Ver--------- 垂直坐标
*str  ------ 字符串地址
DecimalPoint  反白控制    
DecimalPoint的某一位为 1 时 该位对应的字符反白显示

DecimalPoint  BIT15 BIT14 BIT13 BIT12 BIT11 BIT10 BIT9 BIT8 BIT7 BIT6 BIT5 BIT4  BIT3 BIT2 BIT1 BIT0 
字符位置        0     1     2     3     4     5     6    7    8    9   11   10    12   13   14   15   

越界处理，超出边界不显示
*********************************************************************/
void DisplayString(char H,char Ver,char *str,unsigned int DecimalPoint)
{
  char i,j,k;
  char xadd,yadd;        //绘图坐标
  char coding;           //ASC2码缓存
  char *strtemp;         //字符编码地址缓存 
  unsigned int GBcoding;          //GB2312码缓存
  char Hleng=0;          //最大写入长度
  char temp=0;
  
  Coordinate(H,Ver,&xadd,&yadd);    //根据列地址与行地址计算绘图 x坐标 y坐标
    
  for(i=0;i<16;i++)
  {          
    strtemp = str;
    Hleng = 16 - H;
    ST7920WriteCmd(yadd);    	                // 写Y坐标
    ST7920WriteCmd(xadd);       		// 写X坐标
    
    if((H % 2) == 1 )                           // 水平坐标为奇数读出原GDRAM高字节显示内容
    {         
      ST7920ReadData();                         //2014-03-24 修改
      temp = ST7920ReadData();
      ST7920WriteCmd(yadd);    	                // 写Y坐标
      ST7920WriteCmd(xadd);       		// 写X坐标
      ST7920WriteData(temp);
    }    
    yadd++;                                     //垂直绘图坐标++
    if(yadd > 0X9F)                             //上下屏切换
    {    
      xadd -= 8;                                
      yadd = 0X80;
    }        
    else if((yadd > 0X8F) && (xadd <= 0X8F))    //改变水平绘图坐标
    {
      xadd += 16;
    }
    
    for(j=0;j<Hleng;)
    {		 
      if('\0' == *strtemp)
      {       
        break;                //字符串结束退出        
      }
      	
      coding = *strtemp;      //取字符编码
      
      if(coding < 0X80)       //字符不是GB2312字符
      {
        for(k=0;k<ASC2StringCodeLeng;k++)
        {
          if(ASC2Code[k].ASC2 == coding)   //查找字符编码对应的字模结构体数组
            break;
        }
      
        if(k >= ASC2StringCodeLeng)        //字符溢出处理
        {
          k = 0;
        }
    
        if((DecimalPoint << j) & 0X8000)       //反白控制 当显示第 j 个字符时反白(0 =< j < 16)
        {
          ST7920WriteData(~ASC2Code[k].ASC2Code[i]);//反白
        }
        else
        {
          ST7920WriteData(ASC2Code[k].ASC2Code[i]);//不反白
        }                           
      }
      else                                   //显示汉字
      {
         GBcoding = coding;                  //GB2312码高字节
         GBcoding <<= 8;
         strtemp++;                          //指向GB2312码低字节
         coding = *strtemp;                  //GB2312码低字节
         GBcoding += coding;                 //16位GB2312码
         
         for(k=0;k<GB2312StringCodeLeng;k++)
         {
          if(GB2312Code[k].GB2312 == GBcoding)   //GB2312码对应的字模结构体数组
            break;
         }
      
         if(k >= GB2312StringCodeLeng)           //字符溢出处理
         {
           k = 0;
         }
         
         //写入字符左边字模数据
         if((DecimalPoint << j) & 0X8000)                 //反白控制
         {
           ST7920WriteData(~GB2312Code[k].GB2312LeftCode[i]);  //反白
         }
         else
         {
           ST7920WriteData(GB2312Code[k].GB2312LeftCode[i]);  //不反白
         }
         j++;                                            //显示长度加1
         if(j >= Hleng)                                  //越界退出
          break;
         
         //写入字符右边字模数据
         if((DecimalPoint << j) & 0X8000)                //反白控制
         {
           ST7920WriteData(~GB2312Code[k].GB2312RightCode[i]);//反白
         }
         else
         {
           ST7920WriteData(GB2312Code[k].GB2312RightCode[i]); //不反白
         }                  
      }
      j++;                                            //显示长度加1
      strtemp++;                                      //指向下一个字符编码
    }    
  }	 
}

/****************************************************************************************
函数名称：Coordinate(char H,char Ver,char *x,char *y)
功    能：获取绘图坐标
参    数：H---水平坐标，Ver---垂直坐标,
          *x---绘图坐标 x 保存地址
          *y---绘图坐标 y 保存地址
返回值  ：无
******************************************************************************************/
void Coordinate(char H,char Ver,char *x,char *y)
{   
  if(H > 15)
    *x = 0;
  else
    *x = H / 2;
  
  if(Ver < 16)  //计算水平绘图坐标
  {
    *x += 0X80;
  }
  else if(Ver < 32)
  {
    *x += 0X90;
  }
  else if(Ver < 48)
  {
    *x += 0X88;
  }
  else 
  {
    *x += 0X98;
  }
   
  if(Ver >31)   //计算垂直绘图坐标
  {
    Ver -=32;
  }
  if(Ver > 127) //坐标超出范围 为0
  {
    Ver = 0;
  }
  
  *y = Ver + 0X80;   
}


/****************************************************************************************
函数名称：void Draw(char H,char Ver,char Hleng,char Verleng,char wdata,const char *dat)
功    能：绘图
          *dat不为Null时写入*dat数据，否则写入wdata
          wdata==1，时*dat数据取反显示
参    数：H---水平坐标，Ver---垂直坐标,
          Hleng---水平长度
          Verleng---垂直长度
          wdata---写入的数据
          *dat---写入的数据地址
返回值  ：无
******************************************************************************************/
void Draw(char H,char Ver,char Hleng,char Verleng,char wdata,const char *dat)
{
  char xadd;
  char yadd;
  char i,j;
  char temp=0;
  
  Coordinate(H,Ver,&xadd,&yadd);//获取绘图起始坐标
  
   for(i=0;i<Verleng;i++)   //写行
      {                              
        ST7920WriteCmd(yadd);    			// 写Y坐标
        ST7920WriteCmd(xadd);       			// 写X坐标  
    
        if((H % 2) == 1 )                       // 水平坐标为奇数读出原GDRAM高字节显示内容
        {                                       //2014-03-24 修改
          ST7920ReadData();
          temp = ST7920ReadData();
          ST7920WriteCmd(yadd);    	                // 写Y坐标
          ST7920WriteCmd(xadd);       		// 写X坐标
          ST7920WriteData(temp);
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
               
        for(j=0;j<Hleng;j++) //写列
        {   
          if(dat != 0)    //写入图片
          {
            if(1 == wdata)//取反显示图片
            {
              ST7920WriteData(~*dat);
            }
            else
            {
              ST7920WriteData(*dat);//正常显示图片
            }
            dat++;      //图片字模数组下标加1     
          }
          else
          {
            ST7920WriteData(wdata);//全部写入相同的数据
          }         
        }
    }       
}

/****************************************************************************************
函数名称：void Display_8x6(char H,char Ver,char *s)
功    能：显示8*6点阵字符
参    数：H---水平坐标，Ver---垂直坐标,
          *s---字符串地址
返回值  ：无
******************************************************************************************/
void Display_8x6(char H,char Ver,char *s)
{
  char i,j,k;
  char xadd,yadd;
  char wdata;
  char temp;
  
  Coordinate(H,Ver,&xadd,&yadd);        //获取绘图起始坐标
  
  for(i=0;i<8;i++)                      //写行
  {
    ST7920WriteCmd(yadd);    			// 写Y坐标
    ST7920WriteCmd(xadd);       		// 写X坐标
    if((H % 2) == 1 )                           // 水平坐标为奇数读出原GDRAM高字节显示内容
    {                                           // 2014-03-24 修改    
      ST7920ReadData();
      temp = ST7920ReadData();
      ST7920WriteCmd(yadd);    	                // 写Y坐标
      ST7920WriteCmd(xadd);       		// 写X坐标
      ST7920WriteData(temp);
      ST7920WriteData(0x00);
    }                 
    yadd++;                                    //垂直坐标加       
    if(yadd > 0X9F)                            //上下半屏切换
    {
      xadd -= 8;
      yadd = 0X80;
    }        
    else if((yadd > 0X8F) && (xadd <= 0X8F)) //换行
    {
      xadd += 16;
    }
               
    for(j=0;j<21;j++) //写列
    {
      if(s[j] == '\0')          //字符串结束退出
        break;
      
      k = TableNumber_8X6(s[j]);
      
      switch(j % 4)
      {
        case 0:
          wdata = ASC2Code_8X6[k].ASC2Code[i];//第一个字符字模
          if(s[j+1] != '\0')
          {
            k = TableNumber_8X6(s[j+1]);      //下一个字符在字库中的位置
            wdata |= (ASC2Code_8X6[k].ASC2Code[i] >> 6);//计算写入的8bit数据
          }
          ST7920WriteData(wdata);                //写入8bit数据    
          break;
          
        case 1:
          wdata = ASC2Code_8X6[k].ASC2Code[i] << 2;//第二个字符字模
          if(s[j+1] != '\0')
          {
            k = TableNumber_8X6(s[j+1]);      //下一个字符在字库中的位置
            wdata |= (ASC2Code_8X6[k].ASC2Code[i] >> 4);//计算写入的8bit数据 
          }
          ST7920WriteData(wdata);                //写入8bit数据    
          break;
          
        case 2:
          wdata = ASC2Code_8X6[k].ASC2Code[i] << 4;//第三个字符字模
          if(s[j+1] != '\0')
          {
            k = TableNumber_8X6(s[j+1]);      //下一个字符在字库中的位置
            wdata |= (ASC2Code_8X6[k].ASC2Code[i] >> 2);//计算写入的8bit数据
          } 
          ST7920WriteData(wdata);                //写入8bit数据    
          break; 
        case 3:
          break;
        default: break;                          
      }
    }
  }  
}

/****************************************************************************************
函数名称：unsigned char TableNumber_8X6(char asc2)
功    能：查找字符在字库中的位置
参    数：unsigned char asc2 --- 字符ASC2码
返回值  ：字符在字库中的编号
******************************************************************************************/
char TableNumber_8X6(char asc2)
{
  char number;
  
  for(number=0;number<ASC2StringCodeLeng_8X6;number++)
  {
      if(ASC2Code_8X6[number].ASC2 == asc2)   //GB2312码对应的字模结构体数组
          break;
  }
  if(number >= ASC2StringCodeLeng_8X6)
  {
      number = 0;
  }
  return number;
}


/**************************************************************
函数名称：void DisplayNum(char H,char Ver,long int value,
                char type,char cmd,char *s,char fb)
函数介绍：显示数字
输入参数：unsigned char H---->水平坐标
          char Ver--->垂直坐标
          long int value------>显示的数据
          char type-->数据类型
          char cmd--->数据转换控制
          char *s---->连接在显示数据尾部的字符串 最大长度为10
          unsigned int fb---->反显控制
输出参数：无
返回值  ：无           
***************************************************************/
void DisplayNum(char H,char Ver,long int value,char type,char cmd,char *s,unsigned int fb)
{
  char strtmep[16]={0};
  char i=0,j=0;
   
  i = SInt32_TO_String(strtmep,value,type,cmd);//显示数字转换为字符串
  for(j=0;j<10;j++)//显示数字的尾部加入需要显示的字符串
  {
    if(s[j]=='\0')
      break;
    if(i < 16)
    {
      strtmep[i++] = s[j];
    }   
  }
    
  DisplayString(H,Ver,strtmep,fb);
}

/**************************************************************
函数名称：void DisplayNum8X6(char H,char Ver,long int value,
                char type,char cmd,char *s)
函数介绍：显示数字8X6
输入参数：unsigned char H---->水平坐标
          char Ver--->垂直坐标
          long int value------>显示的数据
          char type-->数据类型
          char cmd--->数据转换控制
          char *s---->连接在显示数据尾部的字符串 最大长度为10
输出参数：无
返回值  ：无           
***************************************************************/
void DisplayNum8X6(char H,char Ver,long int value,char type,char cmd,char *s)
{
  char strtmep[16]={0};
  char i=0,j=0;
   
  i = SInt32_TO_String(strtmep,value,type,cmd);//显示数字转换为字符串
  for(j=0;j<10;j++)
  {
    if(s[j]=='\0')
      break;
    
    if(i < 16)
    {
      strtmep[i++] = s[j];
    }   
  } 
  Display_8x6(H,Ver,strtmep);
}


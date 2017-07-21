/*********************************************************
*文件名称：ST7920.h
*摘    要：ST7920驱动接口函数声明
*
*作    者：赵阳
*开始日期：2012年11月26日
*完成日期：2012年11月27日
**********************************************************/

#ifndef __ST7920_H
#define __ST7920_H

#include "ST7920code.h"
#include "system.h"

/******************* I/O定义 *********************************/
#define LCD_DataIn              P5DIR = 0X00          //数据口方向设置为输入
#define LCD_DataOut             P5DIR = 0XFF          //数据口方向设置为输出
#define LCD2MCU_Data            P5IN
#define MCU2LCD_Data            P5OUT
#define LCD_CMDOut              P4DIR |= BIT7|BIT6|BIT5|BIT4+BIT3;
#define LCD_CMIn                P4DIR &= ~(BIT7|BIT6|BIT5|BIT4|BIT3);
#define LCD_RS_H                P4OUT |= BIT5      
#define LCD_RS_L                P4OUT &= ~BIT5    
#define LCD_RW_H                P4OUT |= BIT6      
#define LCD_RW_L                P4OUT &= ~BIT6    
#define LCD_EN_H                P4OUT |= BIT7     
#define LCD_EN_L                P4OUT &= ~BIT7     
#define LCD_POWER_ON            P4DIR |= BIT3;P4OUT &= ~BIT3                  //LCD电源开
#define LCD_POWER_OFF           P4DIR |= BIT3;P4OUT |= BIT3                   //LCD电源关
#define LCD_LIGHT_ON            P4DIR |= BIT4;P4OUT &= ~BIT4                  //LCD背光开
#define LCD_LIGHT_OFF           P4DIR |= BIT4;P4OUT |= BIT4                   //LCD背光关
#define DISPLAYON               ST7920WriteCmd(0x36)            //显示功能开
#define DISPLAYOFF              ST7920WriteCmd(0x34)            //显示功能关

#define ST7920delayms(x)        delayms(x)                      //延时毫秒
#define ST7920delayus(x)        delayus(x)                      //延时毫秒


/*********************************** 功能函数 *********************************/
#define H_LINE(x,y,leng)        Draw(x,y,leng,1,0XFF,NULL)      //画横线
#define ClearScreen             Draw(0,0,16,64,0,0)             //清屏 
#define ClearLine_0             Draw(0,0,16,16,0,0)             //清除第0行显示 
#define ClearLine_1             Draw(0,16,16,16,0,0)            //清除第1行显示 
#define ClearLine_2             Draw(0,32,16,16,0,0)            //清除第2行显示 
#define ClearLine_3             Draw(0,48,16,16,0,0)            //清除第3行显示 


/***************************** 函数声明 ***************************************/
void ST7920WriteCmd(char cmd);
void ST7920WriteData(char dat);
void Coordinate(char H,char Ver,char *x,char *y);
void DisplayString(char H,char Ver,char *str,unsigned int DecimalPoint);
void Ini_Lcd(void);
void Draw(char H,char Ver,char Hleng,char Verleng,char wdata,const char *dat);
char TableNumber_8X6(char asc2);
void Display_8x6(char H,char Ver,char *s);
char  ST7920ReadData(void);
void DisplayNum(char H,char Ver,long int value,
                char type,char cmd,char *s,unsigned int fb);
void DisplayNum8X6(char H,char Ver,long int value,
                char type,char cmd,char *s);

void ST7920Test(void);

#endif /*#ifndef __ST7920_H*/
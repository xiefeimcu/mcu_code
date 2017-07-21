/*********************************************************
*文件名称：SC8035.c
*摘    要：语音芯片驱动
*          
*作    者：赵阳
**********************************************************/
#include "include.h"
#include "SC8035.h"


void SC_WBusy(unsigned int t) 						 //语音播放读忙
{     
  unsigned int times;     
  for(times=0;times<t;times++)            //等待超过7秒最长语音段自动退出      
  {  
    delayms(10);
       
   }
}

void SC_Reset(void)							 //上电复位防止干扰发声芯片放音
{
  SC_RST_OUT;
  SC_DATA_OUT;
  _NOP();
  SC_DATA_0;     							 //数据脉冲低
  SC_RST_1;     							 //复位脉冲高
  delayus(100);                    		                         //延时100US
  SC_RST_0;     							 //复位脉冲低
  delayus(100); 							 //延时100US 
  SC_RST_IN;
  SC_DATA_IN;
}

void SC_Speech(char cnt) 		//播放语音   第 cot(cot>0) 段
{      
  char i;	
  
  SC_Reset();  
  
  SC_RST_OUT;
  SC_DATA_OUT;
  										 //语音播放复位
  SC_RST_1;      						 //复位脉冲高
  delayus(100);						 //延时100US										     //退出报警判断
  SC_RST_0;      						 //复位脉冲低
  delayus(100);						 //延时100US	         

  for(i=0;i<cnt;i++)      
  {          
    SC_DATA_1;              		 //数据脉冲高          
    delayus(100);           		 //延时100US   			
    SC_DATA_0;              		 //数据脉冲低          
    delayus(100);           		 //延时100US 		       
  } 
} 
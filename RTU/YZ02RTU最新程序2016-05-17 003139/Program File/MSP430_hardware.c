//---------------------------------------------------------
//文件名称：MSP430_hardware.c
//摘    要：MSP430硬件
//
//作    者：赵阳
//版    本：1.0
//修改日期：2013年11月22日
//---------------------------------------------------------

#include "MSP430_hardware.h"
#include "include.h"



void InitSysCKL(void)                                        //系统时钟初始化
{  
  /***********************晶体振荡器,系统时钟初始化**************************/
  unsigned int i;
  BCSCTL1 &= ~XT2OFF;                                        //打开XT2振荡器
  do
  {  
    IFG1 &= ~OFIFG;                                          //清除振荡器失效标志
    for (i=0xFF;i>0;i--);                                    //延时，等待XT2起振
  }
  while ((IFG1 & OFIFG) != 0);                               //判断XT2是否起振
  BCSCTL2 = SELM_2+SELS;                                     //选择MCLK、SMCLK为XT2
                                      
}

/*非正常复位处理*/
void Except(void)
{
  if(IFG1&WDTIFG==WDTIFG)       //看门狗中断或安全键值错误
  {
    IFG1 &= ~WDTIFG;            //复位看门狗中断标志    
  }
  else if(FCTL3&KEYV==KEYV)     //FLASH安全键值错误
  {
    FCTL3 &= ~KEYV;             //复位FLASH安全键值   
  }
}

//按键输入初始化
void KeyInit(void)
{
  P2IES |=  BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;            //选择下降沿中断
  P2IE  |=  BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;            //打开中断使能
  P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);          //设置按键接口为输入
 
}

/**************************************************************
函数名称：void IniTimerA(void) 
函数介绍：初始化定时器A
输入参数：无
输出参数：定时器A相关寄存器
返回值  ：无
*************************************************************/
void IniTimerA(void) 
{  
  /*TACTL = TACLR;                        //停止
  _NOP();*/
  TACTL = TASSEL_1+MC_0+TACLR;     //ACLK 32768HZ,增减计数模式，清除TAR，允许定时器中断。
  TACCTL0 = CCIE;                       //CCR0中断使能
  TACCR0 = 16384;                       //计数值 0.5s
  TACTL |= TAIE+MC_3;
}

void TimerBInit()
{
  TBCTL=TBSSEL_1+TBCLR+MC_0;		//选择时钟源为ACLK
  TBCCTL0 = CCIE;    			//允许定时器中断
}

/*****************************************************************************
设置定时时间
ti：要定时的时间，与时钟源的频率有关
*****************************************************************************/
void SetTimerB(unsigned int ti)
{
  TBCCR0=ti;				//定时时间
}

/*****************************************************************************
打开或关闭定时器B 增计数模式
doit: 0：停止   1：运行
*****************************************************************************/
void GotimeDfB(unsigned char doit)
{
  if(doit==1)
  {
    TBCTL |= MC_1+TBCLR;		//打开定时器
  }
  else if(doit==0)
  {
    TBCTL &= ~MC0;					//关闭定时器
  }
}

#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B0(void)   //定时器中断函数
{
  if(System.Event.Bit.UART0BYTE)
  {
    System.Event.Bit.UART0BYTE = RESET;
    System.Event.Bit.UART0 = SET;
  }
  if(System.Event.Bit.UART1BYTE)
  {
    System.Event.Bit.UART1BYTE = RESET;
    System.Event.Bit.UART1 = SET;
  }
  TBCTL &= ~MC0;        //关闭定时器
  LPM3_EXIT;
}

/*TimerA CCR0 中断服务程序*/
#pragma vector = TIMERA0_VECTOR
__interrupt void Time_A_CCR0(void)
{
  RESETWDT;
  TiTimeHandlers();
  if(Rainfall.InDelay > 0)
  {
    Rainfall.InDelay--;
  }
}

/*TimerA TAR 定时溢出中断服务程序*/
#pragma vector = TIMERA1_VECTOR
__interrupt void Time_A_TAIFG(void)
{      
  RESETWDT;
  
  SysTimePlue();        //日期时间计算
  if(Rainfall.InDelay > 0)
  {
    Rainfall.InDelay--;
  }
  LPM3_EXIT;
  TACTL &= ~TAIFG;
}

/*******************************************************************************
函数名称：__interrupt void  PORT1_ISR(void) 
函数介绍：P1端口的中断服务函数 降水量输入
输入参数：无                              
输出参数：系统标志寄存器
返回值  ：无
*******************************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void  PORT1_ISR(void)
{
  unsigned char counts=0;

  RESETWDT;                                     //复位看门狗  
  if((0==Rainfall.InDelay)&&Rainfall.Type)      //允许新的中断输入且雨量计接入
  {
    if(P1IFG&(RAINPORT_1+RAINPORT_2))           //雨量通道1或雨量通道2有其中一路发生中断处理
    {
      counts = 0;
      while(RAININ^(RAINPORT_1+RAINPORT_2))     //输入有效性检测 需保持RAINTIME ms的低电平输入
      {    
        if(counts++ >= RAINTIME)                //是否保持RAINTIME ms的低电平
          break;
        delayms(1);
        RESETWDT;                               //复位看门狗      
      }
      if(counts >= RAINTIME)                    //雨量输入有效
      {        
        System.Event.Bit.Rain = SET;              //雨量输入标志置位
        Rainfall.InDelay = RAINJG;              //雨量输入间隔控制
        LPM3_EXIT;
      }
    }   
  } 
  P1IFG = 0x00;                                 //清除中断标志
  return;
}


/*******************************************************************************
函数名称：__interrupt void  PORT2_ISR(void)
函数介绍：P2端口的中断服务函数 按键输入
输入参数：无                              
输出参数：键值缓存
返回值  ：无
*******************************************************************************/
#pragma vector=PORT2_VECTOR
__interrupt void  PORT2_ISR(void)
{  
  unsigned char counts=0;
        
  RESETWDT;                             //复位看门狗
  if(P2IFG & 0x3F)                      //按键输入
  {
    counts = 0;
    while(KEYIN ^ 0X3F)               //检测按键是否为有效输入
    {
      if(counts++ >= KEYINTIME) 
        break;
      delayms(1);
      RESETWDT;
    }
    if(counts >= KEYINTIME)           //按键输入有效
    {
      System.Key = KEYIN;            //读取按键编码
      System.Event.Bit.Key = SET;      //按键输入事件置位
      Display.TiOFF = OFFLCDDELAY;    //初始化关闭显示计时
      LCD_LIGHT_ON;      //LCD背光关
      Display.Status.Flag.Light = SET;
    }
  }
  LPM3_EXIT;
  P2IFG = 0x00;                       //清除中断标志
  return;
}


#include "include.h"

main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P3DIR &=~BIT1+~BIT3;
  P3OUT |=BIT0;
  SC_RST_OUT; 
  SC_DATA_OUT;
  _NOP();
  SC_DATA_0;     							 //数据脉冲低
  SC_RST_1;     							 //复位脉冲高
  delayus(20);                    		                         //延时100US
  SC_RST_0;     							 //复位脉冲低
  delayus(20); 							         //延时100US 
  SC_RST_IN;
  SC_DATA_IN;
  
  SysInitHandlers();  
  while(1)
  {
    RESETWDT;                   //复位看门狗
    SysEventHandlers();         //系统事件处理
    UserEventHandlers();        //任务处理 

    if(Display.Status.Flag.SetMode)
    {
      SetMode();
    }

    if(0==System.Event.Register)//无事件进入LPM3
    {
      RESETWDT;                 //复位看门狗
      _NOP();
      LPM3;
      _NOP();
    }
  }
}


#include "include.h"


main()
{
  // Stop watchdog timer to prevent time out reset
  
  WDTCTL = WDTPW + WDTHOLD;
  
  SysInitHandlers();            //系统初始化

  while(1)
  {
    RESETWDT;                   //复位看门狗
 
    SysEventHandlers();         //系统事件处理 
    
    UserEventHandlers();        //任务处理 
    
    if(Display.Status.Flag.SetMode)
    {
      SetMode(); 
    }
    
    if(System.Event.Bit.Flash)
    {
      System.Event.Bit.Flash = RESET;
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
 

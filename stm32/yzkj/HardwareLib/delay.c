#include "delay.h"

void delay_us(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}

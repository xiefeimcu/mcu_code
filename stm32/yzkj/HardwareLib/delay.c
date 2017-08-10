#include "delay.h"

void delay_us(uint16_t count){
// 主频2MHz,一条指令周期约0.5us 
 while(count--);
 {
  __ASM("NOP");
  __ASM("NOP");
  __ASM("NOP");
  __ASM("NOP");


 }
}

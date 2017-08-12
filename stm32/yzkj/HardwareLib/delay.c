#include "delay.h"
void delay_us(uint16_t num){
	
	while(num--){
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}

}
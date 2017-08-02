
#include "key.h"

key_event_t key_scan_test(){
	static uint8_t trg=0;
	static uint8_t state=0;
	
	/*记录被按下的bit*/
	uint8_t pin_data=KEY_DATA ^ 0xFF;
	
	/*触发机制,按键发生变化时相应bit为1*/
	trg= pin_data & (pin_data ^ state);
	/*记录本次按键数据*/
	state=pin_data;
	/*按键发生改变并且当前为按下时,才能认定为按下*/
	switch(state & trg){
		case 0x01: return KEY_UP;
		
		case 0x02: return KEY_DOWN;
		
		case 0x04: return KEY_LEFT;
		
		case 0x08: return KEY_RETURN;
		
		case 0x10: return KEY_SET;
		
		case 0x20: return KEY_RIGHT;	
		
		default :return   KEY_NULL;
	}
}























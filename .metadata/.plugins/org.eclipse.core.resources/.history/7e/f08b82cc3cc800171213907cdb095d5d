
#include "key.h"

static uint8_t key_last =  0;
static uint8_t key_trg;

key_event_t key_scan_test(void){
	 uint8_t pin_data=(uint8_t)KEY_DATA;
	
	 pin_data=((pin_data >> 1) |0xC0) ^ 0xFF;
	
	key_trg= pin_data & (pin_data ^ key_last);
	key_last=pin_data;
	
	switch(key_trg){
		case 0x01:
		return KEY_DOWN;
		
		case 0x02:  
		return KEY_UP;
		
		case 0x04: 
		return KEY_LEFT;
		
		case 0x08: 
		return KEY_RETURN;
		
		case 0x10: 
		return KEY_SET;
		
		case 0x20:
		return KEY_RIGHT;
		
		default  :
		return  KEY_NULL;
	}
	
	return  KEY_NULL;
	
}
























#include "key.h"

static uint8_t last_valid_key = 0;
static uint8_t now_valid_key =  0;
static uint8_t last_press_key = 0;

/*
*LastReadKey代表上次KeyScan()采样读取的键值
*CurrReadKey; //记录本次KeyScan()采样读取的键值
*LastKey 代表上次的有效判定的键值（经过逻辑判断，或者经过校正的实际键值）
*CurrKey代表本次的有效判定键值。
*按照逻辑式
*CurrKey=(CurrReadKey&LastReadKey)|LastKey&(CurrReadKey^LastReadKey)  (1)
*/

key_event_t key_scan_test(){
	/*算法1*/
	//uint8_t pin_data=KEY_DATA ^ 0xFF;
	//trg= pin_data & (pin_data ^ now_press_key);
	//now_press_key=pin_data;
	
	/*算法2*/
	uint8_t now_press_key=KEY_DATA ^ 0xFF;
	now_valid_key=(now_press_key & last_press_key) | (last_valid_key & (now_press_key ^ last_press_key));
	
	last_press_key=now_press_key;
	last_valid_key=now_valid_key;
	
	/*按键发生改变并且当前为按下时,才能认定为按下*/
	switch(now_valid_key){
		case 0x01:
		now_valid_key|=0x01;
		return KEY_UP;
		
		case 0x02:  
		now_valid_key|=0x02;
		return KEY_DOWN;
		
		case 0x04: 
		now_valid_key|=0x04;
		return KEY_LEFT;
		
		case 0x08: 
		now_valid_key|=0x08;
		return KEY_RETURN;
		
		case 0x10: 
		now_valid_key|=0x10;
		return KEY_SET;
		
		case 0x20:
		now_valid_key|=0x20;	
		return KEY_RIGHT;
		
		default  :
		now_valid_key&=0x00;
		return  KEY_NULL;
	}
	
}























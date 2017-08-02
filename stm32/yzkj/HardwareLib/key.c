
#include "key.h"

#define KEY_MAX_COUNT        200
#define KEY_SCAN_INTERVAL_MS 10
#define KEY_VALVE_COUNT      4

key_event_t key_event_handle=KEY_NULL;

key_event_t scan_key(void){
	
static	uint8_t key_count[6]={0,0,0,0,0,0};
static  uint8_t last_push=0;
static  uint8_t loss_flag=1;

	if(loss_flag){
		if(READ_KEY_UP() == RESET){
			if(key_count[0] < KEY_MAX_COUNT){
				key_count[0]++;
				
				if(key_count[0]>=KEY_VALVE_COUNT){
					key_count[0]=0;
					last_push=1;
					return KEY_UP;
				}
			}
		}
		
		else if(READ_KEY_DOWN() == RESET){
			if(key_count[1] < KEY_MAX_COUNT){
				key_count[1]++;
				
				if(key_count[1]>=KEY_VALVE_COUNT){
					key_count[1]=0;
					last_push=1;
					return KEY_DOWN;
				}
			}
		}
		
		else if(READ_KEY_LEFT() == RESET){
			if(key_count[2] < KEY_MAX_COUNT){
				key_count[2]++;
				
				if(key_count[2]>=KEY_VALVE_COUNT){
					key_count[2]=0;
					last_push=1;
					return KEY_LEFT;
				}
			}	
		}
		
		else if(READ_KEY_RIGHT() == RESET){
			if(key_count[3] < KEY_MAX_COUNT){
				key_count[3]++;
				
				if(key_count[3]>=KEY_VALVE_COUNT){
					key_count[3]=0;
					last_push=1;
					return KEY_RIGHT;
				}
			}
		}
		
		else if(READ_KEY_SET() == RESET){
			if(key_count[4] < KEY_MAX_COUNT){
				key_count[4]++;
				
				if(key_count[4]>=KEY_VALVE_COUNT){
					key_count[4]=0;
					last_push=1;
					return KEY_SET;
				}
			}
		}
		
		else if(READ_KEY_RETURN() == RESET){
			if(key_count[5] < KEY_MAX_COUNT){
				key_count[5]++;
				
				if(key_count[5]>=KEY_VALVE_COUNT){
					key_count[5]=0;
					last_push=1;
					return KEY_RETURN;
				}
			}
		}	
		
		else{
			if(!last_push){
				last_push=0;
				loss_flag=1;
			}
		}
	}
	
	memset(key_count,0,sizeof(key_count));
}
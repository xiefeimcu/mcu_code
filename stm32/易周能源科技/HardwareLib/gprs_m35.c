
#include "gprs_m35.h"
#include "usart.h"
#include "string.h"

handle_m35_t hm35;

uint8_t search_string(uint8_t *target,uint8_t *src,uint8_t len){
	uint8_t count=0;
	uint8_t *p1=target,*p2=src;
	uint8_t last=0;
	
	while((*p1) && (*p2)){
		if(*p2 == *p1){
			p1++;
			p2++;
			count++;
			last=1;
		}else{
			if(last){
				count=0;
				break;
			}
			p1++;
		}
	}
	
	if(count>=len)
		return M35_OK;
	else
		return M35_ERR; 
}

state_m35 test_at(void){
	uint8_t rx_buf[6];
	hm35.operat.send_uart("AT+?\r\n",sizeof("AT+?\r\n"));
	hm35.operat.receive_uart(rx_buf,sizeof(rx_buf),50);
	
	if(search_string(rx_buf,"\r\nOK\r\n",6)){
			return M35_OK;
	}
	
	return M35_ERR;
}

state_m35 init_param(void){

}

state_m35 register_net(void){
	hm35.operat.send_uart("AT+CGATT=1\r\n",sizeof("AT+CGATT=1\r\n"));
	hm35.operat.delay(75);
	hm35.operat.send_uart("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n",sizeof("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n"));
	hm35.operat.delay(300);
}

state_m35 built_link(net_param_t net_inf){

}

void disconnect(void){
	hm35.operat.send_uart("AT+CGATT=0\r\n",sizeof("AT+CGATT=1\r\n"));
	hm35.operat.delay(75);
}

uint8_t hal_receive_uart(uint8_t *data, uint8_t len,uint32_t time_out){
	HAL_UART_Receive(&huart1,data,len,time_out);
	return huart1.RxXferSize;
}

void hal_send_uart(uint8_t *data, uint8_t len){
	HAL_UART_Transmit(&huart1,data,len,5);
}

void set_transparent_mode(uint8_t mode){
	if(mode){
		hm35.operat.send_uart("AT+QIMODE=1\r\n",sizeof("AT+QIMODE=0\r\n"));
	}
	else{
			hm35.operat.send_uart("AT+QIMODE=0\r\n",sizeof("AT+QIMODE=0\r\n"));
	}
} 

void init_m35(void){
	memset(&hm35,0,sizeof(hm35));
	
	hm35.operat.test_at_cmd=test_at;
	hm35.operat.built_link=built_link;
	hm35.operat.disconnect=disconnect;
	hm35.operat.init_param=init_param;
	hm35.operat.register_net=register_net;
	
	hm35.operat.receive_uart=hal_receive_uart;
	hm35.operat.send_uart=hal_send_uart;
	hm35.operat.delay=HAL_Delay;
	hm35.operat.set_transparent_mode=set_transparent_mode;
}


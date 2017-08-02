#include "modbus.h"
#include "usart.h"
#include "stm32f1xx_hal.h"

void creat_dev_inf(dev_modbus_handle_t *dev ,dev_inf_t dev_inf){
	if(dev_inf==DEV_NULL)
		return;
	
	switch(dev_inf){
	//根据厂家文档填写相应格式
		default: break;
	}
}

void set_485_dir(port_485_idx_t port, dir_485_t dir){
	if(port==PORT_485_1){
		
		if(dir==DIR_485_TX)
			SET_TX_485_1();
		else
		  SET_RX_485_1();
		
	}
	else if(port==PORT_485_2){

		if(dir==DIR_485_TX)
			SET_TX_485_2();
		else
		  SET_RX_485_2();
	}
}

void send_uart(port_485_idx_t port,uint8_t *data ,uint8_t len){
	set_485_dir(port,DIR_485_TX);
	
	if(port==PORT_485_1){
		HAL_UART_Transmit(&huart2,data,len,WAIT_FOR_485_TX_MS);
	}
	else if(port==PORT_485_2){
		HAL_UART_Transmit(&huart3,data,len,WAIT_FOR_485_TX_MS);
	}
	
	set_485_dir(port,DIR_485_RX);
}

uint8_t receive_uart(port_485_idx_t port,uint8_t *data ,uint8_t len){
	set_485_dir(port,DIR_485_RX);	
	
	if(port==PORT_485_1){
		HAL_UART_Receive(&huart2,data,len,WAIT_FOR_485_RX_MS);
	}
	else if(port==PORT_485_2){
		HAL_UART_Receive(&huart3,data,len,WAIT_FOR_485_RX_MS);
	}
	
	return len;
}





#include "modbus.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "crc.h"

void creat_dev_inf(dev_modbus_handle_t *dev ,dev_inf_t dev_inf){
static	uint16_t crc=0;
	if(dev_inf==DEV_NULL)
		return;
	
	switch(dev_inf){
		//根据不同厂家modbus协议获取配置
		case MODBUS_RTU_TEST:
		dev->member.port_idx=PORT_485_2;
		dev->member.dev_addr=01;
		dev->member.fun_code=02;
		dev->member.start_addr_l=0x00;
		dev->member.start_addr_h=0x0c;
		dev->member.read_count_l=0x00;
		dev->member.read_count_h=0x0b;
	
		crc=calculate_crc(dev->modbus_msg +1 , MODBUS_MSG_LEN - 3);

		dev->member.crc_l=(uint8_t)crc;
	  dev->member.crc_h=(uint8_t)(crc>>8);
		default: break;
	}
}

void set_485_dir(uint8_t port, dir_485_t dir){
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

void send_uart(uint8_t port,uint8_t *data ,uint8_t len){
	set_485_dir(port,DIR_485_TX);
	
	if(port==PORT_485_1){
		HAL_UART_Transmit(&huart2,data,len,WAIT_FOR_485_TX_MS);
	}
	else if(port==PORT_485_2){
		HAL_UART_Transmit(&huart3 ,data,len,WAIT_FOR_485_TX_MS);
	}
	
	set_485_dir(port,DIR_485_RX);
}

uint8_t receive_uart(uint8_t port,uint8_t *data ,uint8_t len){
	set_485_dir(port,DIR_485_RX);	
	
	if(port==PORT_485_1){
		HAL_UART_Receive(&huart2,data,len,WAIT_FOR_485_RX_MS);
	}
	else if(port==PORT_485_2){
		HAL_UART_Receive(&huart2,data,len,WAIT_FOR_485_RX_MS);
	}
	
	return len;
}

void   modbus_read_request(dev_modbus_handle_t *dev){
	send_uart(dev->member.port_idx,(dev->modbus_msg + 1),(sizeof(dev->modbus_msg) - 1));
}

void  modbus_clear_msg(dev_modbus_handle_t *dev){
	memset((uint8_t*)(dev->modbus_msg),0,sizeof(dev->modbus_msg));
}

int8_t modbus_recver_data(dev_modbus_handle_t *dev,uint8_t *rx_buf , uint8_t len){
uint16_t crc=(uint16_t)(rx_buf[len-2]) <<8 | rx_buf[len-1];

		if(crc == calculate_crc(rx_buf,len - 2)){
			return 0;
		}
		else{
			return -1;
		}
		
	return -2;
}


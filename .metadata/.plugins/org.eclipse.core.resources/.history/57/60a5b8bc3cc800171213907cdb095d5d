
#ifndef _MODE_BUS_H
#define _MODE_BUS_H

#include "stm32f1xx_hal.h"

#define WAIT_FOR_485_RX_MS 100
#define WAIT_FOR_485_TX_MS 5

#define MODBUS_MSG_LEN     9

#define SET_TX_485_1() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET)
#define SET_RX_485_1() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_TX_485_2() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_SET)
#define SET_RX_485_2() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_RESET)


typedef enum {
	TYPE_NULL,
	READ_COIL_STATE,
	READ_INPUT_STATE,
	READ_HOLD_REG,
	READ_INPUT_REG,
	WRITE_COIL,
	WRITE_REG,
	READ_UNUSUAL_STATE,
	SEND_DIAGNOSE_VERIFY,
	READ_ENVET_COUNT,
	WRITE_COILS,
	WRITE_REGS,
	REPORT_SLAVE_IDENTIFY,
	RESET_COMM_LINK,
}modbus_fun_type;

typedef enum {
	DEV_NULL,
	MODBUS_RTU_TEST,
}dev_inf_t;


typedef enum {
	PORT_485_1,
	PORT_485_2,
}port_485_idx_t;


typedef union{
	 uint8_t modbus_msg[MODBUS_MSG_LEN];
	 struct {
		uint8_t         port_idx;//PORT_485_1 or  PORT_485_2
		uint8_t         dev_addr; 
		uint8_t         fun_code;
		uint8_t         start_addr_l;
		uint8_t         start_addr_h;
		uint8_t         read_count_l;
		uint8_t         read_count_h;
		uint8_t         crc_h;
		uint8_t         crc_l;
	}member;
}
dev_modbus_handle_t;

typedef enum {
	DIR_485_TX,
	DIR_485_RX,
}dir_485_t;

void   creat_dev_inf(dev_modbus_handle_t *dev ,dev_inf_t dev_inf);
int8_t modbus_recver_data(dev_modbus_handle_t *dev,uint8_t *rx_buf , uint8_t len);
void   modbus_read_request(dev_modbus_handle_t *dev);

#endif 

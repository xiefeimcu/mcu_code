
#ifndef _MODE_BUS_H
#define _MODE_BUS_H

#include "stm32f1xx_hal.h"

#define WAIT_FOR_485_RX_MS 100
#define WAIT_FOR_485_TX_MS 5

#define SET_TX_485_1() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_RX_485_1() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET)
#define SET_TX_485_2() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_RESET)
#define SET_RX_485_2() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_SET)

typedef enum {
	DEV_NULL,
}dev_inf_t;

typedef struct {
	uint8_t  port_idx;//PORT_485_1 or  PORT_485_2
	uint8_t  dev_add; 
	uint8_t  fun_code;
	uint16_t start_add;
	uint16_t data_size;
	uint16_t crc_code;	
}dev_modbus_handle_t;

typedef enum {
	DIR_485_TX,
	DIR_485_RX,
}dir_485_t;

typedef enum {
	PORT_485_1,
	PORT_485_2,
}port_485_idx_t;

typedef struct {
	void     (*send_uart)(port_485_idx_t port,uint8_t *data ,uint8_t len);
	uint8_t (*receive_uart)(port_485_idx_t port,uint8_t *data ,uint8_t len);
}modbus_opt_fun_t;

#endif 

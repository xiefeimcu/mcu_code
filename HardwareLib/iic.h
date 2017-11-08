#ifndef _IIC_H
#define _IIC_H
#include "stm32f1xx_hal.h"

typedef struct {
	GPIO_TypeDef *sda_port;
	uint16_t     sda_pin;
	GPIO_TypeDef *scl_port;
	uint16_t     scl_pin;
	
}iic_port_inf;

extern iic_port_inf iic_io;

void init_iic(GPIO_TypeDef port1 ,uint16_t pin1,GPIO_TypeDef port2 ,uint16_t pin2);

void start_iic(void);
void stop_iic(void);

void send_byte_iic(uint8_t txd);
uint8_t read_byte_iic(uint8_t if_need_ack);

uint8_t wait_ack_iic(void);
void    send_ack_iic(void);
void    send_no_ack_iic(void);

#endif

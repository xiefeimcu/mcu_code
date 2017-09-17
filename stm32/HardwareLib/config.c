/*
 * config.c
 *
 *  Created on: 2017年8月16日
 *      Author: xiefei
 */
#include "usart.h"
#include "config.h"
#include "stm32f1xx_hal.h"
#include "AT24C04.h"

#define READ_NVM(SrcBuf,len,addr) Read_NByte_Randomaddress(SrcBuf,len,addr)
#define WRITE_NVM(addr,SrcBuf,len)

rtuParameter_t rtuParameter;

int8_t load_config(void){
	if (READ_NVM((uint8_t*)(&rtuParameter),sizeof(rtuParameter),0x02))
		return 0;
	else
		return -1;
}

uint8_t upData_config(){
	/*
	 * 执行相关设置
	 */
	change_uart_baudrate(&RS2321_UART_HANDLE,rtuParameter.comBaudRate.baudRateRs2321);
	change_uart_baudrate(&RS2322_UART_HANDLE,rtuParameter.comBaudRate.baudRateRs2322);
	change_uart_baudrate(&RS4851_UART_HANDLE,rtuParameter.comBaudRate.baudRateRs4851);
	change_uart_baudrate(&RS4852_UART_HANDLE,rtuParameter.comBaudRate.baudRateRs4852);

	/*
	 * 写入EEPROM
	 */

/*reStartMcu?*/

}





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

void upData_config(){
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

void load_config_Default(void){
	rtuParameter.comBaudRate.baudRateRs2321=BAUD_9600;
	rtuParameter.comBaudRate.baudRateRs2322=BAUD_9600;
	rtuParameter.comBaudRate.baudRateRs4851=BAUD_9600;
	rtuParameter.comBaudRate.baudRateRs4852=BAUD_9600;
	rtuParameter.sysConfig.lcdCloseTime=55;
	rtuParameter.sysConfig.lcdcloseBkTime=35;
	rtuParameter.sysConfig.mcuStopTime=60;
	rtuParameter.upDataArg.RtuStationAddr[0]=1;
	rtuParameter.upDataArg.RtuStationAddr[1]=1;
	rtuParameter.upDataArg.RtuStationAddr[2]=1;
	rtuParameter.upDataArg.RtuStationAddr[3]=1;
	rtuParameter.upDataArg.RtuStationAddr[4]=1;
	rtuParameter.upDataArg.centreStationAddr=0x01;
	rtuParameter.upDataArg.passWord = 1234;
	rtuParameter.upDataArg.rtuType = RTU_TYPE_SK; //水库
	rtuParameter.upDataArg.timeAverageInterval=5; //5分钟

	//upData_config();
}
/*
 *return 0 success
 *return -1 fail
 */
int8_t load_config(void){
	if (READ_NVM((uint8_t*)(&rtuParameter),sizeof(rtuParameter),0x02))
		return 0;
	else
		return -1;
}





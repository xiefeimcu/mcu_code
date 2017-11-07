/*
 * fileOpt.c
 *
 *  Created on: 2017年10月31日
 *      Author: Administrator
 */

#include "fileOpt.h"
#include "config.h"

int8_t write_one_line(char *str,uint8_t *data ,uint8_t len,uint32_t *Pointer){
	FATFS fs;
	FIL file;
	FRESULT ret;
	uint32_t writeCount;

	ret = f_mount(&fs,"",0);
	if(ret != FR_OK){
		failed_inf("mount err",ret);
		return -1;
	}

	ret = f_open(&file,"rtuConfig.csv", FA_CREATE_NEW | FA_WRITE);



	ret = f_write(&file, data, len, Pointer);
	if (ret != FR_OK) {
		f_close(&file);
		return -2;
	}

	if(ret != FR_OK){
		 f_close(&file);
		failed_inf("open err",ret);
		return -2;
	}

}

int8_t write_config_to_sd(){
//	sysConfig_t      *sysConf = &rtuParameter.sysConfig;
//	comBaudRate_t    *comConf = &rtuParameter.comBaudRate;
//	upDataArg_t      *upConf = &rtuParameter.upDataArg;
//
//	write_one_line("背光时间",sysConf->lcdCloseTime,sizeof(sysConf->lcdCloseTime));

}



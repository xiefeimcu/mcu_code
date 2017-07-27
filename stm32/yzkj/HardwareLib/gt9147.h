#ifndef _GT9147_H

#define _GT9147_H

/**************平台相关代码******************/
#include "stm32f1xx_hal.h"

/********************************************/

#define MAX_TOUCH_PIONT_COUNT 5

#defien GT_GET_BUFF_STATE(a)   (uint8_t)(a & 0x80)
#defien GT_GET_TOUCH_COUNT(a)  (uint8_t)(a & 0x0f)

//IO操作函数	   
#define GT_RST    		PFout(11)	//GT9147复位引脚
#define GT_INT    		PFin(10)	//GT9147中断引脚	
 
//I2C读写命令	
#define GT_CMD_WR 		0X28     	//写命令
#define GT_CMD_RD 		0X29		//读命令
  
//GT9147 部分寄存器定义 
#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0X8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址  

typedef struct {
	uint16_t x[MAX_TOUCH_PIONT_COUNT];
	uint16_t y[MAX_TOUCH_PIONT_COUNT];
}point_gt;

typedef struct{
	uint16_t null_point_x;
	uint16_t gain_point_y;
}adjusting_gt;

typedef struct{
 adjusting_gt adjusting;
 point_gt     point;
}touch_data;	

typedef struct{
	touch_data data;
}touch_handle;

extern touch_handle htouch;

uint8_t write_config_gt9147(uint8_t mode);

uint8_t write_reg_gt9147(uint16_t reg_addr,uint8_t *buf,uint8_t len);

uint8_t read_reg_gt9147(uint16_t reg_addr,uint8_t *buf,uint8_t len);

uint8_t init_gt9147(touch_handle* htouch);

uint8_t get_finger_count(uint8_t mode);

uint8_t read_point_gt9147(touch_handle* htouch);

void    callBack_gt9147(touch_handle* htouch);

#endif 

#include"gt9147.h"
#include "iic.h"
touch_handle htouch;
const uint8_t config_table_gt9147[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
}; 

uint8_t write_reg_gt9147(uint16_t reg_addr,uint8_t *buf,uint8_t len){
	uint8_t i;
	uint8_t ret=0;
	
	start_iic();
	send_byte_iic(GT_CMD_WR);
	wait_ack_iic();
	
	send_byte_iic(reg_addr >> 8);
	wait_ack_iic();

	send_byte_iic(reg_addr & 8);
	wait_ack_iic();	
	
	for(i=0;i<len;i++){
		send_byte_iic(buf[i]);
		ret=wait_ack_iic();
	}
	stop_iic();
	
	return ret;
}

uint8_t read_reg_gt9147(uint16_t reg_addr,uint8_t *buf,uint8_t len){
	uint8_t i;
	
	start_iic();
	send_byte_iic(GT_CMD_WR);
	wait_ack_iic();
	
	send_byte_iic(reg_addr >> 8);
	wait_ack_iic();

	send_byte_iic(reg_addr & 8);
	wait_ack_iic();	
	
	start_iic();
	send_byte_iic(GT_CMD_RD);
	wait_ack_iic();
	
	for(i=0;i<len;i++){
		buf[i]=read_byte_iic(len - 1 ? 0 : 1);
	}
	
	stop_iic();
	
	return 0;
}

void init_io(void){
	
#ifdef STM32F103xE
	/*init io pin*/	
	
	/*init iic*/	
	
	/*init interrupt*/
#endif
}

/*
*@mode =1 参数保存到flash
*@mode =0 参数不保存到flash
*/

uint8_t write_config_gt9147(uint8_t mode){
	uint8_t buf[2];
	uint8_t i=0;
	
	buf[1]=mode;
	
	for(i=0;i<sizeof(config_table_gt9147);i++){// sum
		buf[0]+=config_table_gt9147[i];
	}
	
	/*写入配置信息*/
	write_reg_gt9147(GT_CFGS_REG,(uint8_t*)config_table_gt9147,sizeof(config_table_gt9147));
	/*写入校验和*/
	write_reg_gt9147(GT_CHECK_REG,buf,2);
}

uint8_t init_gt9147(touch_handle* htouch){
	
	init_io();

	/*初始化校准参数*/
	htouch->data.adjusting.gain_point_y=NULL;
	htouch->data.adjusting.null_point_x=NULL;
	
	write_config_gt9147(1);
}	

/*
*return 触摸点数
* mode=0 正常扫描
*/
uint8_t get_finger_count(uint8_t mode){
	uint8_t buf[1]={0};
	uint8_t ret=0;
	
	read_reg_gt9147(GT_GSTID_REG,buf,1);
	
	/*清除buff标志位*/
	buf[0]=0x80;
	write_reg_gt9147(GT_GSTID_REG,buf,1);
	
	return GT_GET_TOUCH_COUNT(buf[0]);
}

uint8_t  read_point_gt9147(touch_handle* htouch){
	touch_handle touch;
	uint8_t i;
	uint8_t count=0;
	uint8_t buf[4];
	
	count=get_finger_count(0);
	for(i=0;i<count;i++){
		switch(i){
			case 0:
				read_reg_gt9147(GT_TP1_REG,buf,4);
				break;
			
			case 1:
				read_reg_gt9147(GT_TP2_REG,buf,4);
				break;
			
			case 2:
				read_reg_gt9147(GT_TP3_REG,buf,4);
				break;
			
			case 3:
				read_reg_gt9147(GT_TP4_REG,buf,4);
				break;
			
			case 4:
				read_reg_gt9147(GT_TP5_REG,buf,4);	
				break;
			
			default: 
				break;
		}
		
		htouch->data.point.x[i]= (uint16_t)(buf[i + 0]<<8) | buf[i + 1];
		htouch->data.point.y[i]= (uint16_t)(buf[i + 2]<<8) | buf[i + 3];
	}
	
	/*返回触摸点数*/
	return count;
	
}
	



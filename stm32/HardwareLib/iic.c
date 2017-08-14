#include "iic.h"

iic_port_inf iic_io;

#define IIC_SET_SDA_UP()  HAL_GPIO_WritePin(iic_io.sda_port,iic_io.sda_pin,GPIO_PIN_SET)
#define IIC_SET_SDA_LOW() HAL_GPIO_WritePin(iic_io.sda_port,iic_io.sda_pin,GPIO_PIN_RESET);

#define IIC_SET_SCL_UP()  HAL_GPIO_WritePin(iic_io.scl_port,iic_io.scl_pin,GPIO_PIN_SET);
#define IIC_SET_SCL_LOW()	HAL_GPIO_WritePin(iic_io.scl_port,iic_io.scl_pin,GPIO_PIN_RESET);

#define IIC_GET_SDA_STATE() HAL_GPIO_ReadPin(iic_io.sda_port,iic_io.sda_pin)

void init_bsp_iic(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = iic_io.sda_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init( iic_io.sda_port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = iic_io.scl_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init( iic_io.scl_port, &GPIO_InitStruct);
}

void delay_iic() {	
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}

/*
* @dir 1 OUT PUT 
* @dir 0 IN PUT 
*/
void set_dir_sda(uint8_t dir){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(dir){
		GPIO_InitStruct.Pin = iic_io.sda_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init( iic_io.sda_port, &GPIO_InitStruct);
	}
	else{
		GPIO_InitStruct.Pin = iic_io.sda_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init( iic_io.sda_port, &GPIO_InitStruct);
	}
}	
/*
port1 sda port
pin1  sda pin

port2 scl port
pin2  scl pin

*/

void init_iic(GPIO_TypeDef port1 ,uint16_t pin1,GPIO_TypeDef port2 ,uint16_t pin2){
	
	*(iic_io.sda_port)=port1;
	*(iic_io.scl_port)=port2;
	
		iic_io.sda_pin=pin1;
		iic_io.scl_pin=pin2;
	
		init_bsp_iic();
}	

void start_iic(){
	set_dir_sda(1);     
	IIC_SET_SDA_UP();	  	  
	IIC_SET_SCL_UP();
	HAL_Delay(30);
 	IIC_SET_SDA_LOW();
	delay_iic();
	IIC_SET_SCL_LOW(); 
}	

void stop_iic(void){
	set_dir_sda(1);
	IIC_SET_SCL_UP();
	HAL_Delay(30);
	IIC_SET_SDA_LOW();
	delay_iic();
	IIC_SET_SDA_UP();
}

void send_byte_iic(uint8_t txd){
    uint8_t t;   
	
	set_dir_sda(1); 	    
    IIC_SET_SCL_LOW();//拉低时钟开始数据传输
	delay_iic();
	for(t=0;t<8;t++)
    {              
		if((txd &0x80)>>7){
			IIC_SET_SDA_UP();
		}
		else{
			IIC_SET_SDA_LOW();
		}	
        txd<<=1; 	      
		IIC_SET_SCL_UP(); 
		delay_iic();
		IIC_SET_SCL_LOW();	
		delay_iic();
    }	 
}
	
uint8_t read_byte_iic(uint8_t if_need_ack){
	uint8_t i,receive=0;
	
 	set_dir_sda(0);//SDA设置为输入
	HAL_Delay(30);
	for(i=0;i<8;i++ )
	{ 
		IIC_SET_SCL_LOW(); 	    	   
		delay_iic();
		IIC_SET_SCL_UP();	 
		receive<<=1;
		
		if(IIC_GET_SDA_STATE())
			receive++;   
	}	  				 
	if (!if_need_ack)send_no_ack_iic();//发送nACK
	else send_ack_iic(); //发送ACK   
 	return receive;	
}

uint8_t wait_ack_iic(void){
	uint8_t ucErrTime=0;
	
	set_dir_sda(0);      //SDA设置为输入  
	IIC_SET_SDA_UP();	   
	IIC_SET_SCL_UP();
	delay_iic();
	while(IIC_GET_SDA_STATE())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			stop_iic();
			return 1;
		} 
		delay_iic();
	}
	IIC_SET_SCL_LOW(); 	   
	return 0;  	
}

void send_ack_iic(void){
	IIC_SET_SCL_LOW();
	set_dir_sda(1);
	delay_iic();
	IIC_SET_SDA_LOW();
	delay_iic();
	IIC_SET_SCL_UP();
	delay_iic();
	IIC_SET_SCL_LOW();	
}

void send_no_ack_iic(void){
	IIC_SET_SCL_LOW();
	set_dir_sda(1);
	delay_iic();
	IIC_SET_SDA_UP();
	delay_iic();
	IIC_SET_SCL_UP();
	delay_iic();
	IIC_SET_SCL_LOW();	
}


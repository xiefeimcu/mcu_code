
#include "gprs_m35.h"
#include "usart.h"
#include "string.h"

handle_m35_t hm35;

uint8_t defualt_tcp_server_domain[]="yz.dtu.hk";
uint8_t defualt_tcp_server_ip[]="218.145.64.217";
uint8_t defualt_tcp_server_port[]="22154";

state_m35 search_string(uint8_t *target,uint8_t *src,uint8_t len){
	uint8_t count=0;
	uint8_t *p1=target,*p2=src;
	uint8_t last=0;
	
	while((*p1) && (*p2)){
		if(*p2 == *p1){
			p1++;
			p2++;
			count++;
			last=1;
		}else{
			if(last){
				count=0;
				break;
			}
			p1++;
		}
	}
	
	if(count>=len)
		return M35_OK;
	else
		return M35_ERR; 
}

void copy_string(uint8_t *target,uint8_t *src){
	while(*src){
		*(target++) = *(src++);
	}
}

state_m35 test_at(void){
	uint8_t rx_buf[6];
	hm35.operat.send_uart("AT+?\r\n",sizeof("AT+?\r\n"));
	hm35.operat.receive_uart(rx_buf,sizeof(rx_buf),50);
	if(search_string(rx_buf,"\r\nOK\r\n",6)){
			return M35_OK;
	}
	return M35_ERR;
}

state_m35 init_param(void){
	//eeprom 
	if(0){
	
	}
	//defualt
	else{
		//hm35.net_param.addr_inf.add_type=IP;
		hm35.net_param.addr_inf.add_type=DOMAIN;
		hm35.net_param.link_type=TCP_CLIENT;
		
		memset(hm35.net_param.addr_inf.server_addr,0,MAX_ADDR_LEN);
		
		//init yz tcp server addr and port
		if(hm35.net_param.addr_inf.add_type==IP){
			copy_string(hm35.net_param.addr_inf.server_addr,defualt_tcp_server_ip);
		}
		else{
			copy_string(hm35.net_param.addr_inf.server_addr,defualt_tcp_server_domain);
		}
		
		copy_string(hm35.net_param.addr_inf.server_port,defualt_tcp_server_port);
	}
	return M35_OK;
}

state_m35 register_net(void){
	hm35.operat.send_uart("AT+CGATT=1\r\n",sizeof("AT+CGATT=1\r\n"));
	hm35.operat.delay(75);
	hm35.operat.send_uart("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n",sizeof("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n"));
	hm35.operat.delay(300);
	
	return M35_OK;
}

state_m35 built_link(void){
	uint8_t cmd_open_tcp[]={"AT+QIOPEN=\"TCP\",\"                    \",\"22154\"\r\n"};
	
	if(hm35.net_param.link_type==TCP_SERVER){
	}
	else if(hm35.net_param.link_type==TCP_CLIENT){
		
	  if(hm35.net_param.addr_inf.add_type==IP){
			hm35.operat.send_uart("AT+QIDNSIP=0\r\n",sizeof("AT+QIDNSIP=1\r\n"));
		}
		else if(hm35.net_param.addr_inf.add_type==DOMAIN){
			hm35.operat.send_uart("AT+QIDNSIP=1\r\n",sizeof("AT+QIDNSIP=1\r\n"));
		}
		else {
			assert_param(0); 
		}
	
		copy_string(cmd_open_tcp+17,hm35.net_param.addr_inf.server_addr);
		copy_string(cmd_open_tcp+39,hm35.net_param.addr_inf.server_port);
		hm35.operat.send_uart(cmd_open_tcp,sizeof(cmd_open_tcp));
		
	}
	else if(hm35.net_param.link_type==UDP){
		hm35.operat.send_uart("",sizeof(""));
	}
	else {
		assert_param(0);
	}
	//进入透明传输模式
	return M35_OK;
}

void disconnect(void){
	hm35.operat.send_uart("AT+QICLOSE\r\n",sizeof("AT+QICLOSE\r\n"));
	hm35.operat.delay(300);
	hm35.operat.send_uart("AT+CGATT=0\r\n",sizeof("AT+CGATT=0\r\n"));
	hm35.operat.delay(75);
}

uint8_t hal_receive_uart(uint8_t *data, uint8_t len,uint32_t time_out){
	HAL_UART_Receive(&huart1,data,len,time_out);
	//TODO 接收字节数是否为 RxXferSize ?
	return huart1.RxXferSize;
}

void hal_send_uart(uint8_t *data, uint8_t len){
	HAL_UART_Transmit(&huart1,data,len,5);
}

void set_transparent_mode(uint8_t mode){
	//非透明模式
	if(mode){
		hm35.operat.send_uart("AT+QIMODE=1\r\n",sizeof("AT+QIMODE=0\r\n"));
	}
	//透明模式
	else{
			hm35.operat.send_uart("AT+QIMODE=0\r\n",sizeof("AT+QIMODE=0\r\n"));
	}
} 

void socket_send(uint8_t *data,uint16_t len){
	uint8_t ctrl_z[2]={0x2c,0x1a};
	
	hm35.operat.send_uart("AT+QISEND\r\n",sizeof("AT+QISEND\r\n"));
	hm35.operat.delay(5);
	hm35.operat.send_uart(data,len);
	hm35.operat.delay(5);
	hm35.operat.send_uart(ctrl_z,2);
}

void init_m35(void){
	memset(&hm35,0,sizeof(hm35));
	
	hm35.operat.test_at_cmd=test_at;
	hm35.operat.built_link=built_link;
	hm35.operat.disconnect=disconnect;
	hm35.operat.init_param=init_param;
	hm35.operat.register_net=register_net;
	
	hm35.operat.receive_uart=hal_receive_uart;
	hm35.operat.send_uart=hal_send_uart;
	hm35.operat.delay=HAL_Delay;
	hm35.operat.set_transparent_mode=set_transparent_mode;
	hm35.operat.socket_send=socket_send;
}


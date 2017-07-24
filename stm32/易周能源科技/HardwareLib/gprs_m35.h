
#ifndef _M35_AT_CMD

#define _M35_AT_CMD

//AT+QIOPEN 建立 TCP 连接或注册 UDP 端口号

//AT+QISEND 发送 TCP 或 UDP 数据

//AT+QICLOSE 关闭 TCP 或 UDP 连接

#include "stm32f1xx_hal.h"

const  defualt_tcp_server_domain="yz.dtu.hk";
const  defualt_tcp_server_ip="218.145.64.217";
const  defualt_tcp_server_port="22154";

typedef enum{
	M35_ERR=-1,
	M35_OK,
}state_m35;

typedef enum{
	TCP_SERVER,
	TCP_CLIENT,
	UDP,
}link_type;

typedef enum{
	DOMAIN,
	IP,
}link_type;

typedef struct{
	uint8_t server_addr[20];
	uint8_t self_ip[20];
	uint32_t server_port;
	uint32_t self_port;
	uint8_t  add_type;
}addr_inf_t;

typedef struct{
	uint8_t  	link_type;
	addr_inf_t  addr_inf; 	
}net_param_t;

typedef struct{
	state_m35 (*init_param)   (void);
	state_m35 (*test_at_cmd)  (void);
	state_m35 (*register_net) (void);
	state_m35 (*built_link)   (net_param_t net_inf);
	void      (*disconnect)   (void);
	void      (*send_uart)    (uint8_t *data, uint8_t len);
	uint8_t   (*receive_uart) (uint8_t *data, uint8_t len,uint32_t time_out);
	void      (*delay)        (uint32_t);
	void      (*set_transparent_mode) (uint8_t mode);
	void      (*socket_send)  (uint8_t *data,uint16_t len);
	void      (*socket_receive_callBack) (uint8_t *data,uint16_t len);
}operat_t;

typedef struct{
	operat_t    operat;
	net_param_t net_param;
}handle_m35_t;

extern handle_m35_t hm35;

#endif




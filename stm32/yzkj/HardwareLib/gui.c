
#include "gui.h"
#include "string.h"

menu_t  menu[5*2];

uint8_t gram[LCD_LONG / 8][LCD_WIDTH / 8];

void clear_gram(void){
	memset(gram,0,sizeof(gram));
}

void ui_drew_point(uint8_t x,uint8_t y,fill_type fill_type){
	uint8_t xaddr_byte=(x >> 8) ;
	uint8_t yaddr_byte=(y >> 8) ;
	
	uint8_t bit_pos;
	
	bit_pos =x-( xaddr_byte << 8);
	
	if(fill_type == FILL_TYPE_DEFUALT)
		gram[xaddr_byte][yaddr_byte] |= (0x80 >> bit_pos);
	
	else if(fill_type == FILL_TYPE_CLEAR)
		gram[xaddr_byte][yaddr_byte] &= ~(0x80 >> bit_pos);
	
	else if(fill_type == FILL_TYPE_INVERSE)
		gram[xaddr_byte][yaddr_byte] ^= (0x80 >> bit_pos);
}

void ui_drew_line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,fill_type fill_type){
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		ui_drew_point(uRow,uCol,fill_type);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 	
}

void ui_drew_rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,fill_type fill){
	uint8_t dy;
	if(fill){
		for(dy=y1;dy<=y2;dy++){
			ui_drew_line(x1,dy,x2,dy,FILL_TYPE_DEFUALT);
		}
		
	}else{
		ui_drew_line(x1,y1,x2,y1,FILL_TYPE_DEFUALT);
		ui_drew_line(x1,y1,x1,y2,FILL_TYPE_DEFUALT);
		ui_drew_line(x2,y2,x1,y2,FILL_TYPE_DEFUALT);
		ui_drew_line(x2,y2,x2,y1,FILL_TYPE_DEFUALT);
	}
	
}

void ui_creat_menu(void){
//	const	char	menu_string[5*3][LCD_LONG / 8 +2]={
//		"基本设置",/*0*/
//		"通信设置",
//		"测控设置",
//		"出厂设置",
//		
//		"更改日期",/*4*/
//		"背光时间",
//		"休眠时间",
//		
//		"波特率RS485_1",/*7*/
//		"波特率RS485_2",
//		"波特率RS232_1",
//		"波特率RS232_2",
//	};
	
//	strcpy((char *)menu[0].string,"基本设置");
//	menu[0].last=NULL;
//	menu[0].next=menu[4]
	
}


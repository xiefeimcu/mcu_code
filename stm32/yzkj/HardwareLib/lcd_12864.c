
#include "lcd_12864.h"
#include "main.h"
#include "delay.h"


 /******************************************************************************* 
【产品型号】：BJ12864M-1
【点 阵 数】：128X64
【外型尺寸】：93X70
【视域尺寸】：72X40
【定位尺寸】：88X64
【点 间 距】：0.52X0.52
【点 大 小】：0.48X0.48
【控制器类型】：ST7920
【简单介绍】：带字库，SPI串并口，3.3V或5V

DDRAM地址
0x80  0x81  0x82  0x83  0x84  0x85  0x86  0x87    //第一行汉字位置
0x90  0x91  0x92  0x93  0x94  0x95  0x96  0x97    //第二行汉字位置
0x88  0x89  0x8a  0x8b  0x8c  0x8d  0x8e  0x8f    //第三行汉字位置
0x98  0x99  0x9a  0x9b  0x9c  0x9d  0x9e  0x9f    //第四行汉字位置

*******************************************************************************/  
void init(void);
void show_char(uint8_t  row, uint8_t  col,uint8_t  dat);
void show_strings(uint8_t  row, uint8_t  col,uint8_t  *s);
void show_gbs(uint8_t  row, uint8_t  col, uint8_t  *s);
void show_num(uint8_t  row, uint8_t  col, uint16_t num,uint8_t  DecOrHex);
void lcd_dis_image(const uint8_t *str);
void lcd_test_ban_dot(void);
void close_power(void);
void open_power(void);
void close_bk(void);
void open_bk(void);


const lcd_12864_opt lcd_handle={
	.init=						init,
	.show_char=				show_char,
	.show_string=			show_strings,
	.close_power=			close_power,
	.close_back_light=close_bk,
	.open_back_light=	open_bk,
	.open_power=			open_power,
	.show_gbs=				show_gbs,
	.show_image=			lcd_dis_image,
	.show_num=        show_num,
};

void close_power(void){
	LCD_CLOSE_PW();
}
	
void open_power(void){
	LCD_OPEN_PW();
}

void close_bk(void){
	LCD_CLOSE_BK();
}

void open_bk(void){
	LCD_OPEN_BK();
}


/*
* 1 out
* 0 in
*/
void set_lcd_busy_pin_dir(uint8_t dir){
	GPIO_InitTypeDef GPIO_SetStruct;
	
	GPIO_SetStruct.Pin=LCD_BUSY_PIN;
	
	if(dir){
	 GPIO_SetStruct.Mode=GPIO_MODE_OUTPUT_PP;
	}
	else{
	 GPIO_SetStruct.Mode=GPIO_MODE_INPUT;
	}
	
	HAL_GPIO_Init(LCD_BUSY_PORT,&GPIO_SetStruct);
}
	
	
int8_t check_busy(uint8_t time_out){
	uint32_t time_last=HAL_GetTick();
	int8_t ret=0;
	
	LCD_RS_CMD();
	LCD_RW_READ();
	set_lcd_busy_pin_dir(0);
	
	do{
		if(HAL_GPIO_ReadPin(LCD_BUSY_PORT,LCD_BUSY_PIN)== GPIO_PIN_RESET){
			break;
		}
		
		if(HAL_GetTick() > (time_out + time_last) ){
			ret= -1;
			break;
		}
		
		LCD_E_ENABLE();
		__NOP();
	    __NOP();
		LCD_E_DISABLE();		
	}while(1);
	
	set_lcd_busy_pin_dir(1);
	return ret;
}

void send_cmd (uint8_t cmd){
	
	if(!check_busy(10)){
		log_err(LCD_BUSY_ERR);
	}
	
	LCD_RW_WRITE();
	LCD_RS_CMD();
	LCD_SETUP_DATA(cmd);
	
	LCD_E_ENABLE();
	__NOP();
	__NOP();
	LCD_E_DISABLE();
}

void send_data(uint8_t data){
	
	if(!check_busy(10)){
		log_err(LCD_BUSY_ERR);
	}
	
	LCD_RW_WRITE();
	LCD_RS_DATA();
	LCD_SETUP_DATA(data);
	
	LCD_E_ENABLE();
	__NOP();
	__NOP();
	LCD_E_DISABLE();
}

void init(void){
	LCD_OPEN_PW();
	LCD_OPEN_BK();
	//1>芯片上电；
	//2>延时40ms以上；
	//3>复位操作：RST出现一个上升沿（RST=1;RST=0;RST=1;）
    HAL_Delay(50);  // this delay is necessary !
    send_cmd(0x30);//功能设置，一次送8位数据，基本指令集
    send_cmd(0x0C);//0000,1100  显示状态开关：整体显示ON，光标显示OFF，光标显示反白OFF
    //send_cmd(0x0f);//整体显示 ON C=1； 游标ON(方块闪烁) B=1； 游标位置(游标下划线)ON
    //如果有按键输入数字，则开光标，左右移动：send_cmd(0x10); send_cmd(0x10);  
    send_cmd(0x01);//0000,0001 清除显示DDRAM
    send_cmd(0x02);//0000,0010 DDRAM地址归位
    //send_cmd(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC
    //send_cmd(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加1
    send_cmd(0x06); //点设定，画面不移动,游标右移，DDRAM位地址加1
    HAL_Delay(10);  // this delay is necessary !
	
}

void lcd_clear_ddram(void)
{
  send_cmd(0x01);  //基本指令集
  HAL_Delay(2);        //datasheet >=1.4ms 
}


void lcd_clear_gdram(void)
{    
  uint8_t j, i;
  send_cmd(0x36);
  for(j=0;j<32;j++) //上半屏32点行
  {
    send_cmd(0x80+j);
    send_cmd(0x80);//X坐标
    for(i=0;i<32;i++)//32表示如1行写完,自动写第3行
    {
      send_data(0x00);  
    }
  }
  send_cmd(0x30);     //基本指令 
}

/******************************************************************************* 
* 名称 :  lcd_set_pos
* 功能 : 设定显示位置,只适用于标准指令集 
* 输入 :  x (0~3)行
          y (0~127)列
* 输出 : 无 
*******************************************************************************/  
void lcd_set_pos(uint8_t  row, uint8_t  col)  
{  
    uint8_t  pos;  
    if( row == 0)  
            row = 0x80;  
    else if(row == 1)  
            row = 0x90;  
    else if(row == 2)  
            row = 0x88;  
    else if(row == 3)  
            row = 0x98;     
    pos = row + col;  
    send_cmd(pos);//在写入数据前先指定显示地址 
    delay_us(8);
} 


/******************************************************************** 
* 名称 : ShowCharPos 
* 功能 : 在当前坐标位置显示单个字符,col=0,1,2,每个col占16dot
* 输入 : uint8_t  row, uint8_t  col,uint8_t  dat
* 输出 : 无 
***********************************************************************/ 
void show_char(uint8_t  row, uint8_t  col,uint8_t  dat)
{    
  lcd_set_pos(row, col);
  send_data(dat);
}

/******************************************************************************* 
* 名称 : ShowEnString(uint8_t  *s)  
* 功能 : 显示英文字符串  col=0,1,2,1 col=16dot
* 输入 : *s 
* 输出 : 无 
*******************************************************************************/  
void show_string(uint8_t  *s)  
{    
    uint8_t   i = 0;  
    while(s[i] != '\0')  
    {   
      send_data(s[i++]);      
    }  
}  

void show_strings(uint8_t  row, uint8_t  col,uint8_t  *s)     //col is full char wide 
{  
     uint8_t   i = 0;  
     lcd_set_pos(row, col);    
    while(s[i] != '\0')  
    {       
      send_data(s[i++]);        
      if((2*col+i)%(16)==0)     //must 16, is OK
      {          
        lcd_set_pos(++row,0);     //display start at next row.
      }
   
    }  
}  


/******************************************************************************* 
* 名称 : show_gb 
* 功能 : 显示单个汉字
* 输入 : i 
* 输出 : 无 
********************************************************************************/  
void send_gb( unsigned char *HZ)
{
   send_data(HZ[0]);                //写入汉字的高八位数据
   send_data(HZ[1]);                //写入汉字的低八位数据 
}

void show_gb(uint8_t  row, uint8_t  col, uint8_t  *HZ)
{
   lcd_set_pos(row, col);
   send_gb(HZ);
}


/******************************************************************** 
* 名称 : show_gbs
* 功能 : 显示多个汉字
* 输入 : uint8_t  row, uint8_t  col :汉字串的起始地址
* 输出 : 无 
* 说明 : 自动换行:lcm默认换行顺序是乱的，0--2--1--3--0
***********************************************************************/  
void show_gbs(uint8_t  row, uint8_t  col, uint8_t  *s)
{
  uint8_t  i = 0;
  lcd_set_pos(row, col);
  while (s[i] != '\0')
  {
    send_gb(s+i);
    i+=2;   
    if((2*col+i)%(16)==0)                 //如果满一行
    {            
       lcd_set_pos(++row,0);            //重新设置显示的起始地址
    }     
  } 
}



/******************************************************************** 
* 名称 : show_blank_by_pos 
* 功能 : 
* 输入 : col = 0~7 1num =1半宽字符
* 功能 : 黑板擦功能，擦除不显示的内容
***********************************************************************/ 
void show_blanks(uint8_t  row, uint8_t  col, uint8_t  num)
{
  uint8_t  i ;
  lcd_set_pos(row,col);   
  for (i = 0; i < num;i++) 
    send_data(0x20); //写空格
}


/******************************************************************** 
* 名称 : show_num
* 功能 : 
* 输入 : uint8_t  row, uint8_t  col 起始地址 col=0,1,2,每个col占16dot
* 输出 : 无 
* 说明 : 自动换行:lcm默认换行顺序是乱的，0--2--1--3--0
***********************************************************************/ 
void show_num(uint8_t  row, uint8_t  col, uint16_t num,uint8_t  DecOrHex)
{     
    char buf[16]; 
    if(DecOrHex==10)  // dex 
    sprintf(buf, "%d", num);
    else
    sprintf(buf,"%X",num);  //"0x%X"
    show_strings(row,col,(uint8_t  *)(buf));
}

void lcd_dis_use_char(uint8_t row, uint8_t col,uint8_t index)
{
  lcd_set_pos(row, col);   //cgram char 映射输出的DDRAM地址
  send_data(0x00);	//Must exist!
  send_data( (index-1)*2);	// cgram :00,02,04,06  第index个字符存储地址偏移量
}

void lcd_dis_image(const uint8_t *str)
{
   uint16_t i,j;
   
   send_cmd(0x36);	//绘图显示开，扩充指令集extended instruction(DL=8BITS,RE=1,G=1)
   delay_us(370);		  //delay is important!
   
//*******显示上半屏内容  
   for(i=0;i<32;i++)              //20H
    { 
      send_cmd(0x80 + i);   //SET  垂直地址 VERTICAL ADD
      send_cmd(0x80);       //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         send_data(*str++);     
       }
    }

//*******显示下半屏内容
   for(i=0;i<32;i++)        
    {
      send_cmd(0x80 + i); //SET 垂直地址 VERTICAL ADD
      send_cmd(0x88);     //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         send_data(*str++);
       }
    }
  
}

void lcd_test_ban_dot(void)
{
   uint16_t i,j;
   
   send_cmd(0x36);	//绘图显示开，扩充指令集extended instruction(DL=8BITS,RE=1,G=1)
   delay_us(370);		//delay is important!
   
//*******显示上半屏内容  
   for(i=0;i<32;i++)              //20H
    { 
      send_cmd(0x80 + i);   //SET  垂直地址 VERTICAL ADD
      send_cmd(0x80);       //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
        send_data(0xff);   
       }
    }

//*******显示下半屏内容
   for(i=0;i<32;i++)        
    {
      send_cmd(0x80 + i); //SET 垂直地址 VERTICAL ADD
      send_cmd(0x88);     //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         send_data(0xff);   
       }
    }
}




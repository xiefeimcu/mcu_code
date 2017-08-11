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

/*
* 1 out
* 0 in
*/
void set_rw_dir(uint32_t mode){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Mode=mode;
	GPIO_InitStruct.Pin=LCD_RW_PIN;
	
	HAL_GPIO_Init(LCD_RW_PORT,&GPIO_InitStruct);
	
}

void sendbyte(uint8_t zdata){
	uint8_t i;
	//set_rw_dir(GPIO_MODE_OUTPUT_PP);
	
	for(i=0;i<8;i++){
		if((zdata << i ) & 0x80){
			LCD_RW_HIGH();
		}
		else{
			LCD_RW_LOW();
		}
		
		LCD_E_LOW();
		delay_us(10);
		LCD_E_HIGH();
	}

}

static uint8_t ReceiveByte(void)
{
   uint8_t i,d1,d2;
	
	set_rw_dir(GPIO_MODE_INPUT);
	
   for (i = 0; i < 8; i++)
   {
    LCD_E_LOW();
    delay_us(10);
    LCD_E_HIGH();
    
    if (LCD_RW_READ() == GPIO_PIN_SET) d1++;
    d1 = d1<<1;
   }
   
   for (i = 0; i < 8; i++)
   {
    LCD_E_LOW();
     delay_us(10);
    LCD_E_HIGH();   
    if (LCD_RW_READ() == GPIO_PIN_SET) d2++;
    d2 = d2<<1;
   }
   
	 set_rw_dir(GPIO_MODE_OUTPUT_PP);
	 
   return (d1&0xf0 + d2&0x0f);
}

int8_t check_busy(void){
	if(ReceiveByte() & 0x80){
		return 1;
	}
	else{
		return 0;
	}
}

void send_cmd (uint8_t cmd){
	
	LCD_RS_HIGH();
	
	while(check_busy());
	
	sendbyte(WR_CMD); 
	sendbyte(cmd & 0xf0);  
	sendbyte((cmd << 4) & 0xf0); 
	while(check_busy());
		
	LCD_RS_LOW();
}

void send_data(uint8_t data){
	LCD_RS_HIGH();
	
	while(check_busy());	
	
	sendbyte(WR_DATA); 
	sendbyte(data & 0xf0);  
	sendbyte((data & 0x0f)<<4); 
	while(check_busy());
		
	LCD_RS_LOW();
}

void lcd_init(void){
	LCD_OPEN_PW();
	//LCD_OPEN_BK();
	
	//1>芯片上电；
	//2>延时40ms以上；
	//3>复位操作：RST出现一个上升沿（RST=1;RST=0;RST=1;）
	HAL_Delay(50);
	
	LCD_RST_HIGH();
	HAL_Delay(50);
	LCD_RST_LOW();
	HAL_Delay(50);
	LCD_RST_HIGH();
		
	send_cmd(0x30);
	HAL_Delay(50);  
	
	send_cmd(0x0C);
	HAL_Delay(50); 
	
	send_cmd(0x01);
	HAL_Delay(50); 
		
	send_cmd(0x06); 
	HAL_Delay(50); 
		

}

void lcd_clear_ddram(void){
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
    send_cmd(pos);
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

void lcd_show_strings(uint8_t  row, uint8_t  col,uint8_t  *s)     //col is full char wide 
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
void lcd_show_gbs(uint8_t  row, uint8_t  col, uint8_t  *s)
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
  for (i = 0; i < num;i++) {
	send_data(0x20); 
  }  
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
	
    if(DecOrHex==10)  
		sprintf(buf, "%d", num);
    else if(DecOrHex == 16)
		sprintf(buf,"%X",num);  
	
    lcd_show_strings(row,col,(uint8_t  *)(buf));
}

void lcd_dis_use_char(uint8_t row, uint8_t col,uint8_t index)
{
  lcd_set_pos(row, col);    //cgram char 映射输出的DDRAM地址
  send_data(0x00);			//Must exist!
  send_data( (index-1)*2);	// cgram :00,02,04,06  第index个字符存储地址偏移量
}

void lcd_dis_image(const uint8_t *str)
{
   uint16_t i,j;
   
   send_cmd(0x36);	//绘图显示开，扩充指令集extended instruction(DL=8BITS,RE=1,G=1)
   delay_us(370);   //delay is important!
   
/*显示上半屏内容*/
   for(i=0;i<32;i++)        
    { 
      send_cmd(0x80 + i);   
      send_cmd(0x80);       
      for(j=0;j<16;j++)
       {
         send_data(*str++);     
       }
    }

/*显示下半屏内容*/
   for(i=0;i<32;i++)        
    {
      send_cmd(0x80 + i); 
      send_cmd(0x88);     
      for(j=0;j<16;j++)
       {
         send_data(*str++);
       }
    }
  
}

void lcd_test_ban_dot(void)
{
   uint16_t i,j;
	
   /*绘图显示开，扩充指令集extended instruction(DL=8BITS,RE=1,G=1)*/
   send_cmd(0x36);	
   delay_us(370);  
   
   for(i=0;i<32;i++)       
    { 
      send_cmd(0x80 + i);  
      send_cmd(0x80);       
      for(j=0;j<16;j++)
       {
        send_data(0xaa);   
       }
    }

   for(i=0;i<32;i++)        
    {
      send_cmd(0x80 + i); 
      send_cmd(0x88);     
      for(j=0;j<16;j++)
       {
         send_data(0xaa);   
       }
    }
}




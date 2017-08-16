#include "lcd_12864.h"
#include "main.h"
#include "tim.h"

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
		TIM_delay_us(15);
		LCD_E_HIGH();
	}

}

static uint8_t ReceiveByte(void)
{
   uint8_t i,d1=0,d2=0;
	
	set_rw_dir(GPIO_MODE_INPUT);
	
   for (i = 0; i < 8; i++)
   {
    LCD_E_LOW();
    TIM_delay_us(10);
    LCD_E_HIGH();
    TIM_delay_us(10);
		 
    if (LCD_RW_READ() == GPIO_PIN_SET) 
			d1++;
		
    d1 = d1<<1;
   }
   
   for (i = 0; i < 8; i++)
   {
    LCD_E_LOW();
    TIM_delay_us(15);
    LCD_E_HIGH();  
    TIM_delay_us(15);
		 
    if (LCD_RW_READ() == GPIO_PIN_SET)
			d2++;
		
    d2 = d2<<1;
   }
   
	 set_rw_dir(GPIO_MODE_OUTPUT_PP);
	 
   return ((d1 & 0xf0) + (d2 & 0x0f));
}

int8_t check_busy(void){

	TIM_delay_us(25);
	return 0;
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
	
	//1>閼侯垳澧栨稉濠勬暩閿涳拷
	//2>瀵よ埖妞�40ms娴犮儰绗傞敍锟�
	//3>婢跺秳缍呴幙宥勭稊閿涙瓓ST閸戣櫣骞囨稉锟芥稉顏冪瑐閸楀洦閮ㄩ敍鍦T=1;RST=0;RST=1;閿涳拷
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
  send_cmd(0x01);  //閸╃儤婀伴幐鍥︽姢闂嗭拷
  HAL_Delay(2);        //datasheet >=1.4ms 
}

void lcd_clear_gdram(void)
{    
  uint8_t j, i;
  send_cmd(0x36);
  for(j=0;j<32;j++) //娑撳﹤宕愮仦锟�32閻愮顢�
  {
    send_cmd(0x80+j);
    send_cmd(0x80);//X閸ф劖鐖�
    for(i=0;i<32;i++)//32鐞涖劎銇氭俊锟�1鐞涘苯鍟撶�癸拷,閼奉亜濮╅崘娆戭儑3鐞涳拷
    {
      send_data(0x00);  
    }
  }
  send_cmd(0x30);     //閸╃儤婀伴幐鍥︽姢
}

/******************************************************************************* 
* 閸氬秶袨 :  lcd_set_pos
* 閸旂喕鍏� : 鐠佹儳鐣鹃弰鍓с仛娴ｅ秶鐤�,閸欘亪锟藉倻鏁ゆ禍搴㈢垼閸戝棙瀵氭禒銈夋肠
* 鏉堟挸鍙� :  x (0~3)鐞涳拷
          y (0~127)閸掞拷
* 鏉堟挸鍤� : 閺冿拷
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
    TIM_delay_us(8);
} 


/******************************************************************** 
* 閸氬秶袨 : ShowCharPos
* 閸旂喕鍏� : 閸︺劌缍嬮崜宥呮綏閺嶅洣缍呯純顔芥▔缁�鍝勫礋娑擃亜鐡х粭锟�,col=0,1,2,濮ｅ繋閲渃ol閸楋拷16dot
* 鏉堟挸鍙� : uint8_t  row, uint8_t  col,uint8_t  dat
* 鏉堟挸鍤� : 閺冿拷
***********************************************************************/ 
void show_char(uint8_t  row, uint8_t  col,uint8_t  dat)
{    
  lcd_set_pos(row, col);
  send_data(dat);
}

/******************************************************************************* 
* 閸氬秶袨 : ShowEnString(uint8_t  *s)
* 閸旂喕鍏� : 閺勫墽銇氶懟杈ㄦ瀮鐎涙顑佹稉锟�  col=0,1,2,1 col=16dot
* 鏉堟挸鍙� : *s
* 鏉堟挸鍤� : 閺冿拷
*******************************************************************************/  
void show_string(uint8_t  *s)  
{    
    uint8_t   i = 0;  
    while(s[i] != '\0')  
    {   
      send_data(s[i++]);      
    }  
}  

void lcd_show_strings(uint8_t  row, uint8_t  col, uint8_t  *s)     //col is full char wide 
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
* 閸氬秶袨 : show_gb
* 閸旂喕鍏� : 閺勫墽銇氶崡鏇氶嚋濮瑰鐡�
* 鏉堟挸鍙� : i
* 鏉堟挸鍤� : 閺冿拷
********************************************************************************/  
void send_gb( unsigned char *HZ)
{
   send_data(HZ[0]);                //閸愭瑥鍙嗗Ч澶婄摟閻ㄥ嫰鐝崗顐＄秴閺佺増宓�
   send_data(HZ[1]);                //閸愭瑥鍙嗗Ч澶婄摟閻ㄥ嫪缍嗛崗顐＄秴閺佺増宓�
}

void show_gb(uint8_t  row, uint8_t  col, uint8_t  *HZ)
{
   lcd_set_pos(row, col);
   send_gb(HZ);
}


/******************************************************************** 
* 閸氬秶袨 : show_gbs
* 閸旂喕鍏� : 閺勫墽銇氭径姘嚋濮瑰鐡�
* 鏉堟挸鍙� : uint8_t  row, uint8_t  col :濮瑰鐡ф稉鑼畱鐠у嘲顫愰崷鏉挎絻
* 鏉堟挸鍤� : 閺冿拷
* 鐠囧瓨妲� : 閼奉亜濮╅幑銏ｎ攽:lcm姒涙顓婚幑銏ｎ攽妞ゅ搫绨弰顖欒础閻ㄥ嫸绱�0--2--1--3--0
***********************************************************************/  
void lcd_show_gbs(uint8_t  row, uint8_t  col, uint8_t  *s)
{
  uint8_t  i = 0;
  lcd_set_pos(row, col);
  while (s[i] != '\0')
  {
    send_gb(s+i);
    i+=2;   
    if((2*col+i)%(16)==0)                 //婵″倹鐏夊鈥茬鐞涳拷
    {            
       lcd_set_pos(++row,0);            //闁插秵鏌婄拋鍓х枂閺勫墽銇氶惃鍕崳婵婀撮崸锟�
    }     
  } 
}



/******************************************************************** 
* 閸氬秶袨 : show_blank_by_pos
* 閸旂喕鍏� :
* 鏉堟挸鍙� : col = 0~7 1num =1閸楀﹤顔旂�涙顑�
* 閸旂喕鍏� : 姒涙垶婢橀幙锕�濮涢懗鏂ょ礉閹匡箓娅庢稉宥嗘▔缁�铏规畱閸愬懎顔�
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
* 閸氬秶袨 : show_num
* 閸旂喕鍏� :
* 鏉堟挸鍙� : uint8_t  row, uint8_t  col 鐠у嘲顫愰崷鏉挎絻 col=0,1,2,濮ｅ繋閲渃ol閸楋拷16dot
* 鏉堟挸鍤� : 閺冿拷
* 鐠囧瓨妲� : 閼奉亜濮╅幑銏ｎ攽:lcm姒涙顓婚幑銏ｎ攽妞ゅ搫绨弰顖欒础閻ㄥ嫸绱�0--2--1--3--0
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
  lcd_set_pos(row, col);    //cgram char 閺勭姴鐨犳潏鎾冲毉閻ㄥ嚍DRAM閸︽澘娼�
  send_data(0x00);			//Must exist!
  send_data( (index-1)*2);	// cgram :00,02,04,06  缁楃惒ndex娑擃亜鐡х粭锕�鐡ㄩ崒銊ユ勾閸э拷閸嬪繒些闁诧拷
}

void lcd_dis_image(const uint8_t *str)
{
   uint16_t i,j;
   
   send_cmd(0x36);	//缂佹ê娴橀弰鍓с仛瀵拷閿涘本澧块崗鍛瘹娴犮倝娉xtended instruction(DL=8BITS,RE=1,G=1)
   TIM_delay_us(370);   //delay is important!
   
/*閺勫墽銇氭稉濠傚磹鐏炲繐鍞寸�癸拷*/
   for(i=0;i<32;i++)        
    { 
      send_cmd(0x80 + i);   
      send_cmd(0x80);       
      for(j=0;j<16;j++)
       {
         send_data(*str++);     
       }
    }

/*閺勫墽銇氭稉瀣磹鐏炲繐鍞寸�癸拷*/
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
	
   /*缂佹ê娴橀弰鍓с仛瀵拷閿涘本澧块崗鍛瘹娴犮倝娉xtended instruction(DL=8BITS,RE=1,G=1)*/
   send_cmd(0x36);	
   TIM_delay_us(370);
   
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




//---------------------------------------------------------
//文件名称：UART_Library.c
//摘    要：串口通信
//
//作    者：赵阳
//版    本：1.0
//修改日期：2013年12月08日
//---------------------------------------------------------

#include "UART_Library.h"
#include "system.h"
#include "include.h"

unsigned char UART0_RxdBuff[UART0_RXDBUFFLENG]={0};           //UART0接收缓存
//unsigned char UART0_RxdBuff[UART0_RXDBUFFLENG]={"00123456780112342F80160009131224153136331B"};
unsigned char UART1_RxdBuff[UART1_RXDBUFFLENG]={0};           //UART1接收缓存
unsigned int UART0_RxdLeng=0;
unsigned int UART1_RxdLeng=0;
//unsigned char UART0_RxdFlag=0;
//unsigned char UART1_RxdFlag=0;

/**************************************************************
函数名称：IniUart0(unsigned char bps)
函数介绍：初始化串口0  线路空闲通信模式
输入参数：unsigned char bps-->波特率        
输出参数：无
返回值  ：无
输入参数 bps        初始化波特率
     0                  1200
     1                  2400
     2                  4800
     3                  9600 
     4                  19200
     5                  38400
     6                  57600
     7                  115200
其它值波特率设置为 9600
*************************************************************/
void IniUart0(unsigned char bps)
{
  P3SEL |= BIT4+BIT5;
  P3DIR |= BIT4;
  P3DIR &= ~BIT5;
    
  UCTL0 |= CHAR;                                
#if  UART0_UCLK==32768 //UCLK=32768HZ
  UTCTL0 |= SSEL0;     //UCLK = ACLK;
#else
  UTCTL0 |= SSEL1;     //UCLK = SMCLK;
#endif

#if  UART0_UCLK==32768 //UCLK=32768HZ
  switch(bps)
  {
    case BPS_1200: UBR00 = 0X1B;UBR10 = 0X00;UMCTL0 = 0X22;break; //1200
    case BPS_2400: UBR00 = 0X0D;UBR10 = 0X00;UMCTL0 = 0X6D;break; //2400
    case BPS_4800: UBR00 = 0X06;UBR10 = 0X00;UMCTL0 = 0XFB;break; //4800
    case BPS_9600: UBR00 = 0X03;UBR10 = 0X00;UMCTL0 = 0X4A;break; //9600
    default:UBR00 = 0X03;UBR10 = 0X00;UMCTL0 = 0X4A;break;//9600
  }
#elif UART0_UCLK==1000000  //UCLK=1MHZ
  switch(bps)
  {
    case BPS_1200: UBR00 = 0X41;UBR10 = 0X03;UMCTL0 = 0X00;break; //1200
    case BPS_2400: UBR00 = 0XA0;UBR10 = 0X01;UMCTL0 = 0X6b;break; //2400
    case BPS_4800: UBR00 = 0XD0;UBR10 = 0X00;UMCTL0 = 0X40;break; //4800
    case BPS_9600: UBR00 = 0X68;UBR10 = 0X00;UMCTL0 = 0X40;break; //9600
    case BPS_19200: UBR00 = 0X34;UBR10 = 0X00;UMCTL0 = 0X40;break; //19200
    case BPS_38400: UBR00 = 0X1A;UBR10 = 0X00;UMCTL0 = 0X00;break; //38400
    case BPS_57600: UBR00 = 0X11;UBR10 = 0X00;UMCTL0 = 0X52;break; //57600 
    case BPS_115200: UBR00 = 0X08;UBR10 = 0X00;UMCTL0 = 0X6D;break; //115200
    default:UBR00 = 0X68;UBR10 = 0X00;UMCTL0 = 0X40;break;//9600
  }
#elif UART0_UCLK==2000000  //UCLK=2MHZ 
  switch(bps)
  {
    case BPS_1200: UBR00 = 0X82;UBR10 = 0X06;UMCTL0 = 0X00;break; //1200
    case BPS_2400: UBR00 = 0X41;UBR10 = 0X03;UMCTL0 = 0X00;break; //2400
    case BPS_4800: UBR00 = 0XA0;UBR10 = 0X01;UMCTL0 = 0XC0;break; //4800
    case BPS_9600: UBR00 = 0XD0;UBR10 = 0X00;UMCTL0 = 0X40;break; //9600
    case BPS_19200: UBR00 = 0X68;UBR10 = 0X00;UMCTL0 = 0X40;break; //19200
    case BPS_38400: UBR00 = 0X34;UBR10 = 0X00;UMCTL0 = 0X40;break; //38400
    case BPS_57600: UBR00 = 0X22;UBR10 = 0X00;UMCTL0 = 0XED;break; //57600 
    case BPS_115200: UBR00 = 0X11;UBR10 = 0X00;UMCTL0 = 0X52;break; //115200
    default:UBR00 = 0XD0;UBR10 = 0X00;UMCTL0 = 0X40;break;//9600
  }
#elif UART0_UCLK==4000000  //UCLK=4MHZ
  switch(bps)
  {
    case BPS_1200: UBR00 = 0X05;UBR10 = 0X0D;UMCTL0 = 0X00;break; //1200
    case BPS_2400: UBR00 = 0X82;UBR10 = 0X06;UMCTL0 = 0X00;break; //2400
    case BPS_4800: UBR00 = 0X41;UBR10 = 0X03;UMCTL0 = 0X00;break; //4800
    case BPS_9600: UBR00 = 0XA0;UBR10 = 0X01;UMCTL0 = 0XC0;break; //9600
    case BPS_19200: UBR00 = 0XD0;UBR10 = 0X00;UMCTL0 = 0X40;break; //19200
    case BPS_38400: UBR00 = 0X68;UBR10 = 0X00;UMCTL0 = 0X40;break; //38400
    case BPS_57600: UBR00 = 0X45;UBR10 = 0X00;UMCTL0 = 0X4A;break; //57600 
    case BPS_115200: UBR00 = 0X22;UBR10 = 0X00;UMCTL0 = 0XED;break; //115200
    default:UBR00 = 0XA0;UBR10 = 0X01;UMCTL0 = 0XC0;break;//9600
  }
#elif UART0_UCLK==6000000  //UCLK=6MHZ
  switch(bps)
  {
    case BPS_1200: UBR00 = 0X88;UBR10 = 0X13;UMCTL0 = 0X00;break; //1200
    case BPS_2400: UBR00 = 0XC4;UBR10 = 0X09;UMCTL0 = 0X00;break; //2400
    case BPS_4800: UBR00 = 0XE2;UBR10 = 0X04;UMCTL0 = 0X00;break; //4800
    case BPS_9600: UBR00 = 0X71;UBR10 = 0X02;UMCTL0 = 0X00;break; //9600
    case BPS_19200: UBR00 = 0X38;UBR10 = 0X01;UMCTL0 = 0X40;break; //19200
    case BPS_38400: UBR00 = 0X9C;UBR10 = 0X00;UMCTL0 = 0X40;break; //38400
    case BPS_57600: UBR00 = 0X68;UBR10 = 0X00;UMCTL0 = 0X40;break; //57600 
    case BPS_115200: UBR00 = 0X34;UBR10 = 0X00;UMCTL0 = 0X40;break; //115200
    default:UBR00 = 0X71;UBR10 = 0X02;UMCTL0 = 0X00;break;//9600
  }
#elif UART0_UCLK==8000000  //UCLK=8MHZ
  switch(bps)
  {
    case BPS_1200: UBR00 = 0X0A;UBR10 = 0X1A;UMCTL0 = 0X00;break; //1200
    case BPS_2400: UBR00 = 0X05;UBR10 = 0X0D;UMCTL0 = 0X00;break; //2400
    case BPS_4800: UBR00 = 0X82;UBR10 = 0X06;UMCTL0 = 0X00;break; //4800
    case BPS_9600: UBR00 = 0X41;UBR10 = 0X03;UMCTL0 = 0X00;break; //9600
    case BPS_19200: UBR00 = 0XA0;UBR10 = 0X01;UMCTL0 = 0XC0;break; //19200
    case BPS_38400: UBR00 = 0XD0;UBR10 = 0X00;UMCTL0 = 0X40;break; //38400
    case BPS_57600: UBR00 = 0X8A;UBR10 = 0X00;UMCTL0 = 0XDE;break; //57600 
    case BPS_115200: UBR00 = 0X45;UBR10 = 0X00;UMCTL0 = 0X4A;break; //115200
    default:UBR00 = 0X41;UBR10 = 0X03;UMCTL0 = 0X00;break;//9600
  }
#endif
                      
  UCTL0 &= ~SWRST;                             
  ME1 |= UTXE0 + URXE0;                        
  IE1 |= URXIE0;  
  
  DELETE_UART0RXD;        //清空接收缓存
#ifdef  UART0_RS485_
  UART0_RE;                    //RS485接收模式
#endif
}

/**************************************************************
函数名称：IniUart1(unsigned char bps)
函数介绍：初始化串口1    线路空闲通信模式
输入参数：unsigned char bps-->波特率        
输出参数：无
返回值  ：无
输入参数 bps        初始化波特率
     0                  1200
     1                  2400
     2                  4800
     3                  9600 
     4                  19200
     5                  38400
     6                  57600
     7                  115200
其它值波特率设置为 9600
*************************************************************/
void IniUart1(unsigned char bps)
{
  P3SEL |= BIT6+BIT7;
  P3DIR |= BIT6;
  P3DIR &= ~BIT7;
    
  UCTL1 |= CHAR;                                
#if  UART1_UCLK==32768 //UCLK=32768HZ
  UTCTL1 |= SSEL0;     //UCLK = ACLK;
#else
  UTCTL1 |= SSEL1;     //UCLK = SMCLK;
#endif

#if  UART1_UCLK==32768 //UCLK=32768HZ
  switch(bps)
  {
    case BPS_1200: UBR01 = 0X1B;UBR11 = 0X00;UMCTL1 = 0X22;break; //1200
    case BPS_2400: UBR01 = 0X0D;UBR11 = 0X00;UMCTL1 = 0X6D;break; //2400
    case BPS_4800: UBR01 = 0X06;UBR11 = 0X00;UMCTL1 = 0XFB;break; //4800
    case BPS_9600: UBR01 = 0X03;UBR11 = 0X00;UMCTL1 = 0X4A;break; //9600
    default:UBR01 = 0X03;UBR11 = 0X00;UMCTL1 = 0X4A;break;//9600
  }
#elif UART1_UCLK==1000000  //UCLK=1MHZ
  switch(bps)
  {
    case BPS_1200: UBR01 = 0X41;UBR11 = 0X03;UMCTL1 = 0X00;break; //1200
    case BPS_2400: UBR01 = 0XA0;UBR11 = 0X01;UMCTL1 = 0X6b;break; //2400
    case BPS_4800: UBR01 = 0XD0;UBR11 = 0X00;UMCTL1 = 0X40;break; //4800
    case BPS_9600: UBR01 = 0X68;UBR11 = 0X00;UMCTL1 = 0X40;break; //9600
    case BPS_19200: UBR01 = 0X34;UBR11 = 0X00;UMCTL1 = 0X40;break; //19200
    case BPS_38400: UBR01 = 0X1A;UBR11 = 0X00;UMCTL1 = 0X00;break; //38400
    case BPS_57600: UBR01 = 0X11;UBR11 = 0X00;UMCTL1 = 0X52;break; //57600 
    case BPS_115200: UBR01 = 0X08;UBR11 = 0X00;UMCTL1 = 0X6D;break; //115200
    default:UBR01 = 0X68;UBR11 = 0X00;UMCTL1 = 0X40;break;//9600
  }
#elif UART1_UCLK==2000000  //UCLK=2MHZ
  switch(bps)
  {
    case BPS_1200: UBR01 = 0X82;UBR11 = 0X06;UMCTL1 = 0X00;break; //1200
    case BPS_2400: UBR01 = 0X41;UBR11 = 0X03;UMCTL1 = 0X00;break; //2400
    case BPS_4800: UBR01 = 0XA0;UBR11 = 0X01;UMCTL1 = 0XC0;break; //4800
    case BPS_9600: UBR01 = 0XD0;UBR11 = 0X00;UMCTL1 = 0X40;break; //9600
    case BPS_19200: UBR01 = 0X68;UBR11 = 0X00;UMCTL1 = 0X40;break; //19200
    case BPS_38400: UBR01 = 0X34;UBR11 = 0X00;UMCTL1 = 0X40;break; //38400
    case BPS_57600: UBR01 = 0X22;UBR11 = 0X00;UMCTL1 = 0XED;break; //57600 
    case BPS_115200: UBR01 = 0X11;UBR11 = 0X00;UMCTL1 = 0X52;break; //115200
    default:UBR01 = 0XD0;UBR11 = 0X00;UMCTL1 = 0X40;break;//9600
  }
#elif UART1_UCLK==4000000  //UCLK=4MHZ
  switch(bps)
  {
    case BPS_1200: UBR01 = 0X05;UBR11 = 0X0D;UMCTL1 = 0X00;break; //1200
    case BPS_2400: UBR01 = 0X82;UBR11 = 0X06;UMCTL1 = 0X00;break; //2400
    case BPS_4800: UBR01 = 0X41;UBR11 = 0X03;UMCTL1 = 0X00;break; //4800
    case BPS_9600: UBR01 = 0XA0;UBR11 = 0X01;UMCTL1 = 0XC0;break; //9600
    case BPS_19200: UBR01 = 0XD0;UBR11 = 0X00;UMCTL1 = 0X40;break; //19200
    case BPS_38400: UBR01 = 0X68;UBR11 = 0X00;UMCTL1 = 0X40;break; //38400
    case BPS_57600: UBR01 = 0X45;UBR11 = 0X00;UMCTL1 = 0X4A;break; //57600 
    case BPS_115200: UBR01 = 0X22;UBR11 = 0X00;UMCTL1 = 0XED;break; //115200
    default:UBR01 = 0XA0;UBR11 = 0X01;UMCTL1 = 0XC0;break;//9600
  }
#elif UART1_UCLK==6000000  //UCLK=6MHZ
  switch(bps)
  {
    case BPS_1200: UBR01 = 0X88;UBR11 = 0X13;UMCTL1 = 0X00;break; //1200
    case BPS_2400: UBR01 = 0XC4;UBR11 = 0X09;UMCTL1 = 0X00;break; //2400
    case BPS_4800: UBR01 = 0XE2;UBR11 = 0X04;UMCTL1 = 0X00;break; //4800
    case BPS_9600: UBR01 = 0X71;UBR11 = 0X02;UMCTL1 = 0X00;break; //9600
    case BPS_19200: UBR01 = 0X38;UBR11 = 0X01;UMCTL1 = 0X40;break; //19200 
    case BPS_38400: UBR01 = 0X9C;UBR11 = 0X00;UMCTL1 = 0X40;break; //38400
    case BPS_57600: UBR01 = 0X68;UBR11 = 0X00;UMCTL1 = 0X40;break; //57600 
    case BPS_115200: UBR01 = 0X34;UBR11 = 0X00;UMCTL1 = 0X40;break; //115200
    default:UBR01 = 0X71;UBR11 = 0X02;UMCTL1 = 0X00;break;//9600
  }
#elif UART1_UCLK==8000000  //UCLK=8MHZ
  switch(bps)
  {
    case BPS_1200: UBR01 = 0X0A;UBR11 = 0X1A;UMCTL1 = 0X00;break; //1200
    case BPS_2400: UBR01 = 0X05;UBR11 = 0X0D;UMCTL1 = 0X00;break; //2400
    case BPS_4800: UBR01 = 0X82;UBR11 = 0X06;UMCTL1 = 0X00;break; //4800
    case BPS_9600: UBR01 = 0X41;UBR11 = 0X03;UMCTL1 = 0X00;break; //9600
    case BPS_19200: UBR01 = 0XA0;UBR11 = 0X01;UMCTL1 = 0XC0;break; //19200
    case BPS_38400: UBR01 = 0XD0;UBR11 = 0X00;UMCTL1 = 0X40;break; //38400
    case BPS_57600: UBR01 = 0X8A;UBR11 = 0X00;UMCTL1 = 0XDE;break; //57600 
    case BPS_115200: UBR01 = 0X45;UBR11 = 0X00;UMCTL1 = 0X4A;break; //115200
    default:UBR01 = 0X41;UBR11 = 0X03;UMCTL1 = 0X00;break;//9600
  }
#endif
                        
  UCTL1 &= ~SWRST;                             
  ME2 |= UTXE1 + URXE1;                        
  IE2 |= URXIE1;     
  
  DELETE_UART1RXD;        //清空接收缓存
#ifdef  UART1_RS485_
  UART1_RE;                    //RS485接收模式
#endif
}

void WriteByteUART0(unsigned char value)
{
    TXBUF0 = value;                             //发送数据
    while(!(IFG1 & UTXIFG0));                   //等待发送完毕                         
    IFG1 &= ~UTXIFG0;
}

void WriteByteUART1(unsigned char value)
{
    TXBUF1 = value;                             //发送数据
    while(!(IFG2 & UTXIFG1));                   //等待发送完毕                         
    IFG2 &= ~UTXIFG1;
}

/**************************************************************
函数名称：void WriteUART0(unsigned char *p,unsigned int slenght)
函数介绍：UART0接口发送数据
输入参数：unsigned char *p-------->待发送数据首地址
          unsigned int  slenght--->发送长度
输出参数：无
返回值  ：无
*************************************************************/
void WriteUART0(unsigned char *p,unsigned int slenght)
{  
  unsigned int i=0;

#ifdef  UART0_RS485_                         //定义总线为RS485
  UART0_DE;                                       //发送状态  
#endif
  for(i=0;i<slenght;i++)
  {  
    WriteByteUART0(*p);                         //发送1byte
    p++;                                        //指向下一个数据    
  }
  UART_WriteDelay;
#ifdef  UART0_RS485_                         //定义总线为RS485
  UART_RS485Delay;                                  //延时一段时间等待485总线控制器将最后一个字节发送完毕
  UART0_RE;                                    //接收状态
#endif
}

/**************************************************************
函数名称：void WriteUART1(unsigned char *p,unsigned int slenght)
函数介绍：UART1接口发送数据
输入参数：unsigned char *p-------->待发送数据首地址
          unsigned int  slenght--->发送长度
输出参数：无
返回值  ：无
*************************************************************/
void WriteUART1(unsigned char *p,unsigned int slenght)
{  
  unsigned int i=0;
  
#ifdef  UART1_RS485_                         //定义总线为RS485
  UART1_DE;                                    //发送状态 
#endif
  for(i=0;i<slenght;i++)
  {  
    WriteByteUART1(*p);                         //发送1byte
    p++;                                        //指向下一个数据    
  }
  UART_WriteDelay;
#ifdef  UART1_RS485_                         //定义总线为RS485
  UART_RS485Delay;                             //延时一段时间等待485总线控制器将最后一个字节发送完毕
  UART1_RE;                                   //接收状态
#endif
}

/**************************************************************
函数名称：void WriteLineUART0(unsigned char *p,unsigned int slenght)
函数介绍：UART0接口从新行发送数据，自动换行，在下一行显示新内容。
输入参数：unsigned char *p-------->待发送数据首地址
          unsigned int  slenght--->发送长度
输出参数：无
返回值  ：无
*************************************************************/
void WriteLineUART0(unsigned char *p,unsigned int slenght)
{  
#ifdef  UART0_RS485_                         //定义总线为RS485
  UART0_DE;                                    //发送状态  
#endif
  WriteByteUART0(0X0D);                         //发送1byte  
  WriteByteUART0(0X0A);                         //发送1byte 
  WriteUART0(p,slenght);                       //发送数据
}

/**************************************************************
函数名称：void WriteLineUART1(unsigned char *p,unsigned int slenght)
函数介绍：UART1接口从新行发送数据，自动换行，在下一行显示新内容。
输入参数：unsigned char *p-------->待发送数据首地址
          unsigned int  slenght--->发送长度
输出参数：无
返回值  ：无
*************************************************************/
void WriteLineUART1(unsigned char *p,unsigned int slenght)
{
#ifdef  UART1_RS485_                           //定义总线为RS485
  UART1_DE;                                    //发送状态  
#endif
  WriteByteUART1(0X0D);                         //发送1byte 
  WriteByteUART1(0X0A);                         //发送1byte 
  WriteUART1(p,slenght);                       //发送数据
}

/**************************************************************
函数名称：unsigned int WriteStrUART0(unsigned char *str)
函数介绍：UART0发送字符串
输入参数：unsigned char *str-------->字符串首地址
输出参数：无
返回值  ：发送字节数（字符串长度，不包括结束符）
*************************************************************/
unsigned int WriteStrUART0(unsigned char *str)
{  
  unsigned int count=0;                         //发送长度缓存
#ifdef  UART0_RS485_                         //定义总线为RS485
  UART0_DE;                                    //发送状态  
#endif                                        
  while(*str != '\0')
  {  
    WriteByteUART0(*str);                         //发送1byte
    str++;                                        //指向下一个数据
    count++;
  }
  UART_WriteDelay;
#ifdef  UART0_RS485_                         //定义总线为RS485
  UART_RS485Delay;                                  //延时一段时间等待485总线控制器将最后一个字节发送完毕
  UART0_RE;                                    //接收状态
#endif
  return count;                                 //返回发送字节数
}

/**************************************************************
函数名称：unsigned int WriteStrUART1(unsigned char *str)
函数介绍：UART1发送字符串
输入参数：unsigned char *str-------->字符串首地址
输出参数：无
返回值  ：发送字节数（字符串长度，不包括结束符）
*************************************************************/
unsigned int WriteStrUART1(unsigned char *str)
{  
  unsigned int count=0;                         //发送长度缓存
#ifdef  UART1_RS485_                         //定义总线为RS485
  UART1_DE;                                    //发送状态  
#endif                                        
  while(*str != '\0')
  {  
    WriteByteUART1(*str);                         //发送1byte
    str++;                                      //指向下一个数据
    count++;
  }
  UART_WriteDelay;
#ifdef  UART1_RS485_                         //定义总线为RS485
  UART_RS485Delay;                                  //延时一段时间等待485总线控制器将最后一个字节发送完毕
  UART1_RE;                                    //接收状态
#endif
  return count;                                 //返回发送字节数
}

/**************************************************************
函数名称：unsigned int WriteLineStrUART0(unsigned char *str)
函数介绍：UART0接口从新行发送字符串，自动换行，在下一行显示新内容。
输入参数：unsigned char *str-------->待发送字符串首地址
输出参数：无
返回值  ：发送字节数（字符串长度，不包括结束符与回车换行符）
*************************************************************/
unsigned int WriteLineStrUART0(unsigned char *str)
{
#ifdef  UART0_RS485_                         //定义总线为RS485
  UART0_DE;                                    //发送状态  
#endif
  WriteByteUART0(0X0D);                         //发送1byte
  WriteByteUART0(0X0A);                         //发送1byte
   
  return WriteStrUART0(str);                   //发送数据
}

/**************************************************************
函数名称：unsigned int WriteLineStrUART1(unsigned char *str)
函数介绍：UART1接口从新行发送字符串，自动换行，在下一行显示新内容。
输入参数：unsigned char *str-------->待发送字符串首地址
输出参数：无
返回值  ：发送字节数（字符串长度，不包括结束符与回车换行符）
*************************************************************/
unsigned int WriteLineStrUART1(unsigned char *str)
{
#ifdef  UART1_RS485_                         //定义总线为RS485
  UART1_DE;                                    //发送状态  
#endif
  WriteByteUART1(0X0D);                         //发送1byte
  WriteByteUART1(0X0A);                         //发送1byte

  return WriteStrUART1(str);                   //发送数据 
}

void DeleteUART0_RxdBuff(unsigned int x)
{
  unsigned int i=0;
  
  if(x >= UART0_RXDBUFFLENG)
  {
    x = UART0_RXDBUFFLENG;
  }
  
  for(i=0;i<x;i++)
  {
    UART0_RxdBuff[i] = 0;
  }
  //UART0_RxdLeng = 0;
  //UART0_RxdFlag =0;
  UART0_RxdLeng = 0;
  System.Event.Bit.UART0BYTE = RESET;
  System.Event.Bit.UART0 = RESET;
}

void DeleteUART1_RxdBuff(unsigned int x)
{
  unsigned int i=0;
  if(x >= UART1_RXDBUFFLENG)
  {
    x = UART1_RXDBUFFLENG;
  }
  
  for(i=0;i<x;i++)
  {
    UART1_RxdBuff[i] = 0;
  }
  //UART1_RxdLeng = 0;
  //UART1_RxdFlag = 0;
  UART1_RxdLeng = 0;
  System.Event.Bit.UART1BYTE = RESET;
  System.Event.Bit.UART1 = RESET;
}

/**************************************************************
函数名称：unsigned char UART0_RxdPack(unsigned int ms)
函数介绍：判断一包数据是否接收完毕
输入参数：unsigned int ms-------->封包时间 毫秒
输出参数：无
返回值  ：1 接收成功  0 正在接收
*************************************************************/
unsigned char UART0_RxdPack(unsigned int ms)
{ 
  //UART0_RxdFlag = 0;
  System.Event.Bit.UART0BYTE = RESET;
  ms *= 10;
  while(ms--)
  {
    //if(1==UART0_RxdFlag)
    if(System.Event.Bit.UART0BYTE)
      return 0;
    
    UART_delayus(100);
  }
  return 1;
}

unsigned char UART1_RxdPack(unsigned int ms)
{
  //UART1_RxdFlag = 0;
  System.Event.Bit.UART1BYTE = RESET;
  ms *= 10;
  while(ms--)
  {
    //if(1==UART1_RxdFlag)
    if(System.Event.Bit.UART1BYTE)
      return 0;
    
    UART_delayus(100);
  }
  return 1;
}

void PrintHEXU1(unsigned char *padd,unsigned int length)
{ 
  unsigned int i=0;
  unsigned char asc=0;

#ifdef  UART1_RS485_                         //定义总线为RS485
  UART1_DE;                                    //发送状态 
#endif   
  WriteByteUART1(0X0D);
  WriteByteUART1(0X0A);
  WriteByteUART1('{');
  WriteByteUART1(' '); 
  for(i=0;i<length;i++)
  {   
    asc = HEXASCII[padd[i]>>4];   
    WriteByteUART1(asc); 
    asc = HEXASCII[padd[i]&0X0F];
    WriteByteUART1(asc);     
    WriteByteUART1(' '); 
  }
  WriteByteUART1('}');
  
#ifdef  UART1_RS485_                         //定义总线为RS485
  UART_RS485Delay;                             //延时一段时间等待485总线控制器将最后一个字节发送完毕
  UART1_RE;                                   //接收状态
#endif
}

void PrintHEXU0(unsigned char *padd,unsigned int length)
{ 
  unsigned int i=0;
  unsigned char asc=0;

#ifdef  UART0_RS485_                         //定义总线为RS485
  UART0_DE;                                    //发送状态 
#endif   
  WriteByteUART0(0X0D);
  WriteByteUART0(0X0A);
  WriteByteUART0('{');
  WriteByteUART0(' '); 
  for(i=0;i<length;i++)
  {   
    asc = HEXASCII[padd[i]>>4];   
    WriteByteUART0(asc); 
    asc = HEXASCII[padd[i]&0X0F];
    WriteByteUART0(asc);     
    WriteByteUART0(' '); 
  }
  WriteByteUART0('}');
  
#ifdef  UART0_RS485_                         //定义总线为RS485
  UART_RS485Delay;                             //延时一段时间等待485总线控制器将最后一个字节发送完毕
  UART0_RE;                                   //接收状态
#endif
}


void SendNum(long int value,unsigned char type,unsigned char cmd)
{
  unsigned char strtmep[16]={0};

  SInt32_TO_String(strtmep,value,type,cmd);//显示数字转换为字符串
    
  WriteLineStrUART0(strtmep);
}

void SendNumUART1(long int value,unsigned char type,unsigned char cmd)
{
  unsigned char strtmep[16]={0};

  SInt32_TO_String(strtmep,value,type,cmd);//显示数字转换为字符串
    
  WriteLineStrUART1(strtmep);
}

void sendtime(void)
{
  unsigned char timestr[22]={"2000/00/00 00:00:00"};
  
  timestr[2] = HEXASCII[System.Time.Year/10];
  timestr[3] = HEXASCII[System.Time.Year%10];
  timestr[5] = HEXASCII[System.Time.Month/10];
  timestr[6] = HEXASCII[System.Time.Month%10];
  timestr[8] = HEXASCII[System.Time.Day/10];
  timestr[9] = HEXASCII[System.Time.Day%10];
  timestr[11] = HEXASCII[System.Time.Hour/10];
  timestr[12] = HEXASCII[System.Time.Hour%10];
  timestr[14] = HEXASCII[System.Time.Minute/10];
  timestr[15] = HEXASCII[System.Time.Minute%10];
  timestr[17] = HEXASCII[System.Time.Second/10];
  timestr[18] = HEXASCII[System.Time.Second%10];

  WriteLineStrUART1(timestr);
}

/**********************数据接收中断*********************************/
/*UART0的中断服务函数*/
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX(void)
{
  if(UART0_RxdLeng < UART0_RXDBUFFLENG)
  {
    UART0_RxdBuff[UART0_RxdLeng++] = RXBUF0;
    //UART0_RxdFlag = 1;
    System.Event.Bit.UART0BYTE = SET;
     
    SETTBMS(UART0RXDDELAY);  
    TBCTL |= MC_1+TBCLR;        //打开定时器B
  }
  //LPM3_EXIT;
}          

/*UART1的中断服务函数*/
#pragma vector=UART1RX_VECTOR
__interrupt void UART1_RX(void)
{
  if(UART1_RxdLeng < UART1_RXDBUFFLENG)
  {
    UART1_RxdBuff[UART1_RxdLeng++] = RXBUF1;
    //UART1_RxdFlag = 1;
    System.Event.Bit.UART1BYTE = SET;
    SETTBMS(UART1RXDDELAY); 
    TBCTL |= MC_1+TBCLR;        //打开定时器B
  }
  //LPM3_EXIT;
}     

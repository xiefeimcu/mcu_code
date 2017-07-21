//---------------------------------------------------------
//文件名称：UART_Library.c
//摘    要：串口通信
//
//作    者：赵阳
//版    本：1.1
//修改日期：2014年04月22日 
//2014年05月21日 新增函数 int UART_printf(char uartx,const char *format, ...)
//          
//---------------------------------------------------------

#include "UART_Library.h"
#include "include.h"

char UART0_RxdBuff[UART0_RXDBUFFLENG]={0};           //UART0接收缓存
char UART1_RxdBuff[UART1_RXDBUFFLENG]={0};           //UART1接收缓存
unsigned int UART0_RxdLeng=0;
unsigned int UART1_RxdLeng=0;

/**************************************************************
函数名称：IniUart0(char bps)
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
void IniUart0(char bps)
{
  P3SEL |= BIT4+BIT5;
  /*P3DIR |= BIT4;
  P3DIR &= ~BIT5;*/
    
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

void StopU0(void)
{
   P3SEL &= ~(BIT4+BIT5);
   UCTL0 |= SWRST;
   ME1 &= ~UTXE0 + ~URXE0;
   IE1 &= ~URXIE0;
}

/**************************************************************
函数名称：IniUart1(char bps)
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
void IniUart1(char bps)
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

/**************************************************************
函数名称：void DeleteUART0_RxdBuff(unsigned int x)
函数介绍：清除UART0接收缓存
输入参数：unsigned int x----->清空长度        
输出参数：无
返回值  ：无
*************************************************************/
void DeleteUART0_RxdBuff(unsigned int x)
{
  unsigned int i=0;
  
  if(x > UART0_RXDBUFFLENG)
  {
    x = UART0_RXDBUFFLENG;
  }
  
  for(i=0;i<x;i++)
  {
    UART0_RxdBuff[i] = 0;
  }
  UART0_RxdLeng = 0;
}

void DeleteUART1_RxdBuff(unsigned int x)
{
  unsigned int i=0;
  if(x > UART1_RXDBUFFLENG)
  {
    x = UART1_RXDBUFFLENG;
  }
  
  for(i=0;i<x;i++)
  {
    UART1_RxdBuff[i] = 0;
  }
  UART1_RxdLeng = 0;
  
}

/**************************************************************
函数名称：void WriteByteUART0(char value)
函数介绍：UART0发送 1 byte
输入参数：char value-->发送的数据值        
输出参数：无
返回值  ：无
*************************************************************/
void WriteByteUART0(char value)
{
    TXBUF0 = value;                             //发送数据
    while(!(IFG1 & UTXIFG0));                   //等待发送完毕                         
    IFG1 &= ~UTXIFG0;
}
/**************************************************************
函数名称：void WriteByteUART1(char value)
函数介绍：UART1发送 1 byte
输入参数：char value-->发送的数据值        
输出参数：无
返回值  ：无
*************************************************************/
void WriteByteUART1(char value)
{
    TXBUF1 = value;                             //发送数据
    while(!(IFG2 & UTXIFG1));                   //等待发送完毕                         
    IFG2 &= ~UTXIFG1;
}

/**************************************************************
函数名称：void EndWriteUART(char uartx)
函数介绍：发送结束处理
输入参数：char uartx-->发送端口        
输出参数：无
返回值  ：无
*************************************************************/
void EndWriteUART(char uartx)
{
  UART_WriteDelay;
#ifdef  UART0_RS485_                    //定义总线为RS485
  if(0 == uartx)
  {
    UART_RS485Delay;                    //延时一段时间等待485总线控制器将最后一个字节发送完毕
    UART0_RE;                           //接收状态
  }
#endif
  
#ifdef  UART1_RS485_                    //定义总线为RS485
  if(1 == uartx)
  {
    UART_RS485Delay;                    //延时一段时间等待485总线控制器将最后一个字节发送完毕
    UART1_RE;                           //接收状态
  }
#endif
}

/**************************************************************
函数名称：void WriteUART(char uartx ,char *p,unsigned int slenght)
函数介绍：UART发送数据
输入参数：char uartx----->发送端口
          char *p-------->待发送数据首地址
          unsigned int  slenght--->发送长度
输出参数：无
返回值  ：无
发的端口参数只能为0/1，其他值将直接返回，不发送数据。
*************************************************************/
void WriteUART(char uartx ,char *p,unsigned int slenght)
{  
  unsigned int i=0;
  void (*WriteByte)(char value);        //发送1byte函数指针

  if(0 == uartx)
  {
#ifdef  UART0_RS485_                    //定义总线为RS485
    UART0_DE;                           //发送状态 
#endif
    WriteByte = WriteByteUART0;
  }  
  else if(1 == uartx)
  {
#ifdef  UART0_RS485_                    //定义总线为RS485
    UART1_DE;                           //发送状态 
#endif
    WriteByte = WriteByteUART1;
  }
  else                                  //发送端口参数错误返回
  {
    return;
  }
 
  for(i=0;i<slenght;i++)
  {  
    WriteByte(*p);                      //发送1byte
    p++;                                //指向下一个数据    
  }
  EndWriteUART(uartx);                  //发送结束        
}

/**************************************************************
函数名称：unsigned int WriteStrUART(char uartx,char *str)
函数介绍：发送字符串
输入参数：char uartx---------------->发送端口
          char *str-------->字符串首地址
输出参数：无
返回值  ：发送字节数（字符串长度，不包括结束符）
*************************************************************/
unsigned int WriteStrUART(char uartx,char *str)
{  
  unsigned int count=0;                 //发送长度计数
  void (*WriteByte)(char value);        //发送1byte函数指针

  if(0 == uartx)
  {
#ifdef  UART0_RS485_                    //定义总线为RS485
    UART0_DE;                           //发送状态 
#endif
    WriteByte = WriteByteUART0;
  }  
  else if(1 == uartx)
  {
#ifdef  UART1_RS485_                    //定义总线为RS485
    UART1_DE;                           //发送状态 
#endif
    WriteByte = WriteByteUART1;
  }
  else                                  //发送端口参数错误返回
  {
    return 0;
  }  
  
  while(*str != '\0')
  {  
    WriteByte(*str);                    //发送1byte
    str++;                              //指向下一个数据
    count++;
  }
  EndWriteUART(uartx);                  //发送结束
  return count;                         //返回发送字节数
}

/**********************数据接收中断*********************************/
/*UART0的中断服务函数*/
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX(void)
{
  if(UART0_RxdLeng < UART0_RXDBUFFLENG)         //缓存中的数据长度小于最大长度
  {
    UART0_RxdBuff[UART0_RxdLeng++] = RXBUF0;    //将收到的数据存入接收缓存
    System.Event.Bit.UART0BYTE = SET;           //置位接收标志
    SETTBMS(UART0RXDDELAY);                     //初始化定时器B
    TBCTL |= MC_1+TBCLR;                        //打开定时器B
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
    System.Event.Bit.UART1BYTE = SET;
    SETTBMS(UART1RXDDELAY); 
    TBCTL |= MC_1+TBCLR;        //打开定时器B
  }
  //LPM3_EXIT;
}     

int UART_printf(char uartx,const char *format, ...)
{
  //static const char hex[] = "0123456789ABCDEF";
  void (*WriteByte)(char value);        //发送1byte函数指针
  char format_flag;
  unsigned int u_val, div_val, base;
  char *ptr;
  va_list ap;
  
  if(0 == uartx)
  {
#ifdef  UART0_RS485_                    //定义总线为RS485
    UART0_DE;                           //发送状态 
#endif
    WriteByte = WriteByteUART0;
  }
  else if(1 == uartx)
  {
#ifdef  UART1_RS485_                    //定义总线为RS485
    UART1_DE;                           //发送状态 
#endif
    WriteByte = WriteByteUART1;
  }
  else                                  //发送端口参数错误返回
  {
    return 0;
  }

  va_start(ap, format);

  for (;;)    /* Until full format string read */
  {
    while ((format_flag = *format++) != '%')      /* Until '%' or '\0' */
    {
      if (!format_flag)
      {
        va_end(ap);
        EndWriteUART(uartx);
        return 0;
      }
      WriteByte(format_flag);
    }

    switch (format_flag = *format++)
    {
    case 'c':
      format_flag = va_arg(ap, int);
    default:
      WriteByte(format_flag);
      continue;

    case 's':
      ptr = VAPTR(char);
      while (format_flag = *ptr++)
      {
        WriteByte(format_flag);
      }
      continue;

    case 'o':
      base = 8;
      if (sizeof(int) == 2)
        div_val = 0x8000;
      else
        div_val = 0x40000000;
      goto CONVERSION_LOOP;

    case 'd':
      base = 10;
      if (sizeof(int) == 2)
        div_val = 10000;
      else
        div_val = 1000000000;
      goto CONVERSION_LOOP;

    case 'x':
      base = 16;
      if (sizeof(int) == 2)
        div_val = 0x1000;
      else
        div_val = 0x10000000;

CONVERSION_LOOP:
      u_val = va_arg(ap,int);
      if (format_flag == 'd')
      {
        if (((int)u_val) < 0)
        {
          u_val = - u_val;
          WriteByte('-');
        }
        while (div_val > 1 && div_val > u_val)
        {
          div_val /= 10;
        }
      }

      do
      {
        WriteByte(HEXASCII[u_val / div_val]);
        u_val %= div_val;
        div_val /= base;
      }
      while (div_val);
    }
  }
}


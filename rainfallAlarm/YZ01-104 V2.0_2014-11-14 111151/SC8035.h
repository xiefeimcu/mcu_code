/*********************************************************
*文件名称：SC8035.h
*摘    要：语音芯片驱动声明
*          
*作    者：赵阳
**********************************************************/

#ifndef 	__SC8035_H
#define 	__SC8035_H 

#include"System.h"
#include"Public.h"

#define SC_RST_OUT              P4DIR |= BIT2  //脉冲复位脚
#define SC_DATA_OUT             P4DIR |= BIT1
#define SC_RST_IN               P4DIR &= ~BIT2  //脉冲复位脚
#define SC_DATA_IN              P4DIR &= ~BIT1
#define SC_RST_1                P4OUT |= BIT2  //脉冲复位脚
#define SC_RST_0                P4OUT &= ~BIT2
#define SC_DATA_1               P4OUT |= BIT1
#define SC_DATA_0               P4OUT &= ~BIT1

void SC_Reset(void);
void SC_Speech(char cnt);

#endif 	/*#ifndef __SC8035_H*/
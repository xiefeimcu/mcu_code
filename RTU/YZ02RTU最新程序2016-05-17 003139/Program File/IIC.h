/***********************************************************************************/
//                       I2C.h With MSP430F149 Driver
//Platform: IAR Embedded Workbench Version: 3.42A
//Version: 1.0.0
//Author:  LiHuaiLiang
//Email:   li-huai-liang@163.com
//QQ:      769996244
//Date:    2009-03-14
//Modify:  
//====================================================================================
//                       I2C.h With MSP430F149 Driver
//Platform: IAR Embedded Workbench Version: 3.42A
//版本: 1.0.0
//作者:  李怀良
//Email:   li-huai-liang@163.com
//QQ:      769996244
//日期:    2009-03-14
//修改:
///***********************************************************************************/

#ifndef __IIC_H
#define __IIC_H
#include "MSP430_hardware.h"

#define IICSDA                    BIT0
#define IICSCL                    BIT2

#define IICSDAIn()                P1DIR &= ~IICSDA
#define IICSDAOut()               P1DIR |= IICSDA

#define IICSetSDA()               P1DIR |= IICSDA;P1OUT |= IICSDA
#define IICClrSDA()               P1DIR |= IICSDA;P1OUT &= ~IICSDA
#define IICReadSDA()              P1IN & IICSDA 

#define IICSetSCL()               P1OUT |= IICSCL;
#define IICClrSCL()               P1OUT &= ~IICSCL;

#define IICdelayus(x)             delayus(x)

///***********************************************************************************/
void WriteDelay(void);
void Delay(unsigned int i);
void IICStart(void);
void IICStop(void);
unsigned char IICReadAck(void);
void IICWriteAck(void);
void IICNoAck(void);
unsigned char IICWriteByte(unsigned char v);
unsigned char IICReadByteWithAck(void);
unsigned char IICReadByteWithoutAck(void);
///***********************************************************************************/

#endif /*#ifndef __IIC_H*/

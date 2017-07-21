#ifndef MSP430_HARDWARE_H_
#define MSP430_HARDWARE_H_

#include <msp430x14x.h>
#include <math.h> 
#include <stdlib.h>


#define CPU_F                   ((double)8000000) 
#define delayus(x)              __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delayms(x)              __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
#define RESETWDT                WDTCTL=WDT_ARST_1000                 //∏¥Œªø¥√≈π∑

#define SETTBMS(x)              TBCCR0=(x*32768)/1000


void InitSysCKL(void);
void Except(void);
void KeyInit(void);
void IniTimerA(void);
void TimerBInit();
void SetTimerB(unsigned int ti);
void GotimeDfB(char doit);

#endif  /*#ifndef MSP430_HARDWARE_H_*/
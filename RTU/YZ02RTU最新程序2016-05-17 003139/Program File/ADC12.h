#ifndef ADC12_H_
#define ADC12_H_

#include "MSP430_hardware.h"

typedef struct
{
  unsigned int A0;
  unsigned int A1;
  unsigned int A2;
  unsigned int A3;
  unsigned int A4;
  unsigned int A5;
  unsigned int A6;
  unsigned int A7;
  
}ADC8TClass;

int SuanSheShi(unsigned int adresult);
unsigned int GetVcc(void);
unsigned int GetADC12(unsigned char tongdao,unsigned int sref);
void ADCtest(void);
#endif /*#ifndef ADC12_H_*/
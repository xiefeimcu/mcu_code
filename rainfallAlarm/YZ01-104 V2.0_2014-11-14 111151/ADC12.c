#include "ADC12.h"
#include "include.h"


//ADC8TClass ADC8T={0};

unsigned int index = 0;
unsigned int results[32];            //保存ADC转换结果的数组

char ADCWait = 0;
unsigned int ADCMEMvalue=0;

/*读取电源电压*/
unsigned int GetVcc(void)
{
  unsigned long caltmp=0;
  unsigned int vcctemp=0;
  
  vcctemp = GetADC12(0,SREF_0);
  
  caltmp = vcctemp;
  caltmp = (caltmp << 5) + vcctemp;        //caltmp = vccad * 33
  caltmp = caltmp*100;                     //caltmp = caltmp * 100
  vcctemp = caltmp >> 12;                  //ADCValue = caltmp / 2^n    
  
  caltmp = vcctemp*2u;
  caltmp /= 10;
  
  return (unsigned int)caltmp;
}

/*计算摄氏温度*/
int SuanSheShi(unsigned int adresult)
{
	return (int)(((((double)adresult-1615)*704)/4095)*10);
}

/****************************************************************************
设置转换模式
adr：转换的首地址。取值范围为0~15
mod：转换模式。 0：单通道单次  1：序列通道单次  2：单通道多次 3：序列通道多次
****************************************************************************/
void AdcDo(unsigned int adr,char mod)
{
  ADC12CTL1 = (adr<<12)+SHP+(mod<<1);   //SHP意思为由采样定时器控制采样
}


//tongdao--采样通道，sref参考电压选择
void ST_ADC12(char tongdao,unsigned int sref)
{
  char i=0;
  ADC12CTL0 &= ~ENC;
  ADC12MEM0 = 0;
  index = 0;
  for(i=0;i<32;i++)
  {
    results[i] = 0;
  }
  
  ADC12CTL0 = ADC12ON + MSC+REFON+REF2_5V+SHT0_8+SHT1_8;

  ADC12CTL1 = SHP + CONSEQ_2;         //使用采样定时器,单通道多次转换
  ADC12MCTL0 = tongdao + sref;
  
  if(tongdao < 8)
  {
    P6SEL |= 0X01 << tongdao;
  } 

  ADC12IE = BIT0;
  
  ADCWait = 1;
  ADC12CTL0 |= ENC;                   //使能转换
  ADC12CTL0 |= ADC12SC;               //开始采样
}

void END_ADC12(void)
{                                               //关闭全局中断
  ADC12IE = 0;                                                               
  ADC12CTL0 &= ~ENC;                                                      
  ADC12CTL0 &= ~(ADC12SC+ADC12ON+REFON);   
  P6SEL = 0;    
  ADCWait = 0;                                 //采样标志复位  
} 

//tongdao--采样通道，sref参考电压选择
unsigned int GetADC12(char tongdao,unsigned int sref)
{
  unsigned int error=2500;
  
  ADCMEMvalue = 0;
  ADCWait = 0;
  ST_ADC12(tongdao,sref);

  do
  {
    delayus(100); 
    error--;
  }
  while(ADCWait && error);
  END_ADC12();                            //关闭AD转换
  
  return ADCMEMvalue;
}

/*******************************************
函数名称：ADC12ISR
功    能：ADC中断服务函数，在这里用多次平均的
          计算输入口的模拟电压数值
参    数：无       
返回值  ：无
********************************************/
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR (void)
{
   
    RESETWDT;
    results[index++] = ADC12MEM0;                    //Move results
    if(32 == index)                             //累积多次采样结果,算出平均值
    {
      char i;
      unsigned long sum = 0;
      index = 0;
      for(i = 0; i < 32; i++)
      {
          sum += results[i];
      }
      sum >>= 5;                                //除以32   
      ADCMEMvalue = sum;
      END_ADC12();
    }
    ADC12IFG = 0;
}


/*void ADCtest(void)
{
  ClearScreen;
   DisplayString(0,0,"A0",0);
   DisplayString(0,16,"A1",0);
   DisplayString(0,32,"A2",0);
   DisplayString(0,48,"A3",0);
   DisplayString(8,0,"A4",0);
   DisplayString(8,16,"A5",0);
   DisplayString(8,32,"A6",0);
   DisplayString(8,48,"A7",0);

   while(1)
   {
     
     DisplayNum(2,0,ADC8T.A0,0X50,space,"",0);
    DisplayNum(2,16,ADC8T.A1,0X50,space,"",0);
   DisplayNum(2,32,ADC8T.A2,0X50,space,"",0);
   DisplayNum(2,48,ADC8T.A3,0X50,space,"",0);
   DisplayNum(10,0,ADC8T.A4,0X50,space,"",0);
   DisplayNum(10,16,ADC8T.A5,0X50,space,"",0);
   DisplayNum(10,32,ADC8T.A6,0X50,space,"",0);
   DisplayNum(10,48,ADC8T.A7,0X50,space,"",0);
  
   ADC8T.A0 = GetADC12(0,SREF_0);
   ADC8T.A1 = GetADC12(1,SREF_0);
   ADC8T.A2 = GetADC12(2,SREF_0);
   ADC8T.A3 = GetADC12(3,SREF_0);
   ADC8T.A4 = GetADC12(4,SREF_0);
   ADC8T.A5 = GetADC12(5,SREF_0);
   ADC8T.A6 = GetADC12(6,SREF_0);
   ADC8T.A7 = GetADC12(7,SREF_0);
   delayms(500);
     
   } 
}*/

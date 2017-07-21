#include "include.h"

ClassLsData LsData={0};
ClassFlash Flash={1,2,0,0}; 

unsigned char SendLisdata(unsigned char *p);

void SaveLisdata(void)
{
  if(0 == FindFlash())    //Flash¼ì²âÊ§°Ü·µ»Ø
    return; 
  
  unsigned char array[10];
  unsigned char i;
  
  array[0] = 0XFF;
  array[1] = System.Time.Year;
  array[2] = System.Time.Month;
  array[3] = System.Time.Day;
  array[4] = System.Time.Hour;
  array[5] = (unsigned char)(LsData.P1 >> 8);
  array[6] = (unsigned char)(LsData.P1 & 0X00FF);
  array[7] = (unsigned char)(LsData.PT >> 8);
  array[8] = (unsigned char)(LsData.PT & 0X00FF);
  array[9] = 0XFF;
  
  SPI_Init();
  Save_data_to_Flash_inErase(1,Flash.Page,Flash.Bytes,(char *)array,10);
  SPI_End();
  
  Flash.Bytes += 10;
  if(Flash.Bytes > 260)
  {
    Flash.Bytes = 2;
    Flash.Page++;
    if(Flash.Page > 2046)
    {
      Flash.Page = 1;
    }
    WriteUInt16(Flash.Page,AT_PAGE,0);
  }
  WriteUInt16(Flash.Bytes,AT_BYTE,0);
   
  for(i=0;i<10;i++)
    array[i] = 0XFF;
  
  SPI_Init();
  Save_data_to_Flash_inErase(1,Flash.Page,Flash.Bytes,(char *)array,10);
  SPI_End();
}

void ReadLisdata(void)
{
  unsigned char array[264];
  unsigned int page,i;
  
  
  for(page=1;page<2047;page++)
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(page,2,(char *)array,260);
    SPI_End();
    for(i=0;i<255;i+=10)
    {
      if(0 == SendLisdata(&array[i]))
        return;
    }   
  }
}


unsigned char mychacou(unsigned char *s,unsigned char n,unsigned char c)
{
  unsigned char count=0;
  
  while(n--)
  {
    if(*s == c)
      count++;
    s++;
  }
  return count;
}

unsigned char SendLisdata(unsigned char *p)
{
  unsigned char str[40]={0};
  unsigned int num=0;
  
  if( 2 < mychacou(p,5,0XFF))
    return 0;
  
  str[0] = 0X0D;
  str[1] = 0X0A;
  str[2] = HEXASCII[p[1]/10];
  str[3] = HEXASCII[p[1]%10];
  str[4] = '-';
  str[5] = HEXASCII[p[2]/10];
  str[6] = HEXASCII[p[2]%10];
  str[7] = '-';
  str[8] = HEXASCII[p[3]/10];
  str[9] = HEXASCII[p[3]%10];
  str[10] = ' ';
  str[11] = HEXASCII[p[4]/10];
  str[12] = HEXASCII[p[4]%10];
  str[13] = ':';
  str[14] = '0';
  str[15] = '0';
  str[16] = ' ';
  str[17] = 'P';
  str[18] = '1';
  str[19] = ' ';
  num = p[5];
  num <<= 8;
  num += p[6];
  str[20] = HEXASCII[num/10000];
  num %= 10000;
  str[21] = HEXASCII[num/1000];
  num %= 1000;
  str[22] = HEXASCII[num/100];
  num %= 100;
  str[23] = HEXASCII[num/10];
  str[24] = '.';
  str[25] = HEXASCII[num %= 10];
  str[26] = ' ';
  str[27] = 'P';
  str[28] = 'T';
  str[29] = ' ';
  num = p[7];
  num <<= 8;
  num += p[8];
  str[30] = HEXASCII[num/10000];
  num %= 10000;
  str[31] = HEXASCII[num/1000];
  num %= 1000;
  str[32] = HEXASCII[num/100];
  num %= 100;
  str[33] = HEXASCII[num/10];
  str[34] = '.';
  str[35] = HEXASCII[num %= 10];
  
  WriteUART(0,(char *)str,36);
  
  return 1;
  
}

void sendtest(void)
{
  unsigned char array[10];
  
  array[0] = 0XFF;
  array[1] = System.Time.Year;
  array[2] = System.Time.Month;
  array[3] = System.Time.Day;
  array[4] = System.Time.Hour;
  array[5] = (unsigned char)(LsData.P1 >> 8);
  array[6] = (unsigned char)(LsData.P1 & 0X00FF);
  array[7] = (unsigned char)(LsData.PT >> 8);
  array[8] = (unsigned char)(LsData.PT & 0X00FF);
  array[9] = 0XFF;
  SendLisdata(array);
}
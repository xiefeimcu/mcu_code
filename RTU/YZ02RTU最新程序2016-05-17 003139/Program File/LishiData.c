#include "include.h"

ClassLsData LsData={0};
ClassFlash Flash={0};

unsigned char DRP[40]={0};

void InitDRP(void)
{
  unsigned char i=0;
   
  DRP[0] = System.Time.Year;
  DRP[1] = System.Time.Month;
  DRP[2] = System.Time.Day;
  DRP[3] = System.Time.Hour;
  
  for(i=4;i<40 ;i++)
  {
    DRP[i] = 0XFF;
  }
  
}

void UpdateDRP(void)
{
  unsigned char n = System.Time.Minute;
  if(0 == n)
  {
    n = 60;
  }
  n /= 5;
  
  if(Rainfall.PN05 > 254)
  {
    DRP[n+3] = 254;
  }
  else
  {
    DRP[n+3] = (unsigned char)Rainfall.PN05;
  }
  n = (2*n)+14;
  DRP[n++] = (unsigned char)(Relvtive>>8);
  DRP[n] = (unsigned char)(Relvtive &0X00FF);
}

void Flashadd(void)
{
  Flash.Bytes += 40;
  if(Flash.Bytes >= 250)
  {
    Flash.Bytes = 12;
    Flash.Page++;
    if(Flash.Page > 2046)
    {
      Flash.Page = 1;
    }
    WriteUInt16(Flash.Page,EEPROM.Page);
  }
  WriteUInt16(Flash.Bytes,EEPROM.Bytes);
}

void SaveDRP(void)
{
  if(0 == FindFlash())    //Flash检测失败返回
    return; 
  SPI_Init();
  Save_data_to_Flash_inErase(1,Flash.Page,Flash.Bytes,DRP,40);
  SPI_End();
}




void flashtest(void)
{
  unsigned char array[264]={0};
  unsigned int i=0;
  unsigned char j=0;
  for(i=0;i<264;i++)
  {
    array[i]=j++;
  }
  SPI_Init(); 
  Save_data_to_Flash_inErase(1,1,0,array,264);
  SPI_End();
  delayms(1000);
  for(i=0;i<264;i++)
  {
    array[i]=0;
  }
  SPI_Init(); 
  AT45DB041B_MainMemoryPageRead(1,0,array,264);
  SPI_End();
  PrintHEXU0(array,264);
}

void WriteLishidata(void)
{
  unsigned char array[16]={0};
  
  array[0] = 0XFF;
  array[1] = Message.ATT.Year;
  array[2] = Message.ATT.Month;
  array[3] = Message.ATT.Day;
  array[4] = Message.ATT.Hour;
  array[5] = Message.ATT.Minute;
  array[6] = (unsigned char)(Rainfall.PT>>8);
  array[7] = (unsigned char)(Rainfall.PT&0X00FF);
  array[8] = (unsigned char)(Analog1.ADValue>>8);
  array[9] = (unsigned char)(Analog1.ADValue&0X00FF);
  array[10] = (unsigned char)(Analog2.ADValue>>8);
  array[11] = (unsigned char)(Analog2.ADValue&0X00FF);
  Setbyte4(array,12,RS485.Value);
  
  if(0==FindFlash())    //Flash检测失败返回
    return; 
  
  if(Flash.Page > 2046)
    return;
  
  SPI_Init();  
  Save_data_to_Flash_inErase(1,Flash.Page,Flash.Bytes,array,16);    //写入数据
  SPI_End();
  Flash.Bytes +=16;
  if(Flash.Bytes>=250)
  {
    Flash.Bytes = 0;
    Flash.Page++;
    if(Flash.Page>2046)
    {
      Flash.Page = 1;
    }
    WriteUInt16(Flash.Page,EEPROM.Page);
  }
  WriteUInt16(Flash.Bytes,EEPROM.Bytes);
#if DEBUG >= 3
  sendtime();
  WriteLineStrUART1("Save Data");
#endif
}

void ReadDataMode(void)
{
  unsigned char buffarray[265]={0};
  unsigned int page=1;
  unsigned int bytes=0;

  for(page=1;page<=Flash.Page;page++)
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(page,0,buffarray,264);
    SPI_End();
    for(bytes=0;bytes<250;)
    {
      if(1==SearchLishidata(&buffarray[bytes]))
      {
        if(timecmp(&LsData.TT,&Flash.Satrtime)>=0)
          goto L1;          
      }
      else
        return;       
      bytes += 16;
    }
  }
L1:  
  for(;page<=Flash.Page;page++) 
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(page,0,buffarray,264);
    SPI_End();
    for(;bytes<250;)
    {
      if(1==SearchLishidata(&buffarray[bytes]))
      {
        if(timecmp(&LsData.TT,&Flash.Endtime)==1)
          return;                 
        SendMessenge();       
      }
      else
        return;        
      bytes += 16;
    }
    bytes = 0;
  }
  
}

void ReadallLishiData(void)
{
  unsigned char buffarray[265]={0};
  unsigned int page=1;
  unsigned int bytes=0;
  for(page=1;page<2048;page++)
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(page,0,buffarray,264);
    SPI_End();
    for(bytes=0;bytes<250;)
    {
      if(SearchLishidata(&buffarray[bytes])!=-1)
      {
        Writelishidatatext();
      }
      else
        return;
      
      bytes += 16;
    }
  } 
}

void ReadFlashHEX(void)
{
  unsigned char buffarray[265]={0};
  unsigned char srt[10]={'P','a','g','e',':',' ',' ',' ',' ',0};
  unsigned int page=1;

  for(page=1;page<5;page++)
  {
    SPI_Init();
    AT45DB041B_MainMemoryPageRead(page,0,buffarray,264);
    SPI_End();
    srt[5] = HEXASCII[(page/1000)%10];
    srt[6] = HEXASCII[(page/100)%10];
    srt[7] = HEXASCII[(page/10)%10];
    srt[8] = HEXASCII[page%10];
    WriteLineStrUART0(srt);
    PrintHEXU0(buffarray,264);
  } 
}

int SearchLishidata(unsigned char *p)
{
  
  if((0XFF==p[1])&&(0XFF==p[2]))
    return -1;
  
  LsData.TT.Year = p[1];
  LsData.TT.Month = p[2];
  LsData.TT.Day = p[3];
  LsData.TT.Hour = p[4];
  LsData.TT.Minute = p[5];
  LsData.PT = p[6];
  LsData.PT <<= 8;
  LsData.PT += p[7];
  LsData.A1 = p[8];
  LsData.A1 <<= 8;
  LsData.A1 += p[9];
  LsData.A2 = p[10];
  LsData.A2 <<= 8;
  LsData.A2 += p[11];
  LsData.Rs485 = Getbyte4(p,12);
  
  if(LsData.TT.Year>99||LsData.TT.Hour>23||LsData.TT.Minute>59)
    return 0;
  if(LsData.TT.Month>12||LsData.TT.Month<1)
    return 0;
  if(LsData.TT.Day>31||LsData.TT.Day<1)
    return 0;
  
  return 1;
}

unsigned char Writelishidatatext(void)
{
  unsigned char i=0;
  unsigned char strarray[80]={0};
  E_data element={0};                   
  
  strarray[i++] = SOH;
  strarray[i++] = ' ';
  strarray[i++] = HEXASCII[RTU.ST[0] >> 4];             //遥测站地址变量格式为BCD
  strarray[i++] = HEXASCII[RTU.ST[0] & 0X0F]; 
  strarray[i++] = HEXASCII[RTU.ST[1] >> 4];     
  strarray[i++] = HEXASCII[RTU.ST[1] & 0X0F]; 
  strarray[i++] = HEXASCII[RTU.ST[2] >> 4];     
  strarray[i++] = HEXASCII[RTU.ST[2] & 0X0F]; 
  strarray[i++] = HEXASCII[RTU.ST[3] >> 4];     
  strarray[i++] = HEXASCII[RTU.ST[3] & 0X0F]; 
  strarray[i++] = HEXASCII[RTU.ST[4] >> 4];     
  strarray[i++] = HEXASCII[RTU.ST[4] & 0X0F];  
  strarray[i++] = ' ';
  strarray[i++] = 'T';                                  //观测时间标识符                               
  strarray[i++] = 'T';    
  strarray[i++] = ' ';  
  strarray[i++] = HEXASCII[LsData.TT.Year / 10];        //年 十进制转换为ASCⅡ
  strarray[i++] = HEXASCII[LsData.TT.Year % 10];
  strarray[i++] = HEXASCII[LsData.TT.Month / 10];
  strarray[i++] = HEXASCII[LsData.TT.Month % 10];
  strarray[i++] = HEXASCII[LsData.TT.Day / 10];
  strarray[i++] = HEXASCII[LsData.TT.Day % 10];
  strarray[i++] = HEXASCII[LsData.TT.Hour / 10];
  strarray[i++] = HEXASCII[LsData.TT.Hour % 10];
  strarray[i++] = HEXASCII[LsData.TT.Minute / 10];
  strarray[i++] = HEXASCII[LsData.TT.Minute % 10];
  
  element.keyword = "PT";
  element.value = LsData.PT;
  element.DType = 0X51;
  i = WriteMessageVariable(strarray,i,&element);
  
  if(Analog1.Name != 0)
  {
    element.DType = YaoSuInfo[Analog1.Name].DataType;
    element.keyword = YaoSuInfo[Analog1.Name].Bzf;
    element.value = AnalogCalc(Analog1.SType,LsData.A1,Analog1.Max,Analog1.Min);
    i = WriteMessageVariable(strarray,i,&element);
  }
  if(Analog2.Name != 0)
  {
    element.DType = YaoSuInfo[Analog2.Name].DataType;
    element.keyword = YaoSuInfo[Analog2.Name].Bzf;
    element.value = AnalogCalc(Analog2.SType,LsData.A2,Analog2.Max,Analog2.Min);
    i = WriteMessageVariable(strarray,i,&element);
  }
  if(RS485.Name != 0)
  {
    element.DType = YaoSuInfo[RS485.Name].DataType;
    element.keyword = YaoSuInfo[RS485.Name].Bzf;
    element.value = LsData.Rs485;
    i = WriteMessageVariable(strarray,i,&element);
  }
  strarray[i++] = ETX;
  strarray[i++] = 0X0D;
  strarray[i++] = 0X0A;
  WriteUART0(strarray,i);
  return i;
}




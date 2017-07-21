#include "include.h"
#include "ATConfig.h"

ClassATcmdBasic ATcmdBasic={0};
const ClassATcmd ATcmdarray[MAX_CMD_AT]={
{"CCLK",ATcmd_CCLK},
{"ATE",ATcmd_ATE},
{"RST",ATcmd_RST},
{"YLFBL",ATcmd_YLFBL},
{"YLJBFZ",ATcmd_YLJBFZ},
{"YLJBSD",ATcmd_YLJBSD},
{"PDTT",ATcmd_PDTime},
{"YLJLX",ATcmd_RainType},
{"CRES",ATcmd_RstConfig},
{"TEST",ATcmd_TEST},
{"FLASHHEX",ATcmd_FlashHex},
{"FLASHASC",ATcmd_FlashASC},
{"RFDA",ATcmd_FlashSendTest},
{NULL,NULL}
};

unsigned char ATcmdif(unsigned char *pdata)
{
  unsigned char i=0;
  
  if(UART0_RxdLeng < 7)
    return 0;
 
  if(('Y'==pdata[0])&&('Z'==pdata[1])&&('+'==pdata[2]))
  {    
    pdata += 3;
    ATcmdBasic.attrib = 0;
    ATcmdBasic.val_ptr = NULL;
    for(i=0;i<MAX_CMD_LENGTH;i++)
    {
      ATcmdBasic.name[i] = 0;
    }
    for(i=0;i<MAX_CMD_LENGTH;i++)
    {         
      if((0X0D==pdata[i])&&((0X0A==pdata[i+1])))//Ö´ÐÐÃüÁî
      {
        ATcmdBasic.attrib = CMD_ARRTIB_RUN;
      }
      else if(('='==pdata[i])&&('?'==pdata[i+1])&&(0X0D==pdata[i+2]))//²âÊÔÃüÁî
      {
        ATcmdBasic.attrib = CMD_ARRTIB_TEST;
      }
      else if('='==pdata[i])                    //ÉèÖÃÃüÁî
      {
        ATcmdBasic.attrib = CMD_ARRTIB_SET;
        ATcmdBasic.val_ptr = &pdata[i+1];
      }
      else if(('?'==pdata[i])&&(0X0D==pdata[i+1]))//²éÑ¯ÃüÁî
      {
        ATcmdBasic.attrib = CMD_ARRTIB_CX;
      }
      if(ATcmdBasic.attrib != 0)
        return 1;
      
      if((pdata[i]<33)||(pdata[i]>126))          //·Ç·¨×Ö·û
        return 0;
      
      ATcmdBasic.name[i] = pdata[i];
    }                  
  }
  return 0;
}

void ATcmdconfig(void)
{
  unsigned char i=0;
  
  if(ATcmdBasic.ate)
  {
    WriteUART0(UART0_RxdBuff,UART0_RxdLeng-2);
    
  }
 
  for(i=0;i<MAX_CMD_AT;i++)
  {
    if(NULL==ATcmdarray[i].name)
    {
      WriteLineStrUART0("ERROR!\x0d\x0a");
      break;
    }
    
    if(NULL != Search(ATcmdBasic.name,strlenght(ATcmdBasic.name),ATcmdarray[i].name,strlenght(ATcmdarray[i].name)))
    {
      if(ATcmdarray[i].Subs())
      {
        WriteLineStrUART0("OK!\x0d\x0a");
      }
      else
      {
        WriteLineStrUART0("ERROR!\x0d\x0a");
      }
      break;
    }
  }
  
  ATcmdBasic.attrib = 0;
  ATcmdBasic.val_ptr = NULL;
  for(i=0;i<MAX_CMD_LENGTH;i++)
  {
    ATcmdBasic.name[i] = 0;
  }
}

unsigned char ATcmd_CCLK(void)
{
  unsigned char timestr[20]={"2000/00/00 00:00:00"};
  
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    if(PCF8563Ini())
    {
      WriteLineStrUART0("CCLK OK!");
    }
    else
    {
      WriteLineStrUART0("CCLK NG!");
    }
  }
  else if(CMD_ARRTIB_CX==ATcmdBasic.attrib)//²éÑ¯ÃüÁî
  {
    ReadTime(&System.Time);
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
    WriteLineStrUART0("+CCLK:");
    WriteStrUART0(timestr);
  }
  else if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {
    DataTime val_time={0};
    unsigned char i=0;
    if(ATcmdBasic.val_ptr[0]==timestr[0])
    {
      for(i=1;i<17;)
      {
        if(ATcmdBasic.val_ptr[i]!=timestr[i])
          return 0;
        
        i += 3;
      }
      val_time.Year = ToAscii_U8(ATcmdBasic.val_ptr[2],ATcmdBasic.val_ptr[3]);
      val_time.Month = ToAscii_U8(ATcmdBasic.val_ptr[5],ATcmdBasic.val_ptr[6]);
      val_time.Day = ToAscii_U8(ATcmdBasic.val_ptr[8],ATcmdBasic.val_ptr[9]);
      val_time.Hour = ToAscii_U8(ATcmdBasic.val_ptr[11],ATcmdBasic.val_ptr[12]);
      val_time.Minute = ToAscii_U8(ATcmdBasic.val_ptr[14],ATcmdBasic.val_ptr[15]);
      val_time.Second = ToAscii_U8(ATcmdBasic.val_ptr[17],ATcmdBasic.val_ptr[18]);
      if(False == TimeCheck(val_time))   //Ê±¼äÊý¾Ý·Ç·¨
        return 0;
      System.Time = val_time;
      SetTime(&System.Time);
    }
    else
      return 0;
  }
  return 1;
}

unsigned char ATcmd_ATE(void)
{
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)
  {
    if('0'==ATcmdBasic.val_ptr[0])
    {
      ATcmdBasic.ate = RESET;
      return 1;
    }
    else if('1'==ATcmdBasic.val_ptr[0])
    {
      ATcmdBasic.ate = SET;
      return 1;
    }      
  }
  return 0;
}

unsigned char ATcmd_RST(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    WriteLineStrUART0("Reset...\x0d\x0a");
    delayms(5000);
    WDTCTL = 0;    
  }
  return 0;
}




unsigned char ATcmd_YLFBL(void)
{
  unsigned char x=0;
 
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {
    x = ATgetnum(ATcmdBasic.val_ptr,0X21);
    if(0==x)
      return 0; 
    
    Write_1Byte(x,EEPROM.RainFBL);

    return 1;
  }
  else if(CMD_ARRTIB_CX==ATcmdBasic.attrib)//²éÑ¯ÃüÁî
  {
    x = Read_1Byte_Randomaddress(EEPROM.RainFBL);
    ATreturnstringnum(x,0X21);
    return 1;
  }
    
  return 0;
}

unsigned char ATcmd_YLJBFZ(void)
{
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {
    WriteUInt16(ATgetnum(ATcmdBasic.val_ptr,0X51),EEPROM.RainJBFZ);
    return 1;
  }
  else if(CMD_ARRTIB_CX==ATcmdBasic.attrib)//²éÑ¯ÃüÁî
  {
    ATreturnstringnum(ReadUInt16(EEPROM.RainJBFZ),0X51);
    return 1;
  }
    
  return 0;
}

unsigned char ATcmd_YLJBSD(void)
{
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {  
   Write_1Byte(ATgetnum(ATcmdBasic.val_ptr,0X20),EEPROM.RainJBSD);
    return 1;
  }
  else if(CMD_ARRTIB_CX==ATcmdBasic.attrib)//²éÑ¯ÃüÁî
  {
    ATreturnstringnum(Read_1Byte_Randomaddress(EEPROM.RainJBSD),0X20);
    return 1;
  }
     
  return 0;
}

unsigned char ATcmd_PDTime(void)
{
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {  
   Write_1Byte(ATgetnum(ATcmdBasic.val_ptr,0X20),EEPROM.PDTime);
    return 1;
  }
  else if(CMD_ARRTIB_CX==ATcmdBasic.attrib)//²éÑ¯ÃüÁî
  {
    ATreturnstringnum(Read_1Byte_Randomaddress(EEPROM.PDTime),0X20);
    return 1;
  }
    
  return 0;
}

unsigned char ATcmd_RainType(void)
{
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {  
   Write_1Byte(ATgetnum(ATcmdBasic.val_ptr,0X10),EEPROM.RainType);
    return 1;
  }
  else if(CMD_ARRTIB_CX==ATcmdBasic.attrib)//²éÑ¯ÃüÁî
  {
    ATreturnstringnum(Read_1Byte_Randomaddress(EEPROM.RainType),0X10);
    return 1;
  }

  return 0;
}

unsigned char ATcmd_RstConfig(void)
{
  WriteLineStrUART0("µ¼ÈëÅäÖÃ>>>");
  DefaultConfig();
  RainfallClear();
  WriteConfig();
  return 1;
}

unsigned char ATcmd_TEST(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    if(PCF8563Ini())
      WriteLineStrUART0("CCLK OK!");
    else
      WriteLineStrUART0("CCLK NG!");
    
    if(AT24C04TEST(1))
      WriteLineStrUART0("EEPROM OK!");
    else
      WriteLineStrUART0("EEPROM NG!");
    
    if(FindFlash())
      WriteLineStrUART0("FALSH OK!");
    else
      WriteLineStrUART0("FALSH NG!");
    
  }
  return 1;
}

unsigned char ATcmd_FlashHex(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    ReadFlashHEX();
  }
  return 1;
}

unsigned char ATcmd_FlashASC(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    ReadallLishiData();
  }
  return 1;
}

unsigned char ATcmd_FlashSendTest(void)
{
  if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {
    Flash.Satrtime.Year = ToAscii_U8(ATcmdBasic.val_ptr[0],ATcmdBasic.val_ptr[1]);
    Flash.Satrtime.Month = ToAscii_U8(ATcmdBasic.val_ptr[2],ATcmdBasic.val_ptr[3]);
    Flash.Satrtime.Day = ToAscii_U8(ATcmdBasic.val_ptr[4],ATcmdBasic.val_ptr[5]);
    Flash.Satrtime.Hour = ToAscii_U8(ATcmdBasic.val_ptr[6],ATcmdBasic.val_ptr[7]);
    Flash.Satrtime.Minute = ToAscii_U8(ATcmdBasic.val_ptr[8],ATcmdBasic.val_ptr[9]);
    Flash.Endtime.Year = ToAscii_U8(ATcmdBasic.val_ptr[10],ATcmdBasic.val_ptr[11]);
    Flash.Endtime.Month = ToAscii_U8(ATcmdBasic.val_ptr[12],ATcmdBasic.val_ptr[13]);
    Flash.Endtime.Day = ToAscii_U8(ATcmdBasic.val_ptr[14],ATcmdBasic.val_ptr[15]);
    Flash.Endtime.Hour = ToAscii_U8(ATcmdBasic.val_ptr[16],ATcmdBasic.val_ptr[17]);
    Flash.Endtime.Minute = ToAscii_U8(ATcmdBasic.val_ptr[18],ATcmdBasic.val_ptr[19]);
    if(TimeCheck(Flash.Satrtime)&&TimeCheck(Flash.Endtime))
    {
      Message.AFN = AFN.ChaXunShiDuan;
      System.Event.Bit.Flash = SET;
      #if DEBUG >= 3
      WriteLineStrUART1("System.Event.Bit.Flash = SET;");
      #endif
    }   
    else
    {
      #if DEBUG >= 3
      WriteLineStrUART1("TimeCheck error");
      #endif
    }
  }
  else if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    Flash.Satrtime.Year = 1;
    Flash.Satrtime.Month = 1;
    Flash.Satrtime.Day = 1;
    Flash.Satrtime.Hour = 0;
    Flash.Satrtime.Minute = 0;
    Flash.Endtime.Year = 99;
    Flash.Endtime.Month = 12;
    Flash.Endtime.Day = 31;
    Flash.Endtime.Hour = 23;
    Flash.Endtime.Minute = 59;
    Message.AFN = AFN.ChaXunShiDuan;
    System.Event.Bit.Flash = SET;
    #if DEBUG >= 3
      WriteLineStrUART1("System.Event.Bit.Flash = SET;");
      #endif
  }
  return 1;
}


long int ATgetnum(unsigned char *p,unsigned char type)
{
  unsigned char i=0;
  
  for(i=0;i<10;i++)
  {
    if(0X0D==p[i])
      break;
  }
  if((i==0)||(i>=10))
    return 0;
  
  return AtoLong(p,i,type);
  
}

void ATreturnstringnum(long num,unsigned char type)
{
  unsigned char srtarr[15]={0};
  
  SInt32_TO_String(srtarr,num,type,0);
  WriteLineStrUART0("+");
  WriteStrUART0(ATcmdBasic.name);
  WriteStrUART0(":");
  WriteStrUART0(srtarr);
}




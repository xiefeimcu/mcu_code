#include "include.h"
#include "ATConfig.h"

ClassATcmdBasic ATcmdBasic={0};
const ClassATcmd ATcmdarray[MAX_CMD_AT]={
{"CCLK",ATcmd_CCLK},
{"ATE",ATcmd_ATE},
{"RST",ATcmd_RST},
{"CRES",ATcmd_RstConfig},
{"TEST",ATcmd_TEST},
{"LedTest",ATcmd_LedTest},
{"Read",ATcmd_Read},
{NULL,NULL}
};

char ATcmdif(char *pdata)
{
  char i=0;
  
  if(UART0_RxdLeng < 7)
    return 0;
 
  if(('Y'==pdata[0])&&('Z'==pdata[1])&&('+'==pdata[2]))
  { 
    Display.TiOFF = OFFLCDDELAY;  
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
  char i=0;
  
  if(ATcmdBasic.ate)
  {
    WriteUART(0,UART0_RxdBuff,UART0_RxdLeng-2);
  }
 
  for(i=0;i<MAX_CMD_AT;i++)
  {
    if(NULL==ATcmdarray[i].name)
    { 
      UART_printf(0,"\r\nERROR!");
      break;
    } 
    
    if(NULL != Search(ATcmdBasic.name,strlenght(ATcmdBasic.name),ATcmdarray[i].name,strlenght(ATcmdarray[i].name)))
    {
      if(ATcmdarray[i].Subs())
      {
        UART_printf(0,"\r\nOK!");
      }
      else
      {
        UART_printf(0,"\r\nERROR!");
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

char ATcmd_CCLK(void)
{
  char timestr[20]={"2000-00-00 00:00:00"};
  
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    if(PCF8563Ini())
    {
      UART_printf(0,"\r\nCCLK OK!");
    }
    else
    {
      UART_printf(0,"\r\nCCLK NG!");
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
    UART_printf(0,"\r\n+CCLK:");
    UART_printf(0,timestr);
  }
  else if(CMD_ARRTIB_SET==ATcmdBasic.attrib)//ÉèÖÃÃüÁî
  {
    DataTime val_time={0};
    char i=0;
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

char ATcmd_ATE(void)
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

char ATcmd_RST(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    UART_printf(0,"Reset...\x0d\x0a");
    delayms(5000);
    WDTCTL = 0;    
  }
  return 0;
}

char ATcmd_RstConfig(void)
{
  unsigned char array[264]={0XFF};
  unsigned int i;

  UART_printf(0,"\r\nµ¼ÈëÅäÖÃ>>>");
  ResetFactory();
  UART_printf(0,"\r\n³õÊ¼»¯¹ÌÌ¬´æ´¢Æ÷>>>");
  for(i=0;i<264;i++)
  {
    array[i] = 0XFF;
  }
  SPI_Init();
  for(i=0;i<2048;i++)
  {  
    Save_data_to_Flash_inErase(1,i,0,(char *)array,264);
  }
  SPI_End();
  return 1;
}
char ATcmd_TEST(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    if(PCF8563Ini())
      UART_printf(0,"\r\nCCLK OK!");
    else
      UART_printf(0,"\r\nCCLK NG!");
    
    if(AT24C04TEST(1))
      UART_printf(0,"\r\nEEPROM OK!");
    else
      UART_printf(0,"\r\nEEPROM NG!");
    
    if(FindFlash())
      UART_printf(0,"\r\nFALSH OK!");
    else
      UART_printf(0,"\r\nFALSH NG!");
    
  }
  return 1;
}



long int ATgetnum(char *p,char type)
{
  char i=0;
  
  for(i=0;i<10;i++)
  {
    if(0X0D==p[i])
      break;
  }
  if((i==0)||(i>=10))
    return 0;
  
  return AtoLong(p,i,type);
  
}

void ATreturnstringnum(long num,char type)
{
  char srtarr[15]={0};
  
  SInt32_TO_String(srtarr,num,type,0);
  UART_printf(0,"\r\n+");
  WriteStrUART(0,ATcmdBasic.name);
  UART_printf(0,"\r\n:");
  WriteStrUART(0,srtarr);
}

char ATcmd_LedTest(void)
{
    char i=0;
    for(i=1;i<4;i++)
    {
      LEDINI;
      if(i==1)
      {
        LED_1_ON;
      }
      else if(i==2)
      {
        LED_2_ON;
      }
      else if(i==3)
      {
        LED_3_ON;
      }
      SC_Speech(i);
      delayms(5000);
    }
    LEDINI;
        
  return 1;
}

char ATcmd_Read(void)
{
  if(CMD_ARRTIB_RUN==ATcmdBasic.attrib)//Ö´ÐÐÃüÁî
  {
    UART_printf(0,"\r\nRead...\x0d\x0a");
    ReadLisdata(); 
  }
  return 1;
}


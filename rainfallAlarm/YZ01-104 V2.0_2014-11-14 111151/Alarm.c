#include "include.h"
#include "Alarm.h"

AlarmClass Alarm={0};
AlarmLevelClass AlarmLevel={0};
AlarmModeClass AlarmMode={0};

unsigned int AlarmValueif(AlarmValue *p,unsigned int value)
{
  if(p->I && (value >= p->I))
    return 0XFFFF;
  if(p->II && (value >= p->II))
    return 0XFFFF;
  if(p->III && (value >= p->III))
    return 0XFFFF;
  
  return 0;
}

void AlarmLevelHandle(char x)
{
  AlarmLevel.Qlevel = AlarmLevel.Level;
#if DEBUG
  WriteLineStrUSART0("AlarmLevel.Qlevel=");
  SendNum(AlarmLevel.Qlevel,0X20,0,"",0);
#endif
  if(Alarm.PT.I && (Rainfall.PT >= Alarm.PT.I))
  {
    AlarmLevel.Rain = SD_PT;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.P.I && (Rainfall.P >= Alarm.P.I))
  {
    AlarmLevel.Rain = SD_P;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.PJ.I && (Rainfall.PJ >= Alarm.PJ.I))
  {
    AlarmLevel.Rain = SD_PJ;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.P24.I && (Rainfall.P24 >= Alarm.P24.I))
  {
    AlarmLevel.Rain = SD_P24;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.P12.I && (Rainfall.P12 >= Alarm.P12.I))
  {
    AlarmLevel.Rain = SD_P12;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.P6.I && (Rainfall.P6 >= Alarm.P6.I))
  {
    AlarmLevel.Rain = SD_P6;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.P3.I && (Rainfall.P3 >= Alarm.P3.I))
  {
    AlarmLevel.Rain = SD_P3;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.P1.I && (Rainfall.P1 >= Alarm.P1.I))
  {
    AlarmLevel.Rain = SD_P1;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.PN30.I && (Rainfall.PN30 >= Alarm.PN30.I))
  {
    AlarmLevel.Rain = SD_PN30;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.PN10.I && (Rainfall.PN10 >= Alarm.PN10.I))
  {
    AlarmLevel.Rain = SD_PN10;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.PN05.I && (Rainfall.PN05 >= Alarm.PN05.I))
  {
    AlarmLevel.Rain = SD_PN05;
    AlarmLevel.Level = LI;
  }
  else if(Alarm.PT.II && (Rainfall.PT >= Alarm.PT.II))
  {
    AlarmLevel.Rain = SD_PT;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.P.II && (Rainfall.P >= Alarm.P.II))
  {
    AlarmLevel.Rain = SD_P;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.PJ.II &&(Rainfall.PJ >= Alarm.PJ.II))
  {
    AlarmLevel.Rain = SD_PJ;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.P24.II && (Rainfall.P24 >= Alarm.P24.II))
  {
    AlarmLevel.Rain = SD_P24;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.P12.II && (Rainfall.P12 >= Alarm.P12.II))
  {
    AlarmLevel.Rain = SD_P12;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.P6.II && (Rainfall.P6 >= Alarm.P6.II))
  {
    AlarmLevel.Rain = SD_P6;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.P3.II && (Rainfall.P3 >= Alarm.P3.II))
  {
    AlarmLevel.Rain = SD_P3;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.P1.II && (Rainfall.P1 >= Alarm.P1.II))
  {
    AlarmLevel.Rain = SD_P1;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.PN30.II && (Rainfall.PN30 >= Alarm.PN30.II))
  {
    AlarmLevel.Rain = SD_PN30;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.PN10.II && (Rainfall.PN10 >= Alarm.PN10.II))
  {
    AlarmLevel.Rain = SD_PN10;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.PN05.II && (Rainfall.PN05 >= Alarm.PN05.II))
  {
    AlarmLevel.Rain = SD_PN05;
    AlarmLevel.Level = LII;
  }
  else if(Alarm.PT.III && (Rainfall.PT >= Alarm.PT.III))
  {
    AlarmLevel.Rain = SD_PT;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.P.III && (Rainfall.P >= Alarm.P.III))
  {
    AlarmLevel.Rain = SD_P;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.PJ.III && (Rainfall.PJ >= Alarm.PJ.III))
  {
    AlarmLevel.Rain = SD_PJ;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.P24.III && (Rainfall.P24 >= Alarm.P24.III))
  {
    AlarmLevel.Rain = SD_P24;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.P12.III && (Rainfall.P12 >= Alarm.P12.III))
  {
    AlarmLevel.Rain = SD_P12;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.P6.III && (Rainfall.P6 >= Alarm.P6.III))
  {
    AlarmLevel.Rain = SD_P6;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.P3.III && (Rainfall.P3 >= Alarm.P3.III))
  {
    AlarmLevel.Rain = SD_P3;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.P1.III && (Rainfall.P1 >= Alarm.P1.III))
  {
    AlarmLevel.Rain = SD_P1;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.PN30.III && (Rainfall.PN30 >= Alarm.PN30.III))
  {
    AlarmLevel.Rain = SD_PN30;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.PN10.III && (Rainfall.PN10 >= Alarm.PN10.III))
  {
    AlarmLevel.Rain = SD_PN10;
    AlarmLevel.Level = LIII;
  }
  else if(Alarm.PN05.III && (Rainfall.PN05 >= Alarm.PN05.III))
  {
    AlarmLevel.Rain = SD_PN05;
    AlarmLevel.Level = LIII;
  }
#if DEBUG
  WriteLineStrUSART0("AlarmLevel.Rain=");
  SendNum(AlarmLevel.Rain,0X20,0,"",0);
  WriteLineStrUSART0("AlarmLevel.Level=");
  SendNum(AlarmLevel.Level,0X20,0,"",0);
#endif
  if(x && (AlarmLevel.Level>AlarmLevel.Qlevel))
  {   
    #if DEBUG
    WriteLineStrUSART0("OpenAlarm()");
    #endif
    OpenAlarm();
  }
}

void OpenAlarm(void)
{ 
  AlarmMode.Status.Flag.YuYing = True;
  AlarmMode.Status.Flag.Stop = False;
  AlarmMode.Status.Flag.Run = True;
  AlarmMode.BofangCount = 1;
  AlarmMode.Time = 0;
  AlarmMode.level = AlarmLevel.Level;
  LEDalarm();
  AlarmSpeech();
  if(Display.Status.Flag.Mon)
  {
    DisplayMon();
  }  
}
    
void StopAlarm(void)
{
  AlarmMode.Status.Flag.YuYing = False;
  AlarmMode.Status.Flag.Stop = False;
  AlarmMode.Status.Flag.Run = False;
  AlarmMode.BofangCount = 1;
  AlarmMode.Time = 0;
  AlarmMode.level = 0;
  LEDINI;  
  SC_Reset();
  if(Display.Status.Flag.Mon)
  {
    DisplayMon();
  }  
  JDQOFF;
}

void LEDalarm(void)
{
  LEDINI;
  if(LI==AlarmMode.level)
  {
    LED_3_ON;
  }
  else if(LII==AlarmMode.level)
  {
    LED_2_ON;
  }
  else if(LIII==AlarmMode.level)
  {
    LED_1_ON;
  } 
}
  
void AlarmSpeech(void)
{
  char i=0;
  
  System.Voltage = GetVcc();
  if(System.Voltage < OFFVCC)
    return;
  
  i = AlarmMode.BofangCount % 3;
  
  SC_Reset();
  if(1==i)
  {
    SpeechYuying();
  }
  else 
  {
    SpeechMingdi();
  }
  AlarmMode.Time = 6;
  i = AlarmMode.BofangCount % 9;
  if(0==i)
  {
    AlarmMode.Time = 60;
  }
  if(AlarmMode.BofangCount>=90)
  {
    AlarmMode.Status.Flag.YuYing = False;
    AlarmMode.Status.Flag.Stop = True;
  }
  AlarmMode.BofangCount++;
}

void SpeechYuying(void)
{
  if(LI==AlarmMode.level)
  {
    SC_Speech(YYS_I);
  }
  else if(LII==AlarmMode.level)
  {
    SC_Speech(YYS_II);
  }
  else if(LIII==AlarmMode.level)
  {
    SC_Speech(YYS_III);
  } 
  else 
  {
    SC_Reset();
  }
}

void SpeechMingdi(void)
{
  if(LI==AlarmMode.level)
  {
    SC_Speech(MDS_I);
  }
  else if(LII==AlarmMode.level)
  {
    SC_Speech(MDS_II);
  }
  else if(LIII==AlarmMode.level)
  {
    SC_Speech(MDS_III);
  } 
  else
  {
    SC_Reset();
  }
}





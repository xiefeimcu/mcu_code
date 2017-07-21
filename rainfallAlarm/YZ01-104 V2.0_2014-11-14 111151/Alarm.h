#ifndef __ALARM_
#define __ALARM_

#define SD_PN05          1
#define SD_PN10          2
#define SD_PN30          3
#define SD_P1            4
#define SD_P3            5
#define SD_P6            6
#define SD_P12           7
#define SD_P24           8
#define SD_PJ            9
#define SD_P             10
#define SD_PT            11
#define LI                3
#define LII               2
#define LIII              1

#define YYS_I             3
#define YYS_II            2
#define YYS_III           1
#define MDS_I             6
#define MDS_II            5
#define MDS_III           4

#define OFFVCC            1

#define LEDINI          P6DIR |= BIT4+BIT7;P6OUT |= BIT4+BIT7;P1DIR |= BIT7;P1OUT |= BIT7;
#define LED_1_ON        P6OUT &= ~BIT4
#define LED_2_ON        P6OUT &= ~BIT7
#define LED_3_ON        P1OUT &= ~BIT7
#define LED_1_OFF       P6OUT |= BIT4
#define LED_2_OFF       P6OUT |= BIT7
#define LED_3_OFF       P1OUT |= BIT7
#define JDQON           P4DIR |= BIT0;_NOP();P4OUT |= BIT0;     //打开继电器2
#define JDQOFF          P4DIR |= BIT0;_NOP();P4OUT &= ~BIT0;    //关闭继电器2

typedef struct
{
  unsigned int I;
  unsigned int II;
  unsigned int III;
}AlarmValue;

typedef struct 
{
  AlarmValue PT;         
  AlarmValue PN05;            
  AlarmValue PN10;           
  AlarmValue PN30;           
  AlarmValue P1;
  AlarmValue P3;
  AlarmValue P6;
  AlarmValue P12;
  AlarmValue P24;
  AlarmValue PJ;            
  AlarmValue P;
}AlarmClass;

typedef struct
{
  char Qlevel;
  char Rain;
  char Level;
}AlarmLevelClass;


typedef struct
{
  union
  {
    char value;
    struct 
    {
      char YuYing  :1;
      char Stop    :1;   
      char Run     :1;
    }Flag;
  }Status;
  char Time;
  char BofangCount;
  char level;
}AlarmModeClass;

extern AlarmClass Alarm;
extern AlarmLevelClass AlarmLevel;
extern AlarmModeClass AlarmMode;

unsigned int AlarmValueif(AlarmValue *p,unsigned int value);
void AlarmLevelHandle(char x);
void SpeechYuying(void);
void SpeechMingdi(void);
void AlarmSpeech(void);
void OpenAlarm(void);
void LEDalarm(void);
void StopAlarm(void);
#endif  /*#ifndef __ALARM_*/
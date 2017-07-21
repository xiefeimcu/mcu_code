#ifndef ATCONFIG_H_
#define ATCONFIG_H_

#define MAX_CMD_LENGTH          10              //命令名称最大值
#define MAX_CMD_AT              20
#define CMD_ARRTIB_SET          1
#define CMD_ARRTIB_TEST         2
#define CMD_ARRTIB_CX           3
#define CMD_ARRTIB_RUN          4
typedef struct
{
  char name[MAX_CMD_LENGTH];   //名称
  char attrib;                 //属性
  char *val_ptr;               //参数指针
  char ate;                    //回显功能开关
}ClassATcmdBasic;

typedef struct
{
  char *name;
  char (*Subs)();
}ClassATcmd;


extern ClassATcmdBasic ATcmdBasic;

char ATcmdif(char *pdata);
void ATcmdconfig(void);
char ATcmd_CCLK(void);
char ATcmd_ATE(void);
char ATcmd_RST(void);
char ATcmd_YLFBL(void);
char ATcmd_YLFBL(void);
char ATcmd_YLJBFZ(void);
char ATcmd_YLJBSD(void);
char ATcmd_PDTime(void);
char ATcmd_RainType(void);
char ATcmd_RstConfig(void);
char ATcmd_TEST(void);
long int ATgetnum(char *p,char type);
void ATreturnstringnum(long num,char type);
char ATcmd_FlashASC(void);
char ATcmd_FlashHex(void);
char ATcmd_FlashSendTest(void);
char ATcmd_LedTest(void);
char ATcmd_Read(void);
#endif
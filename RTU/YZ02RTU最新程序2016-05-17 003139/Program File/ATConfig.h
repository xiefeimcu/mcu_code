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
  unsigned char name[MAX_CMD_LENGTH];   //名称
  unsigned char attrib;                 //属性
  unsigned char *val_ptr;               //参数指针
  unsigned char ate;                    //回显功能开关
}ClassATcmdBasic;

typedef struct
{
  unsigned char *name;
  unsigned char (*Subs)();
}ClassATcmd;


extern ClassATcmdBasic ATcmdBasic;

unsigned char ATcmdif(unsigned char *pdata);
void ATcmdconfig(void);
unsigned char ATcmd_CCLK(void);
unsigned char ATcmd_ATE(void);
unsigned char ATcmd_RST(void);
unsigned char ATcmd_YLFBL(void);
unsigned char ATcmd_YLFBL(void);
unsigned char ATcmd_YLJBFZ(void);
unsigned char ATcmd_YLJBSD(void);
unsigned char ATcmd_PDTime(void);
unsigned char ATcmd_RainType(void);
unsigned char ATcmd_RstConfig(void);
unsigned char ATcmd_TEST(void);
long int ATgetnum(unsigned char *p,unsigned char type);
void ATreturnstringnum(long num,unsigned char type);
unsigned char ATcmd_FlashASC(void);
unsigned char ATcmd_FlashHex(void);
unsigned char ATcmd_FlashSendTest(void);
#endif
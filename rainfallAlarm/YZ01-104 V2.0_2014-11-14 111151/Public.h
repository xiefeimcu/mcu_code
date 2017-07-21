
/*********************************************************
*文件名称：Public.h
*摘    要：与硬件无关的常用功能函数
*
*作    者：赵阳 
*开始日期：2013年4月16日
*完成日期：
**********************************************************/

#ifndef __PUBLIC_H
#define __PUBLIC_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "stdarg.h"
#include "sysmac.h"

#define         uchar           char
#define         uint            unsigned int
#define         True            1                       //真
#define         False           0                       //假
#define         Null            0                       //空值

/*字符串转换为数字格式控制控制*/
#define Zero                    (0x0001u)                                    //保留前导零
#define Symbol                  (0x0002u)                                    //正数前加入'+'
#define space                   (0x0004u)                                    //前导零替换为空格
#define DEZero                  (0x0008u)                                    //删除尾部的零                                  


/*日期时间数据结构*/
typedef struct
{
  char Year;
  char Month;
  char Day;
  char Hour;
  char Minute;
  char Second; 
}DataTime;

/*无符号16bit整数结构体，不用计算直接得到高8bit与低8bit*/
typedef union 
{
  unsigned int num;
  struct 
  {
    char nLow;
    char nHigh;
  }bit16;
}Uint16;

/*有符号32bit整数结构体，不用计算直接得到每个8bit内容*/
typedef union
{
  signed long num;
  struct
  {
    Uint16 nLow;
    Uint16 nHigh;
  }bit32;
}Slong32;

/*IEEE754浮点数*/
typedef union
{
  float num;
  struct
  {
    char a0;
    char a1;
    char a2;
    char a3;
  }bit;
}float_n;

typedef union
{
  unsigned int i;
  struct
  {
    char L;
    char H;
  }CharPart;
}UnionInt;

typedef union
{
  long l;
  struct
  {
    unsigned int L;
    unsigned int H;
  }IntPart;
  struct
  {
    char LL;
    char LH;
    char HL;
    char HH;
  }CharPart;
}UnionLong;
 
extern const char HEXASCII[16];

unsigned int strlenght(char *s);
char Byte_BCD(char byte);
char BCD_Byte(char bcd);
signed char ASCII_Hex(char asc2);
char ToAscii_Hex(char asc1,char asc2);
char ToAscii_U8(char asc1,char asc2);
unsigned int crc(char *crc_pointer,unsigned int data_length);
unsigned int CRC16(char *puchMsg, unsigned int usDataLen);
char SInt32_TO_String(char *p,long int value,char type,char cmd);
long int String_TO_SInt32(char *str);
char TimeCheck(DataTime time);
long int AtoLong(char *p,char length,char type);
char Timedaymax(DataTime time);
long atofxy(char *str,char type);
char* Search(char* base,char nmemb,char* key,char len);
int timecmp(DataTime *t0,DataTime *t1);
void reverse(char* begin, char* end);
void itoa(int value, char* str, int base);
void ltoax(long value, char* str, int base) ;
void ltoa(long int n,char *str);
unsigned int Getbyte2(char *pa,char add);
long Getbyte4(char *pa,char add);
void Setbyte2(char *pa,char add,unsigned int value);
void Setbyte4(char *pa,char add,long value);


#endif  /*#ifndef __PUBLIC_H*/

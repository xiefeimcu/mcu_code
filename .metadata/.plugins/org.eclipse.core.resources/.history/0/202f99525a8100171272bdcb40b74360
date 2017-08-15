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

#define         uchar           unsigned char
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
  unsigned char Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Minute;
  unsigned char Second; 
}DataTime;

/*无符号16bit整数结构体，不用计算直接得到高8bit与低8bit*/
typedef union 
{
  unsigned int num;
  struct 
  {
    unsigned char nLow;
    unsigned char nHigh;
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
    unsigned char a0;
    unsigned char a1;
    unsigned char a2;
    unsigned char a3;
  }bit;
}float_n;
 
extern const unsigned char HEXASCII[16];

unsigned int strlenght(unsigned char *s);
unsigned char Byte_BCD(unsigned char byte);
unsigned char BCD_Byte(unsigned char bcd);
signed char ASCII_Hex(unsigned char asc2);
unsigned char ToAscii_Hex(unsigned char asc1,unsigned char asc2);
unsigned char ToAscii_U8(unsigned char asc1,unsigned char asc2);
unsigned int crc(unsigned char *crc_pointer,unsigned int data_length);
unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen);
unsigned char SInt32_TO_String(unsigned char *p,long int value,unsigned char type,unsigned char cmd);
long int String_TO_SInt32(unsigned char *str);
unsigned char TimeCheck(DataTime time);
long int AtoLong(unsigned char *p,unsigned char length,unsigned char type);
unsigned char Timedaymax(DataTime time);

long atofxy(unsigned char *str,unsigned char type);

unsigned char* Search(unsigned char* base,unsigned char nmemb,unsigned char* key,unsigned char len);
int timecmp(DataTime *t0,DataTime *t1);
unsigned int Getbyte2(unsigned char *pa,unsigned char add);
long Getbyte4(unsigned char *pa,unsigned char add);
void Setbyte2(unsigned char *pa,unsigned char add,unsigned int value);
void Setbyte4(unsigned char *pa,unsigned char add,long value);


#endif  /*#ifndef __PUBLIC_H*/

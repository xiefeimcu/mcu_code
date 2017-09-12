/*********************************************************
*文件名称：Public.c
*摘    要：与硬件无关的常用功能函数
*
*当前版本：1.0
*作    者：赵阳 
*更新日期：2013年12月03日
*
**********************************************************/

#include "Public.h"

const unsigned char HEXASCII[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

const unsigned char auchCRCHi[] = 
{
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;

const unsigned char auchCRCLo[] = 
{
  0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
  0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
  0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
  0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
  0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
  0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
  0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
  0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
  0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
  0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
  0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
  0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
  0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
  0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
  0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
  0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
  0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
  0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
  0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
  0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
  0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
  0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
  0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
  0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
  0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
  0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/*******************************************************************************
函数名称：unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen)
函数介绍：查表法CRC16
输入参数：unsigned char *puchMsg-------->CRC校验计算内容的首地址
          unsigned int usDataLen-------->需要校验的长度
输出参数：CRC校验码
返回值  ：CRC校验码
*******************************************************************************/
unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned char uchCRCHi = 0xFF; 
  unsigned char uchCRCLo = 0xFF;
  unsigned char uIndex ; 			

  while (usDataLen--) 		
  {
    uIndex = uchCRCHi ^ *puchMsg++;
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
    uchCRCLo = auchCRCLo[uIndex];
  }
  return (uchCRCHi << 8 | uchCRCLo);
}


/**************************************************************
函数名称：unsigned int strlenght(unsigned char *s) 
函数介绍：返回字符串长度，不含结束符
输入参数：字符串地址
输出参数：
返回值  ：成功返回字符串长度，空串或长度大于65530返回0
*************************************************************/
unsigned int strlenght(unsigned char *s) 
{
  unsigned int i;
  
  for(i=0;i<65530;i++)
  {
    if(*s == '\0')
    {
      return i;
    }
    s++;
  }
  return 0;
}

/*******************************************************************************
函数名称：unsigned char Byte_BCD(unsigned char byte)
函数介绍：将长度为1byte的无符号整型数转换为BCD码
输入参数：unsigned char byte-------->需要转换的无符号整型数
输出参数：无
返回值  ：成功返回BCD码
          失败返回0,HEX数大于99
*******************************************************************************/
unsigned char Byte_BCD(unsigned char byte)
{
  unsigned char bcd=0;
  
  if(byte > 99)
    return 0;
  
  bcd = byte / 10;
  bcd <<= 4;
  bcd += byte%10;
  
  return bcd;
}

/**************************************************************
函数名称：unsigned char BCD_Byte(unsigned char bcd)
函数介绍：将两位BCD码转化为长度为1byte的无符号整型数
输入参数：unsigned char bcd---->待转换的BCD码
输出参数：无
返回值  ：转换完成的无符号整型数
*************************************************************/
unsigned char BCD_Byte(unsigned char bcd)
{
  unsigned char temp=0;
  
  temp = bcd >> 4;
  temp *= 10;
  temp += bcd & 0X0F;
  return temp;
}

/**************************************************************
函数名称：signed char ASCII_Hex(unsigned char asc2)
函数介绍：将一个十六进制ASCII码字符转换为8bitHEX无符号整型
          有效字符 0~9 a~f A~F
输入参数：unsigned char asc2---->ASC2字符
输出参数：无
返回值  ：成功返回 转换值  失败返回 -1;
*************************************************************/
signed char ASCII_Hex(unsigned char asc2)
{
  if((asc2>='0')&&(asc2<='9'))
  {
    return (asc2 & 0X0F);
  }
  else if((asc2>='A')&&(asc2<='F'))
  {
    return (asc2 - 55);
  }
  else if((asc2>='a')&&(asc2<='f'))
  {
    return (asc2 - 87);
  }
  
  return -1;
}

/**************************************************************
函数名称：unsigned char ToAscii_Hex(unsigned char asc1,unsigned char asc2)
函数介绍：将2个ASC2码字符转换为HEX
输入参数：unsigned char asc1---->第一个ASC2字符
          unsigned char asc1---->第二个ASC2字符
输出参数：无
返回值  ：成功返回 转换值  失败返回 0;
*************************************************************/
unsigned char ToAscii_Hex(unsigned char asc1,unsigned char asc2)
{
  unsigned char hex=0;
  signed char temp=0;
  
  temp = ASCII_Hex(asc1);
  if(-1 == temp)                 //非法字符
    return 0;
  
  hex = temp;
  hex <<= 4;
  temp = ASCII_Hex(asc2);
  if(-1 == temp)                 //非法字符
    return 0;
  
  hex += temp;
  return hex; 
}

/**************************************************************
函数名称：unsigned char ToAscii_U8(unsigned char asc1,unsigned char asc2)
函数介绍：将2个ASC2码字符转换为8bit十进制无符号整型
输入参数：unsigned char asc1---->第一个ASC2字符
          unsigned char asc1---->第二个ASC2字符
输出参数：无
返回值  ：成功返回 转换值  遇到非法字符返回 0
*************************************************************/
unsigned char ToAscii_U8(unsigned char asc1,unsigned char asc2)
{
  unsigned char u8=0;
  
  if((asc1>='0')&&(asc1<='9'))
  {
    asc1 &= 0X0F;
    u8 = asc1 * 10;
  }
  else
  {
    return 0;
  }
  if((asc2>='0')&&(asc2<='9'))
  {
    asc2 &= 0X0F;
    u8 += asc2;
  }
  else
  {
    return 0;
  }
  
  return u8; 
}

/*******************************************************************************
函数名称：unsigned int crc(unsigned char *crc_pointer,unsigned int data_length)
函数介绍：CRC16
输入参数：unsigned char *crc_pointer-------->CRC校验计算内容的首地址
          unsigned int data_length  -------->需要校验的长度
输出参数：CRC校验码
返回值  ：CRC校验码
*******************************************************************************/
unsigned int crc(unsigned char *crc_pointer,unsigned int data_length)
{
  unsigned int k,k0,bit_flag;
  unsigned int int_crc=0xffff;
  for (k=0;k<data_length;k++)
  {
    int_crc ^= *(crc_pointer++);
    for(k0=0;k0<8;k0++)
    {
      bit_flag = int_crc&0x0001;
      int_crc >>= 1;
      if (bit_flag == 1)
        int_crc ^= 0xa001;                         
    }
  }
  return(int_crc);                     //高位在前低位在后
}

/*********************************************************************************************
函数名称：unsigned char SInt32_TO_String(unsigned char *p,long int value,unsigned char point)
函数介绍：将long int 数转为ASC2码字符串         
          负数自动加入'-'，转换时加入字符串结束符'\0'
输入参数：unsigned char *p----->转换完成后字符串的保存地址
          long int value------->需要转换的长整型数
          unsigned char type--->数据类型  type=0Xxy, N(x,y)[数据位，小数位]
          unsigned char cmd---->格式控制 Symbol加入+-符号，Zero保留前导0，space前导0替换为空格
                                           DEZero 删除尾部的零
输出参数：转换完成的字符串
返回值  ：小数位大于9时返回 0

          返回转换完成的字符串长度，不包括字符串结束符
*********************************************************************************************/
unsigned char SInt32_TO_String(unsigned char *p,long int value,unsigned char type,unsigned char cmd)
{
  unsigned char str[14]={0};                     //转换字符串缓存
  unsigned char flag=False;                      //负数标识
  unsigned long int quan=1;                      //十进制权 

  unsigned char i=0,j=0,k=0;
  ldiv_t lnum={0,0};

  if(0==type)
    return 0;
  
  if((type>>4)>9)
    return 0;
  if((type&0X0F)>9)
    return 0;
  
  i = type>>4;                                  //数据位数  
  for(;i>0;i--)                                 //最高位的权
  {
    quan *= 10;
  }
  
  if((type&0X0F) > 8)                           //小数位数大于8返回0                                                
  {
    return 0;
  }
  
  if(value < 0)                                 //转换的数为负数时
  {
    value = 0 - value;                          //计算绝对值
    flag = True;
  }
                       
  lnum = ldiv(value,quan);
  quan /= 10;
  
  /*转换为ASCⅡ字符串，从十进制最大值的最高位开始*/
  for(i=0;i<11;i++)
  {
    if((type>>4)==((type&0x0f)+i))                           //插入小数点
    {      
      if((type&0x0f) != 0)
        str[i] = '.';     
      continue;
    }
    
    lnum = ldiv(lnum.rem,quan);                 //长整数除法 除以十进制数该位的权 商=lnum.quot， 余数=lnum.rem
    quan /= 10;                                 
    str[i]=HEXASCII[(unsigned char)lnum.quot];//转换为ASCⅡ码
    if(0 == quan)
      break;
  } 

  for(i=0;i<11;i++)                             //查找第一个不为'0'字符的位置
  {
    if('\0' == str[i])                          //找到结束符退出查找
    {
      i--;
      break;
    }    
    if(str[i] != '0')  
    {
      if('.' == str[i])
        i--;
      break;
    }   
  }
  
  if(cmd & space)                        //前导0转换为空格
  {
    if(0==i)                            //不为‘0’的位置为0
    {
      j = 0;                                               
      if(flag == True)                              //为负数时先写入负号 '-'
      {
        p[j++] = '-';
      }
      else if(cmd & Symbol)                         //是否加入+号
      {
        p[j++] = '+';
      }
    }
    else
    {
      for(j=0;j<i;j++)
      {
        str[j] = ' ';
      }
      if(flag == True)
      {
        str[--j] = '-';
      }
      else if(cmd & Symbol)
      {
        str[--j] = '+';
      }
      j = 0;
    }    
  }
  else
  {
    j = 0;                                               
    if(flag == True)                              //为负数时先写入负号 '-'
    {
      p[j++] = '-';
    }
    else if(cmd & Symbol)                         //是否加入+号
    {
      p[j++] = '+';
    }
  }
  
  if((cmd&DEZero)&&(type&0X0F))                              //有小数时删除尾部的零
  {
    for(k=0;k<14;k++)                           //查找结束符
    {
      if(str[k] == 0)
        break;
    }
    if(k < 13)
    {
      k--;
      for(;k>0;k--)
      {
        if(str[k]=='0')
        {
          str[k] = 0;
        }
        else if(str[k]=='.')
        {
          str[k] = 0;
          break;
        }
        else
        {
          break;
        }  
      }     
    }
  }
  
  if(cmd &(Zero+space))                  //保留前导0或前导0替换为空格
    i = 0;
   
  for(;j<12;j++)
  {
    p[j] = str[i++];
    if('\0' == p[j])
      break;
  }
  
  return j;   
}

/*******************************************************************************
函数名称：long atofxy(unsigned char *str,unsigned char type)
函数介绍：字符串转换为指定格式定点小数
输入参数：unsigned char *str-------->转换的字符串      
输出参数：unsigned char type-------->数据类型  N(x,y)[数据位，小数位]
返回值  ： 转换完成数
失败返回 0
*******************************************************************************/
long atofxy(unsigned char *str,unsigned char type)
{
  double num=0;
  unsigned long x=1,max=1;
  unsigned char i=0;
  
  i = type & 0X0F;      //转换的小数位数
  for(;i>0;i--)
  {
    x *= 10;
  } 
  
  i = type >> 4;         //转换值允许的最大值
  for(;i>0;i--)
  {
    max *= 10;
  }
  num = atof((char const*)str); //转换结果
  num *= x;
  
  if((unsigned long)(fabs(num)) >= max) //转换结果大于允许的最大值返回0
  {
    return 0;
  }
  
  return (long)num;
}

/*******************************************************************************
函数名称：long int String_TO_SInt32(unsigned char *str)
函数介绍：字符串转换为32位有符号整数
输入参数：*str-------->转换的字符串      
输出参数：无
返回值  ： 转换完成的32位有符号整型数
失败返回 0
*******************************************************************************/
long int String_TO_SInt32(unsigned char *str)
{
  unsigned char i=0,j=0;
  unsigned long quan=1;
  long rvalue=0;
  
  i = strlenght(str);
  
  for(j=0;j<10;j++)     //查找最后一位数字的位置
  {
    i--;
    if((str[i]>='0')&&(str[i]<='9'))
      break;
    
    if(0==i)
      break;
  }
  for(j=0;j<10;j++)
  {
    if((str[i]>='0')&&(str[i]<='9'))
    {
      rvalue += (str[i]&0X0F)*quan;
      quan *= 10;
    }
    else if(!(str[i]=='+'||str[i]=='-'||str[i]=='.'))//非法字符
    {
        return 0;
    }
    if(0==i)
      break;
    i--;
  } 
  if(str[0]=='-')       //负数
    rvalue = 0 - rvalue;
     
  return rvalue;  
}

/*******************************************************************************
函数名称：unsigned char TimeCheck(Time time)
函数介绍：检查时间数据是否合法
输入参数：Time time-------->检查的时间                                
输出参数：需要检查的时间数据
返回值  ：正确返回1 
          错误返回0
*******************************************************************************/
unsigned char TimeCheck(DataTime time)
{
  if(time.Year > 99)
    return 0; 
  if(time.Month<1 || time.Month>12)
    return 0;
  if(time.Day<1 || time.Day>31)
    return 0;
  if(time.Hour > 23)
    return 0;
  if(time.Minute > 59)
    return 0;
  if(time.Second > 59)
    return 0;
  
  return 1; 
}

/*******************************************************************************
函数名称：long int AtoLong(unsigned char *p,unsigned char length,unsigned type)
函数介绍：将指定长度的内存内容指定格式定点小数
输入参数：unsigned char *p-------->首地址 
          unsigned char length---->长度
          unsigned char type------>数据类型
输出参数：无
返回值  ：正确返回 转换结果 
          错误返回 0
*******************************************************************************/
long int AtoLong(unsigned char *p,unsigned char length,unsigned char type)
{
  unsigned char strtemp[15]={0};
  unsigned char i=0;
  
  for(i=0;i<length;i++)
  {
    strtemp[i] = p[i];
  }
  return atofxy(strtemp,type);
}

/*******************************************************************************
函数名称：unsigned char Timedaymax(Time time)
函数介绍：计算时间数据日最大值
输入参数：Time time-------->计算的时间       
输出参数：无
返回值  ：当月日最大值
*******************************************************************************/
unsigned char Timedaymax(DataTime time)
{
   unsigned char mont[7]={1,3,5,7,8,10,12};//最大为31天的月份
   unsigned char i=0;
   unsigned char max=0;
   
   for(i=0;i<7;i++)
   {
     if(time.Month == mont[i])
       break;
   }
   if(i < 7)
   {
     max = 31;
   }
   else if(time.Month == 2)
   {
     max = 28;
     if(time.Year%4 == 0)//闰年
      max = 29;
   }
   else
     max = 30;
   
   return max;
}


/*******************************************************************************
函数名称：unsigned char* Search(unsigned char* base,unsigned char nmemb,unsigned char* key,unsigned char len)
函数介绍：搜索
输入参数：unsigned char* base---->被搜索数组指针 
          unsigned char nmemb---->被搜索数组元素个数
          unsigned char* key ---->要搜索对象数组指针
          unsigned char len  ---->要搜索数组元素个数
输出参数：无
返回值  ：成功返回base中与key内容相同的指针
          失败返回空指针 NULL
*******************************************************************************/
unsigned char* Search(unsigned char* base,unsigned char nmemb,unsigned char* key,unsigned char len)
{
  unsigned char i=0;
  unsigned char j=0,k=0;

  if(nmemb < len)
    return NULL;
  
  for(i=0;i<=nmemb-len;i++)
  {
    if(base[i]==key[0])
    {
      k = i;
      for(j=0;j<len;j++,k++)
      {
        if(base[k] != key[j])
          break;
      }
      if(j >= len)
        return &base[i];
    }
  }  
  
  return NULL;
}
/*比较两个时间
t0 > t1 返回 1
t0 < t1 返回-1
t0 = t1 返回0
*/
int timecmp(DataTime *t0,DataTime *t1)
{
  unsigned char *pt0,*pt1;
  unsigned char i=0;
  
  pt0 = (unsigned char*)t0;
  pt1 = (unsigned char*)t1;
  
  for(i=0;i<6;i++)
  {
    if(pt0[i]>pt1[i])
      return 1;
    
    if(pt0[i]<pt1[i])
      return -1;
  }
  
  return 0;
}

unsigned int Getbyte2(unsigned char *pa,unsigned char add)
{
  Uint16 value={0};
  
  value.bit16.nHigh = pa[add++];
  value.bit16.nLow = pa[add];
  return value.num;
}

long Getbyte4(unsigned char *pa,unsigned char add)
{
  Slong32 s32={0};
  
  s32.bit32.nHigh.bit16.nHigh = pa[add++];          //高16bit中的高8bit
  s32.bit32.nHigh.bit16.nLow = pa[add++];           //高16bit中的低8bit
  s32.bit32.nLow.bit16.nHigh = pa[add++];           //低16bit中的高8bit
  s32.bit32.nLow.bit16.nLow = pa[add];              //低16bit中的低8bit
  
  return s32.num;
}

void Setbyte2(unsigned char *pa,unsigned char add,unsigned int value)
{
  Uint16 u16={0};
  
  u16.num = value;
  pa[add++] = u16.bit16.nHigh;
  pa[add] = u16.bit16.nLow;
}

void Setbyte4(unsigned char *pa,unsigned char add,long value)
{
  Slong32 s32={0};
  
  s32.num = value;
  
  pa[add++] = s32.bit32.nHigh.bit16.nHigh;
  pa[add++] = s32.bit32.nHigh.bit16.nLow;
  pa[add++] = s32.bit32.nLow.bit16.nHigh;
  pa[add] = s32.bit32.nLow.bit16.nLow;
}
/*******************************************************************************
函数名称：convert_byte_to_ascall
函数介绍：把byte转成两个字节的ascall
输入参数：

输出参数:
返回值  ：
*******************************************************************************/

void convert_byte_to_ascall(uint8_t byte,uint8_t *asc1,uint8_t *asc2){
	uint8_t hByte = byte >> 16;
	uint8_t lByte = byte - (byte >> 16);

	if(hByte >= 10){
		*asc1 = hByte + 'a' - 10;//a-f
	}
	else{
		*asc1 = hByte + '0';
 	}

	if(lByte >= 10){
		*asc2 = lByte + 'a' - 10;
	}
	else{
		*asc2 = lByte + '0';
 	}
}


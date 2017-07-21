/*********************************************************
*文件名称：ST7920code.h
*摘    要：字库字模定义
*          更新字库后必须修改字库长度的宏定义
*
*作    者：赵阳
*开始日期：2012年5月23日
*完成日期：2012年5月24日
*           
**********************************************************/

#ifndef __ST7920CODE_H
#define __ST7920CODE_H 

/************更新字库后必须修改字库长度的宏定义***********/

#define ASC2StringCodeLeng         117            //ASC2字符字库长度
#define ASC2StringCodeLeng_8X6     16            //ASC2字符8*6字库长度
#define GB2312StringCodeLeng       161           //GB2312字符字库长度

/********************************************************
       ASC2码字符字模结构体 8*16点阵
********************************************************/
struct ASC2StringCode
{
  char ASC2;                   //字符编码
  char ASC2Code[16];           //字模数组
};

/********************************************************
       GB2312码字符字模结构体 16*16点阵
********************************************************/
struct GB2312StringCode
{ 
  unsigned int GB2312;                        //字符编码
  char GB2312LeftCode[16];            //字符左边字模数组
  char GB2312RightCode[16];           //字符右边字模数组
};


/********************************************************
       ASC2码字符字模结构体 8*6点阵
********************************************************/
struct ASC2StringCode_8X6
{
  char ASC2;                   //字符编码
  char ASC2Code[8];            //字模数组
};


extern const struct ASC2StringCode ASC2Code[ASC2StringCodeLeng ];
extern const struct GB2312StringCode GB2312Code[GB2312StringCodeLeng];
extern const struct ASC2StringCode ASC2Code_8X6[ASC2StringCodeLeng_8X6];

extern const char logp[488];           //开机图片
extern const char rightarrow[18];      //右箭头
extern const char leftarrow[18];       //左箭头
extern const char Hourglass[116];      //沙漏
extern const char qingtian[32];        //晴天图片
extern const char baojing[32];         //报警图片
#endif /* #ifndef __ST7920CODE_H */
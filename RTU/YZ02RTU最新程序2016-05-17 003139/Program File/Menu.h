/*********************************************************
*文件名称：menu.h
*摘    要：菜单设置声明
* 
*作    者： 赵阳
*开始日期： 2012年12月22日
*完成日期：
**********************************************************/

#ifndef  _MENU_H
#define  _MENU_H

#include "include.h"
 
#define AdminPass               111111           //管理员密码
#define UserPass                111111           //用户密码

/************* 菜单结构体定义 ********************/
struct Menu
{
  unsigned char MenuCount;      //菜单长度
  unsigned char H;              //菜单选项提示字显示水平坐标  
  unsigned char *string;        //菜单选项提示字符串 
  void (*Subs)();               //选择后执行的动作
  struct Menu *SubMenu;         //子级菜单  
  struct Menu *PreviousMenu;    //父级菜单   
};

/***************时间设置菜单结构体**********************/
struct TimeSetMenu 
{
  unsigned char type;
  unsigned char stringadd_x;
  unsigned char stringadd_y;
  unsigned char *string;
  unsigned char disadd_x;
  unsigned char disadd_y;
  void (*Subs)(unsigned char x,unsigned char y,unsigned char z);
};


/*设置数字结构*/
typedef struct
{
  long value;   //设置初始值
  unsigned char type;   //数据类型 N(x,y)
  unsigned char negative;//是否允许负值 
  unsigned char *str;   //数字后面显示的字符串
  long max;    //最大值
  long min;    //最小值
}setnum;

typedef struct
{
  unsigned char user;     //被选中的项
  unsigned char *str[10];  //选项显示字符串
}setstring;



void SetMode(void);
void WaitKey(void);
unsigned char PassWord(void);
long int SetNum(setnum num,unsigned char H,unsigned char Ver);
void SetMenu(void);
void ShowMenu(struct Menu *MenuUser,unsigned char DisplayPoint,signed char USER,signed char UserChoose,unsigned char refresh);
unsigned char SetString(setstring s,unsigned char H,unsigned char Ver);
void settimetype(unsigned char type,unsigned char H,unsigned char Ver);
void SetClock(void);
void SetResetFactory(void);

unsigned char SetYaoSu(unsigned char ys,unsigned char H,unsigned char Ver);
void SetRTUtype(void);
void SetCenteradd(void);
void SetST(void);
void SetWorkMode(void);
void SetDSBJG(void);
void SetJBJG(void); 
void SetPDTime(void);
void SetCYJG(void);
void SetCCJG(void);
void SetYLJFBL(void);
void SetYLJBFZ(void);
void SetYLJType(void);
void SetSWJZ(void);
void SetSWXZJZ(void);
void SetDTUPower(void);
void SetDTUPreheat(void);
void SetDTUPreheat(void);
void SetDTUCloseDelay(void);
void SetDTUTxdTime(void);
void SetDTUTxdMax(void);
void SetPT(void);
void SetRainJBSD(void);
void SetRainClear(void);
void SetDTUbps(void);
void SetWaterChaliang(void);
void SetRS485bps(void);
void SetRS485Power(void);
void SetRS485Preheat(void);
void SetRS485CgqType(void);
void SetRS485Name(void);
void SetTongDaoName_1(void);
void SetTongDaoSType_1(void);
void SetTongDaoMax_1(void);
void SetTongDaoMin_1(void);
void SetTongDaoName_2(void);
void SetTongDaoName_3(void);
void SetTongDaoName_4(void);
void SetTongDaoSType_2(void);
void SetTongDaoSType_3(void);
void SetTongDaoSType_4(void);
void SetTongDaoMax_2(void);
void SetTongDaoMax_3(void);
void SetTongDaoMax_4(void);
void SetTongDaoMin_2(void);
void SetTongDaoMin_3(void);
void SetTongDaoMin_4(void);
void SetHeight(void);
void SysInfo(void);
  
#endif  /*#ifndef  _MENU_H*/



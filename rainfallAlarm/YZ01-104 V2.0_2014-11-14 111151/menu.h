/*********************************************************
*文件名称：menu.h
*摘    要：菜单设置声明
* 
*作    者： 赵阳
*开始日期： 2013年10月12日
*完成日期：
**********************************************************/

#ifndef  _MENU_H
#define  _MENU_H

#include "Alarm.h"
#include "include.h"
 
#define AdminPass               111111           //管理员密码
#define UserPass                111111           //用户密码


/************* 菜单结构体定义 ********************/
struct Menu
{
  char MenuCount;      //菜单长度
  char H;              //菜单选项提示字显示水平坐标  
  char *string;        //菜单选项提示字符串 
  void (*Subs)();               //选择后执行的动作
  struct Menu *SubMenu;         //子级菜单  
  struct Menu *PreviousMenu;    //父级菜单   
};

/***************时间设置菜单结构体**********************/
struct TimeSetMenu 
{
  char type;
  char stringadd_x;
  char stringadd_y;
  char *string;
  char disadd_x;
  char disadd_y;
  void (*Subs)(char x,char y,char z);
};


/*设置数字结构*/
typedef struct
{
  long value;   //设置初始值
  char type;   //数据类型 N(x,y)
  char negative;//是否允许负值 
  char *str;   //数字后面显示的字符串
  long max;    //最大值
  long min;    //最小值
}setnum;

typedef struct
{
  char user;     //被选中的项
  char *str[10];  //选项显示字符串
}setstring;

typedef struct
{
  AlarmValue *pA;
  char at_i;
  char at_ii;
  char at_iii;  
}AlarmSetClass;

extern const struct Menu MainMenu[5];
extern const struct Menu SysSetMenu[3];

void SetMode(void);
void SetMenu(void); 
void ShowMenu(struct Menu *MenuUser,char DisplayPoint,signed char USER,signed char UserChoose,char refresh);
void WaitKey(void);
long int SetNum(setnum num,char H,char Ver);
char PassWord(void);
void SetClock(void);
void settimetype(char type,char H,char Ver);
void SetYLJFBL(void);
void SetRainClear(void);
void RainfallClear(void);
void SysInfo(void);
void WriteConfig(void);
void ResetFactory(void);
void SetResetFactory(void);
void DefaultConfig(void);
void AlarmValueSet(void);
void SystemSelfTestMenu(void);
#endif  /*#ifndef  _MENU_H*/



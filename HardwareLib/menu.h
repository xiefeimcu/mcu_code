/*
 * menu.h
 *
 *  Created on: 2017年10月16日
 *      Author: Administrator
 */

/*********************************************************
*文件名称：menu.h
*摘    要：菜单设置声明
*
*作    者： 赵阳
*开始日期： 2012年12月22日
*
*修改 ：谢飞
*修改日期：2017年10月16日
**********************************************************/

#ifndef  _MENU_H
#define  _MENU_H

#include "include.h"

#define AdminPass               111111           //管理员密码
#define UserPass                111111           //用户密码

#define KEY_TIME_OUT 0

/************* 菜单结构体定义 ********************/
struct Menu
{
  uint8_t MenuCount;      //菜单长度
  uint8_t H;              //菜单选项提示字显示水平坐标
  const char *string;        //菜单选项提示字符串
  void (*Subs)();               //选择后执行的动作
  struct Menu *SubMenu;         //子级菜单
  struct Menu *PreviousMenu;    //父级菜单
};

typedef enum {
H_YEAR,
H_MONTH,
H_DAY,
H_HOUR,
H_MINUTE,
H_SECOND,
} timeType_t;

/***************时间设置菜单结构体**********************/
struct TimeSetMenu
{
  timeType_t type;
  uint8_t stringadd_x;
  uint8_t stringadd_y;
  const char *string;
  uint8_t disadd_x;
  uint8_t disadd_y;
  void (*Subs)(uint8_t x,uint8_t y,uint8_t z);
};


/*设置数字结构*/
typedef struct
{
  long value;   //设置初始值
  uint8_t type;   //数据类型 N(x,y)
  uint8_t negative;//是否允许负值
  uint8_t *str;   //数字后面显示的字符串
  long max;    //最大值
  long min;    //最小值
}setnum;

typedef struct
{
  uint8_t user;     //被选中的项
  const char *str[11];  //选项显示字符串
}setstring;

void SetMode(void);
void WaitKey(void);
uint8_t PassWord(void);
long int SetNum(setnum num,uint8_t H,uint8_t Ver);
void SetMenu(void);
void ShowMenu(struct Menu *MenuUser,uint8_t DisplayPoint,signed char USER,signed char UserChoose,uint8_t refresh);
uint8_t SetString(setstring s,uint8_t H,uint8_t Ver);
void settimetype(uint8_t type,uint8_t H,uint8_t Ver);
void SetClock(void);
void SetResetFactory(void);

uint8_t SetYaoSu(uint8_t ys,uint8_t H,uint8_t Ver);
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
void SetDTKEY_UPower(void);
void SetDTKEY_UPreheat(void);
void SetCKEY_SETadd(void);

#endif  /*#ifndef  _MENU_H*/



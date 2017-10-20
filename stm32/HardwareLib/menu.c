/*
 * menu.c
 *
 *  Created on: 2017年10月16日
 *      Author: Administrator
 */

/*********************************************************
*文件名称：SetKEY_UPMenu.c
*摘    要：菜单设置功能定义
*
*作    者：赵阳
*开始日期： 2013年05月26日
*
*修改 ：谢飞
*修改日期：2017年10月16日
*
**********************************************************/

#include "menu.h"
#include "key.h"
#include "lcd_12864.h"

extern const struct Menu MainMenu[8];
extern const struct Menu ConfigMenu[7];
extern const struct Menu RainfallMenu[7];
extern const struct Menu WaterMenu[4];
extern const struct Menu ComConfigMenu[6];
extern const struct Menu RS485Menu[3];
extern const struct Menu TongDaoMenu[2];
extern const struct Menu TongDao1Menu[4];
extern const struct Menu TongDao2Menu[4];
//extern const struct Menu TongDao3Menu[4];
//extern const struct Menu TongDao4Menu[4];
extern const struct Menu CaiYangMenu[5];

const struct Menu MainMenu[8]={            //主菜单
{8,4,"基本配置",NULL,(struct Menu*)ConfigMenu,NULL},
{8,4,"雨量参数",NULL,(struct Menu*)RainfallMenu,NULL},
{8,4,"水位参数",NULL,(struct Menu*)WaterMenu,NULL},
{8,4,"采样设置",NULL,(struct Menu*)CaiYangMenu,NULL},
{8,4,"通信设置",NULL,(struct Menu*)ComConfigMenu,NULL},
{8,4,"系统时间",SetClock,NULL,NULL},
{8,4,"恢复出厂",SetResetFactory,NULL,NULL},
{8,4,"系统信息",SysInfo,NULL,NULL}
};


const struct Menu ConfigMenu[7]={        //基本配置
{7,4,"测站类型",SetRTUtype,NULL,(struct Menu*)MainMenu},
{7,4,"中心地址",SetCKEY_SETadd,NULL,(struct Menu*)MainMenu},
{7,4,"测站地址",SetST,NULL,(struct Menu*)MainMenu},
{7,4,"工作模式",SetWorkMode,NULL,(struct Menu*)MainMenu},
{7,4,"定时报间隔",SetDSBJG,NULL,(struct Menu*)MainMenu},
{7,4,"加报间隔",SetJBJG,NULL,(struct Menu*)MainMenu},
{7,4,"存储间隔",SetCCJG,NULL,(struct Menu*)MainMenu}
};

const struct Menu RainfallMenu[7]={		//雨量参数
{7,4,"雨量计类型",SetYLJType,NULL,(struct Menu*)MainMenu},
{7,4,"雨量分辨力",SetYLJFBL,NULL,(struct Menu*)MainMenu},
{7,4,"日起始时间",SetPDTime,NULL,(struct Menu*)MainMenu},
{7,4,"加报时段",SetRainJBSD,NULL,(struct Menu*)MainMenu},
{7,4,"加报阀值",SetYLJBFZ,NULL,(struct Menu*)MainMenu},
{7,4,"累计雨量",SetPT,NULL,(struct Menu*)MainMenu},
{7,4,"雨量清零",SetRainClear,NULL,(struct Menu*)MainMenu}
};

const struct Menu WaterMenu[4]={	   //水位参数
{4,4,"水位基值",SetSWJZ,NULL,(struct Menu*)MainMenu},
{4,4,"修正基值",SetSWXZJZ,NULL,(struct Menu*)MainMenu},
{4,4,"水位变率",SetWaterChaliang,NULL,(struct Menu*)MainMenu},
{4,4,"安装高度",SetHeight,Null,(struct Menu*)MainMenu}
};

const struct Menu ComConfigMenu[6]={    //通信设置
{6,4,"通信电源",SetDTKEY_UPower,Null,(struct Menu*)MainMenu},
{6,4,"通信预热",SetDTKEY_UPreheat,Null,(struct Menu*)MainMenu},
{6,4,"掉电延时",SetDTUCloseDelay,Null,(struct Menu*)MainMenu},
{6,3,"通信波特率",SetDTUbps,Null,(struct Menu*)MainMenu},
{6,4,"超时时间",SetDTUTxdTime,Null,(struct Menu*)MainMenu},
{6,4,"重发次数",SetDTUTxdMax,Null,(struct Menu*)MainMenu}
};

const struct Menu RS485Menu[3]={		//RS485通道
{3,4,"测量要素",SetRS485Name,NULL,(struct Menu*)MainMenu},
{3,4,"波特率",SetRS485bps,NULL,(struct Menu*)MainMenu},
{3,3,"传感器类型",SetRS485CgqType,NULL,(struct Menu*)MainMenu}
};

const struct Menu TongDaoMenu[2]={		//模拟通道
{2,2,"模拟量通道1",NULL,(struct Menu*)TongDao1Menu,(struct Menu*)MainMenu},
{2,2,"模拟量通道2",NULL,(struct Menu*)TongDao2Menu,(struct Menu*)MainMenu},
//{4,2,"模拟量通道3",NULL,(struct Menu*)TongDao3Menu,(struct Menu*)MainMenu},
//{4,2,"模拟量通道4",NULL,(struct Menu*)TongDao4Menu,(struct Menu*)MainMenu},
};

const struct Menu TongDao1Menu[4]={		//模拟通道 1
{4,4,"测量要素",SetTongDaoName_1,NULL,(struct Menu*)TongDaoMenu},
{4,4,"输入信号",SetTongDaoSType_1,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最大值",SetTongDaoMax_1,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最小值",SetTongDaoMin_1,NULL,(struct Menu*)TongDaoMenu},
};

const struct Menu TongDao2Menu[4]={		//模拟通道 2
{4,4,"测量要素",SetTongDaoName_2,NULL,(struct Menu*)TongDaoMenu},
{4,4,"输入信号",SetTongDaoSType_2,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最大值",SetTongDaoMax_2,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最小值",SetTongDaoMin_2,NULL,(struct Menu*)TongDaoMenu},
};

/*const struct Menu TongDao3Menu[4]={	//模拟通道 3
{4,4,"测量要素",SetTongDaoName_3,NULL,(struct Menu*)TongDaoMenu},
{4,4,"输入信号",SetTongDaoSType_3,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最大值",SetTongDaoMax_3,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最小值",SetTongDaoMin_3,NULL,(struct Menu*)TongDaoMenu},
};

const struct Menu TongDao4Menu[4]={		//模拟通道 4
{4,4,"测量要素",SetTongDaoName_4,NULL,(struct Menu*)TongDaoMenu},
{4,4,"输入信号",SetTongDaoSType_4,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最大值",SetTongDaoMax_4,NULL,(struct Menu*)TongDaoMenu},
{4,3,"量程最小值",SetTongDaoMin_4,NULL,(struct Menu*)TongDaoMenu},
};*/

const struct Menu CaiYangMenu[5]={	//采样设置
{5,4,"采样间隔",SetCYJG,NULL,(struct Menu*)MainMenu},
{5,4,"采样电源",SetRS485Power,NULL,(struct Menu*)MainMenu},
{5,4,"采样预热",SetRS485Preheat,NULL,(struct Menu*)MainMenu},
{5,4,"RS485通道",NULL,(struct Menu*)RS485Menu,NULL},
{5,3,"模拟量通道",NULL,(struct Menu*)TongDaoMenu,NULL},
};


const struct TimeSetMenu TimeSet[6]={   //时间设置
	{H_YEAR,2,8,"年",5,8,settimetype},
	{H_MONTH,2,24,"月",5,24,settimetype},
	{H_DAY,2,40,"日",5,40,settimetype},
	{H_HOUR,10,8,"时",13,8,settimetype},
	{H_MINUTE,10,24,"分",13,24,settimetype},
	{H_SECOND,10,40,"秒",13,40,settimetype}
};

/*******************************************************************************
函数名称：void SetMenu(void)
函数介绍：设置模式
输入参数：无
输出参数：无
返回值  ：无
*******************************************************************************/
void SetMode(void)
{
  Display.Status.Flag.Mon = RESET;
  Display.Status.Flag.Menu = SET;  //设置界面状态标志置位
  lcd_clear_ddram();
  if(0!=PassWord())
  {
    SetMenu();
  }

  key_event_handle = Null;
  lcd_clear_ddram();
  DisplayMon();      //显示监测界面
  KEY_TIME_OUT = RESET;
  Display.Status.Flag.SetMode = RESET;
}

/*******************************************************************************
函数名称：void ShowMenu(Menu *MenuUser,uint8_t DisplayPoint,uint8_t refresh)
函数介绍：显示菜单
输入参数：Menu *MenuUser---->显示菜单地址
          DisplayPoint----->显示点
          UserChoose------->当前用户选择
          USER------------->前一次用户选择
          refresh---------->全部刷新标志
输出参数：无
返回值  ：无
*******************************************************************************/
void ShowMenu(struct Menu *MenuUser,uint8_t DisplayPoint,signed char USER,signed char UserChoose,uint8_t refresh)
{
  uint8_t i=0;
  while(i<4)
  {
    if(1==refresh)    //显示全部刷新
    {
      if(UserChoose == DisplayPoint) //当前显示的菜单选项是否是被用户选择
        DisplayString(MenuUser[DisplayPoint].H,i<<4,MenuUser[DisplayPoint].string,0XFFFF);
      else
        DisplayString(MenuUser[DisplayPoint].H,i<<4,MenuUser[DisplayPoint].string,0);    //显示菜单选项字符串
    }
    else    //只刷新用户选择项
    {
      if(USER == DisplayPoint)//显示点等于前一次用户选择
      {
        DisplayString(MenuUser[DisplayPoint].H,i<<4,MenuUser[USER].string,0);    //显示菜单选项字符串
      }
      else if(UserChoose == DisplayPoint)//显示点为当前用户选择
      {
        DisplayString(MenuUser[DisplayPoint].H,i<<4,MenuUser[DisplayPoint].string,0XFFFF);    //显示菜单选项字符串
      }
    }
    i++;
    DisplayPoint++;     //显示点加1
    if(DisplayPoint==MenuUser[0].MenuCount)
      return;
  }
}

/*******************************************************************************
函数名称：void SetMenu(void)
函数介绍：设置菜单
输入参数：无
输出参数：无
返回值  ：无
*******************************************************************************/
void SetMenu(void)
{
  signed char user[5]={0};              //不同层级菜单用户选择缓存
  uint8_t i=0;                    //菜单层级
  signed char UserChoose = 0;           //当前用户选择
  signed char USER=0;                   //前一次用户选择
  uint8_t  DisplayPoint = 0;      //显示点
  uint8_t  RefreshMenu = 1;       //刷新菜单标志
  struct Menu *MenKEY_UPoint = (struct Menu*)MainMenu;

  lcd_clear_ddram();
  while(1)
  {
    ShowMenu(MenKEY_UPoint,DisplayPoint,USER,UserChoose,RefreshMenu);         //显示目录

    WaitKey();     //等待按键按下

      switch(key_event_handle)
      {
        case KEY_UP:
                USER = UserChoose;                      //保存前一次用户选择
                UserChoose--;                           //用户选择上移
                if(UserChoose < 0)                      //当前用户选择为第一项时 再向上选择
                {
                  UserChoose = MenKEY_UPoint[0].MenuCount - 1;           //用户选择指向最后一项
                  if(MenKEY_UPoint[0].MenuCount > 4)                     //菜单长度大于显示屏最大行数
                  {
                    lcd_clear_ddram();                      //清屏
                    RefreshMenu = 1;
                  }
                }
                else if(((UserChoose+1)%4)==0)          //向上翻页
                {
                  lcd_clear_ddram();
                  RefreshMenu = 1;
                }
                break;
        case KEY_DOWN:                                      //用户向下选择
                USER = UserChoose;                      //保存前一次用户选择
                UserChoose++;
                if(UserChoose >= MenKEY_UPoint[0].MenuCount)             //用户选择大于菜单长度
                {
                  UserChoose = 0;                       //用户选择指向首项
                  if(MenKEY_UPoint[0].MenuCount > 4)
                  {
                    lcd_clear_ddram();
                    RefreshMenu = 1;
                  }
                }
                break;

        case KEY_RETURN:                                       //返回上级菜单
                if(MenKEY_UPoint == MainMenu)               //当前菜单为主菜单时 退出菜单
                {
                  return;
                }
                else if(MenKEY_UPoint[0].PreviousMenu != NULL)
                {
                  MenKEY_UPoint = MenKEY_UPoint[0].PreviousMenu;
                  UserChoose = user[--i]; //恢复上级菜单用户选择
                  lcd_clear_ddram();
                  RefreshMenu = 1;
                }
                break;

        case KEY_SET:                                             //进入下一级菜单或设置
                if(MenKEY_UPoint[UserChoose].Subs != NULL)          //进入对应设置
                {
                  lcd_clear_ddram();
                  (*MenKEY_UPoint[UserChoose].Subs)();
                }
                else if(MenKEY_UPoint[UserChoose].SubMenu != NULL)  //进入下一级菜单
                {
                  MenKEY_UPoint=MenKEY_UPoint[UserChoose].SubMenu;
                  user[i++] = UserChoose;     //保存当前菜单的用户选择
                  UserChoose = 0;
                }
                RefreshMenu = 1;
                lcd_clear_ddram();
                break;
        default :break;
      }

    DisplayPoint = UserChoose-UserChoose%4;     // 4 指屏幕显示最大行数
    if((UserChoose+USER)%8 == 7)                // 上下翻页时清屏
    {
      lcd_clear_ddram();
      RefreshMenu = 1;
    }
    if(KEY_TIME_OUT)              //等待按键超时 返回监测界面
    {
      KEY_TIME_OUT = False;
      lcd_clear_ddram();
      return;
    }
  }
}

/*等待按键*/
void WaitKey(void)
{
  unsigned int i=0;

  key_event_handle = Null;
  while(!key_event_handle)
  {
    SysEventHandlers();         //系统事件处理
    UserEventHandlers();        //任务处理
    delayms(1);
    if(i++ > 20000)
    {
      KEY_TIME_OUT = True;
      key_event_handle = Null;
      return;
    }
  }
}

uint8_t PassWord(void)
{
  long x=0;
  setnum num={0,0X60,0,"\0"};

  DisplayString(4,16,"输入密码",0);

  num.max = 9999999;
  num.min = 0;
  x = SetNum(num,5,32);

  if(KEY_TIME_OUT)
      return 0;
  if(UserPass == x)
    return 2;
  if(AdminPass == x)
    return 1;

  return 0;
}


/*******************************************************************************
函数名称：long int SetNum(setnum num,uint8_t H,uint8_t Ver)
函数介绍：设置数字
输入参数：setnum num----->设置数的数据结构 定义在menu.h
          H-------------->显示水平坐标
          Ver------------>显示垂直坐标
输出参数：无
返回值  ： 用户输入的值

*******************************************************************************/
long int SetNum(setnum num,uint8_t H,uint8_t Ver)
{
  uint8_t strnum[15]={0};
  uint8_t i=0,j=0,k=0;
  int user=0;
  unsigned int fb=0;

  i = SInt32_TO_String(strnum,num.value,num.type,num.negative+Zero);//转换为字符串时保留前导零
  k = i;                //字符串长度
  for(j=0;j<10;j++)      //转换完成后字符串 尾部加入需要显示的字符
  {
    if(num.str[j]=='\0')
      break;
    strnum[i++] = num.str[j];
  }

  while(1)
  {
    fb = 0X8000 >> user;
    DisplayString(H,Ver,strnum,fb);

    WaitKey();//等待按键按下

    switch(key_event_handle)
    {
      case KEY_UP:  if('+'==strnum[user])
                  strnum[user] = '-';
                else if(('-'==strnum[user]))
                  strnum[user] = '+';
                else if(++strnum[user]>'9')
                  strnum[user] = '0';
                break;
      case KEY_DOWN: if('+'==strnum[user])
                   strnum[user] = '-';
                 else if(('-'==strnum[user]))
                   strnum[user] = '+';
                 else if(--strnum[user]<'0')
                   strnum[user] = '9';
                 break;
      case KEY_RIGHT: user++;
                  if('.'==strnum[user])
                    user++;
                  else if((strnum[user]>'9')||(strnum[user]<'0'))
                    user = 0;
                  break;
      case KEY_LEFT:  user--;
                  if(user<0)
                    user = k-1;
                  else if('.'==strnum[user])
                    user--;
                  break;
      case KEY_RETURN:   return num.value;
      case KEY_SET: num.value = String_TO_SInt32(strnum);
                  Draw(H,Ver,16,16,0,Null);
                  if(num.value > num.max)
                    num.value = num.max;
                  else if(num.value < num.min)
                    num.value = num.min;
                  return num.value;
      default :break;
    }

    if(KEY_TIME_OUT)    //等待按键超时
      return num.value;
  }
}

/*******************************************************************************
函数名称：uint8_t SetString(setstring s,uint8_t H,uint8_t Ver)
函数介绍：选择字符
输入参数：setstring s----->提供的待选项
          H--------------->水平显示地址
          Ver------------->垂直显示地址
输出参数：无
返回值  ：用户选择项编号
*******************************************************************************/
uint8_t SetString(setstring s,uint8_t H,uint8_t Ver)
{
  uint8_t i=0,j=0;

  j = s.user;
  for(i=0;i<10;i++)
  {
    if(s.str[i][0]=='\0')
      break;
  }
  i--;
  while(1)
  {
    DisplayString(H,Ver,s.str[s.user],0);

    WaitKey();//等待按键按下

    switch(key_event_handle)
    {
      case KEY_UP:  Draw(H,Ver,strlenght(s.str[s.user]),16,0,Null);
                s.user++;
                if(s.user > i)
                  s.user = 0;
                break;
      case KEY_DOWN: Draw(H,Ver,strlenght(s.str[s.user]),16,0,Null);
                if(0==s.user)
                   s.user = i;
                 else
                   s.user--;
                 break;
      case KEY_RETURN: return j;
      case KEY_SET:return s.user;
    }
    if(KEY_TIME_OUT)
      return j;
  }
}

uint8_t SetYaoSu(uint8_t ys,uint8_t H,uint8_t Ver)
{
  uint8_t i;

  if(ys >= YAOSULENG)
  {
    i=ys=0;
  }
  else
  {
    i = ys;
  }

  while(1)
  {
    DisplayString(H,Ver,YaoSuInfo[ys].NameStr,0);

    WaitKey();//等待按键按下

    switch(key_event_handle)
    {
      case KEY_UP:  Draw(H,Ver,strlenght(YaoSuInfo[ys].NameStr),16,0,Null);
                ys++;
                if(ys > YAOSULENG-1)
                  ys = 0;
                break;
      case KEY_DOWN: Draw(H,Ver,strlenght(YaoSuInfo[ys].NameStr),16,0,Null);
                if(0==ys)
                   ys = YAOSULENG-1;
                 else
                   ys--;
                 break;
      case KEY_RETURN: return i;
      case KEY_SET:return ys;
      default :break;
    }
    if(KEY_TIME_OUT)
      return i;
  }
}

void SetRTUtype(void)
{
  uint8_t temp[5]={'P','H','K','Z','M'};//遥测站分类码
  uint8_t i=0;
  setstring settemp={0,{"降水","河道","水库","闸坝","泵站","","墒情","地下水","水质","取水口","排水口",}};

  DisplayString(4,8,"测站类型",0);
  H_LINE(2,49,12);
  for(i=0;i<5;i++)//参数超出范围恢复为 降水
  {
    if(rtuParameter.upDataArg.rtuType == temp[i])
      break;
  }
  if(i >= 5)
    i = 0;
  settemp.user = i;

  settemp.user = SetString(settemp,6,32);

  rtuParameter.upDataArg.rtuType = temp[settemp.user];

}

void SetCKEY_SETadd(void)
{
  setnum num={rtuParameter.upDataArg.centreStationAddr,0X30,0,""};
  num.max = 255;
  num.min = 0;

  DisplayString(3,8,"中心站地址",0);
  H_LINE(2,49,12);
  rtuParameter.upDataArg.centreStationAddr = (unsigned char)SetNum(num,6,32);
  WriteUInt8(rtuParameter.upDataArg.centreStationAddr,EEPROM.CKEY_SETAdd);//保存设置值
}

void SetST(void)
{
  uint8_t strtemp[11]={0};
  uint8_t i=0,j=0;
  unsigned int fb=0;

  DisplayString(4,8,"测站地址",0);
  H_LINE(2,49,12);
  for(i=0;i<10;)
  {
    strtemp[i++] = HEXASCII[RTU.ST[j]>>4];
    strtemp[i++] = HEXASCII[RTU.ST[j]&0X0F];
    j++;
    if(5==j)
      break;
  }
  i=0;
  while(1)
  {
    fb=0X8000>>i;
    DisplayString(3,32,strtemp,fb);

    WaitKey();//等待按键按下

      switch(key_event_handle)
      {
        case KEY_UP: if(++strtemp[i]>'9')
                   strtemp[i] = '0';
                 break;
        case KEY_DOWN: if(--strtemp[i]<'0')
                   strtemp[i] = '9';
                 break;
        case KEY_RIGHT: if(++i > 9)
                      i = 0;
                    break;
        case KEY_LEFT: if(0==i)
                      i = 9;
                   else
                     i--;
                   break;
        case KEY_SET: RTU.ST[0] = strtemp[0]<<4;
                    RTU.ST[0] += strtemp[1]&0X0F;
                    RTU.ST[1] = strtemp[2]<<4;
                    RTU.ST[1] += strtemp[3]&0X0F;
                    RTU.ST[2] = strtemp[4]<<4;
                    RTU.ST[2] += strtemp[5]&0X0F;
                    RTU.ST[3] = strtemp[6]<<4;
                    RTU.ST[3] += strtemp[7]&0X0F;
                    RTU.ST[4] = strtemp[8]<<4;
                    RTU.ST[4] += strtemp[9]&0X0F;
                    Write_NByte(RTU.ST,5,EEPROM.ST);//保存数据
                    return;
         case KEY_RETURN:  return;
         default :break;
      }
      if(KEY_TIME_OUT)
        return;

  }
}

void SetWorkMode(void)
{
  uint8_t temp[4]={WMODE_1,WMODE_2,WMODE_3,WMODE_4};
  uint8_t i=0;
  setstring settemp={0,{"  自报  ","自报确认","查询应答","调试维修",""}};

  DisplayString(4,8,"工作模式",0);
  H_LINE(2,49,12);
  for(i=0;i<4;i++)//参数超出复位 恢复为查询应答
  {
    if(RTU.WorkMode==temp[i])
      break;
  }
  if(i >= 4)
    i = 2;
  settemp.user = i;

  settemp.user = SetString(settemp,4,32);

  RTU.WorkMode = temp[settemp.user];
  WriteUInt8(RTU.WorkMode,EEPROM.WorkMode);//保存设置值
}

void SetDSBJG(void)
{
  setnum num={RTU.DingShiBaoTime,0X20,0,"H"};
  num.max = 24;
  num.min = 0;

  DisplayString(3,8,"定时报间隔",0);
  H_LINE(2,49,12);
  RTU.DingShiBaoTime = (unsigned char)SetNum(num,6,32);
  WriteUInt8(RTU.DingShiBaoTime,EEPROM.DingShiBaoTime);//保存设置值
}

void SetJBJG(void)
{
  setnum num={RTU.JiaBaoTime,0X30,0,"分钟"};
  num.max = 250;
  num.min = 0;

  DisplayString(4,8,"加报间隔",0);
  H_LINE(2,49,12);
  RTU.JiaBaoTime = (unsigned char)SetNum(num,6,32);
  WriteUInt8(RTU.JiaBaoTime,EEPROM.JiaBaoTime);   //保存设置值
}

void SetPDTime(void)
{
  setnum num={Rainfall.PDTime,0x20,0,"时"};
  num.max = 23;
  num.min = 0;

  DisplayString(3,8,"日起始时间",0);
  H_LINE(2,49,12);
  Rainfall.PDTime = (unsigned char)SetNum(num,6,32);
  WriteUInt8(Rainfall.PDTime,EEPROM.PDTime);//保存设置值
}

void SetPT(void)
{
  setnum num={Rainfall.PT,0X51,0,"mm"};
  num.max = 65535;
  num.min = 0;

  DisplayString(4,8,"累计雨量",0);
  H_LINE(2,49,12);
  Rainfall.PT = (unsigned int)SetNum(num,5,32);
  WriteUInt16(Rainfall.PT,EEPROM.PT);//保存设置值
}


void SetCYJG(void)
{
  setnum num={RTU.CaiYangJianGe,0X50,0,"分钟"};
  num.max = 65535;
  num.min = 0;

  DisplayString(4,8,"采样间隔",0);
  H_LINE(2,49,12);
  RTU.CaiYangJianGe = (unsigned int)SetNum(num,5,32);
  WriteUInt16(RTU.CaiYangJianGe,EEPROM.CaiYangJianGe);//保存设置值
}

void SetCCJG(void)
{
  setnum num={RTU.CunChuJianGe,0X50,0,"分钟"};
  num.max = 65535;
  num.min = 0;

  DisplayString(4,8,"存储间隔",0);
  H_LINE(2,49,12);
  RTU.CunChuJianGe = (unsigned int)SetNum(num,5,32);
  WriteUInt16(RTU.CunChuJianGe,EEPROM.CunChuJianGe);//保存设置值
}

void SetYLJType(void)
{
  setstring settemp={Rainfall.Type,{" NONE","翻斗式",""}};

  DisplayString(3,8,"雨量计类型",0);
  H_LINE(2,49,12);
  if(settemp.user > 1)
  settemp.user = 1;

  settemp.user = SetString(settemp,5,32);

  Rainfall.Type = settemp.user;
  WriteUInt8(Rainfall.Type,EEPROM.RainType);//保存设置值

}

void SetYLJFBL(void)
{
  setnum num={Rainfall.Resolution,0X21,0,"mm"};
  num.max = 99;
  num.min = 0;

  DisplayString(2,8,"雨量计分辨力",0);
  H_LINE(2,49,12);
  Rainfall.Resolution = (unsigned char)SetNum(num,6,32);
  WriteUInt8(Rainfall.Resolution,EEPROM.RainFBL);//保存设置值
}

void SetRainJBSD(void)
{
  uint8_t temp[4]={0,5,10,30};
  uint8_t i=0;

  setstring settemp={0,{" NONE"," 5分钟","10分钟","30分钟",""}};

  DisplayString(2,8,"雨量加报时段",0);

  H_LINE(2,49,12);
  for(i=0;i<4;i++)                      //超出范围默认为 5分钟
  {
    if(Rainfall.YLJBSD==temp[i])
      break;
  }
  if(i >= 4)
    i = 1;

  settemp.user = i;
  settemp.user = SetString(settemp,5,32);
  Rainfall.YLJBSD = temp[settemp.user];
  WriteUInt8(Rainfall.YLJBSD ,EEPROM.RainJBSD);//保存设置值
}


void SetYLJBFZ(void)
{
  setnum num={Rainfall.JBFZ,0x51,0,"mm"};
  num.max = 65535;
  num.min = 0;

  DisplayString(3,8,"雨量加报阀值",0);
  H_LINE(2,49,12);
  Rainfall.JBFZ = (unsigned int)SetNum(num,5,32);
  WriteUInt16(Rainfall.JBFZ,EEPROM.RainJBFZ);//保存设置值
}

void SetSWJZ(void)
{
  setnum num={RTU.ShuiWeiJiZhi,0X73,Symbol,"m"};
  num.max = 9999999;
  num.min = -9999999;

  DisplayString(4,8,"水位基值",0);
  H_LINE(2,49,12);
  RTU.ShuiWeiJiZhi = SetNum(num,3,32);
  WriteSInt32(RTU.ShuiWeiJiZhi,EEPROM.ShuiWeiJiZhi); //存储设置值
}

void SetSWXZJZ(void)
{
  setnum num={RTU.XiuZhengJiZhi,0X53,Symbol,"m"};
  num.max = 99999;
  num.min = -99999;

  DisplayString(2,8,"水位修正基值",0);
  H_LINE(2,49,12);
  RTU.XiuZhengJiZhi = SetNum(num,4,32);
  WriteSInt32(RTU.XiuZhengJiZhi,EEPROM.XiuZhengJiZhi); //存储设置值
}

void SetDTKEY_UPower(void)
{
  setstring settemp ={Com.PowerType,{"常关","常开","自动",""}};

  DisplayString(4,8,"通信电源",0);
  H_LINE(2,49,12);
  if(settemp.user>2)  //超出范围 恢复默认值 自动
  {
    settemp.user = 2;
  }
  Com.PowerType = SetString(settemp,6,32);
  WriteUInt8(Com.PowerType,EEPROM.ComPowerType);//保存设置值
  if(Com.PowerType!=PAUTO)
  {
    OperComPort(&Com);
  }
}

void SetDTKEY_UPreheat(void)
{
  setnum num={Com.Preheat,0X30,0,"S"};
  num.max = 255;
  num.min = 0;

  DisplayString(4,8,"通信预热",0);
  H_LINE(2,49,12);
  Com.Preheat = (unsigned char)SetNum(num,6,32);
  WriteUInt8(Com.Preheat,EEPROM.ComPreheat);//保存设置值
}

void SetDTUCloseDelay(void)
{
  setnum num={Com.CloseDelay,0X30,0,"S"};
  num.max = 255;
  num.min = 0;

  DisplayString(4,8,"掉电延时",0);
  H_LINE(2,49,12);
  Com.CloseDelay = (unsigned char)SetNum(num,6,32);
  WriteUInt8(Com.CloseDelay,EEPROM.ComCloseDelay);//保存设置值
}

void SetDTUTxdTime(void)
{
  setnum num={Com.TxdTime,0X20,0,"S"};
  num.max = 20;
  num.min = 0;

  DisplayString(4,8,"超时时间",0);
  H_LINE(2,49,12);
  Com.TxdTime = (unsigned char)SetNum(num,6,32);
  WriteUInt8(Com.TxdTime,EEPROM.ComTxdTime);//保存设置值
}

void SetDTUTxdMax(void)
{
  setnum num={Com.TxdMax,0X10,0," T"};
  num.max = 9;
  num.min = 0;

  DisplayString(4,8,"重发次数",0);
  H_LINE(2,49,12);
  Com.TxdMax = (unsigned char)SetNum(num,6,32);
  WriteUInt8(Com.TxdMax,EEPROM.TxdMax);//保存设置值
}

void SetDTUbps(void)
{
  setstring settemp ={Com.bps,{"1200","2400","4800","9600","19200","38400","57600","115200",""}};

  DisplayString(3,8,"通信波特率",0);
  H_LINE(2,49,12);

  if(settemp.user>7) //超出范围 恢复默认值 9600
  {
    settemp.user = 3;
  }

  Com.bps = SetString(settemp,6,32);
  IniUart0(Com.bps); //初始化通信串口
  WriteUInt8(Com.bps,EEPROM.ComBps);//保存设置值
}

void SetWaterChaliang(void)
{
  setnum num={RTU.WaterChaLiang,0X53,0,"m"};
  num.max = 50000;
  num.min = 0;

  DisplayString(4,8,"水位变率",0);
  H_LINE(2,49,12);
  RTU.WaterChaLiang = (unsigned int)SetNum(num,6,32);
  WriteUInt16(RTU.WaterChaLiang,EEPROM.WaterChaLiang); //存储设置值
}

/*设置雨量清零*/
void SetRainClear(void)
{
  uint8_t i=0;

  DisplayString(4,10,"雨量清零?",0);

  while(1)
  {
    if(1==i)
      DisplayString(5,30,"是  否",BITF+BITE);
    else
      DisplayString(5,30,"是  否",BITB+BITA);

    WaitKey();  //等待按键按下
    switch(key_event_handle)
    {
      case KEY_RIGHT:
      case KEY_LEFT: i = i==1?0:1;break;
      case KEY_SET: if(1==i)
                    RainfallClear();
      case KEY_RETURN: return;
      default :break;
    }
    if(KEY_TIME_OUT)
      return;
  }
}

void SetRS485bps(void)
{
  setstring settemp ={RS485Com.bps,{"1200","2400","4800","9600","19200","38400","57600","115200",""}};

  DisplayString(3,8,"采样波特率",0);
  H_LINE(2,49,12);

  if(settemp.user>7) //超出范围 恢复默认值 9600
  {
    settemp.user = 3;
  }

  RS485Com.bps = SetString(settemp,6,32);
  IniUart1(RS485Com.bps); //初始化通信串口
  WriteUInt8(RS485Com.bps,EEPROM.RS485Bps);//保存设置值
}

void SetRS485Power(void)
{
  setstring settemp ={RS485Com.PowerType,{"常关","常开","自动",""}};

  DisplayString(4,8,"采样电源",0);
  H_LINE(2,49,12);
  if(settemp.user>2)  //超出范围 恢复默认值 自动
  {
    settemp.user = 2;
  }
  RS485Com.PowerType = SetString(settemp,6,32);
  WriteUInt8(RS485Com.PowerType,EEPROM.RS485PowerType);//保存设置值
  if(RS485Com.PowerType!=PAUTO)
  {
    OperComPort(&RS485Com);
  }
}

void SetRS485Preheat(void)
{
  setnum num={RS485Com.Preheat,0X30,0,"S"};
  num.max = 255;
  num.min = 0;

  DisplayString(4,8,"采样预热",0);
  H_LINE(2,49,12);
  RS485Com.Preheat = (unsigned char)SetNum(num,6,32);
  WriteUInt8(RS485Com.Preheat,EEPROM.RS485Preheat);//保存设置值
}

void SetRS485CgqType(void)
{
  setnum num={RS485.CgqType,0X30,0,""};
  num.max = 255;
  num.min = 0;

  DisplayString(3,8,"传感器类型",0);
  H_LINE(2,49,12);
  RS485.CgqType = (unsigned char)SetNum(num,6,32);
  WriteUInt8(RS485.CgqType,EEPROM.RS485CgqType);//保存设置值
}

void SetRS485Name(void)
{
  DisplayString(4,8,"测量要素",0);
  H_LINE(2,49,12);
  RS485.Name = SetYaoSu(RS485.Name,6,32);
  WriteUInt8(RS485.Name,EEPROM.RS485Name);
}

void SetTongDaoName_1(void)
{
  DisplayString(4,8,"测量要素",0);
  H_LINE(2,49,12);
  Analog1.Name = SetYaoSu(Analog1.Name,6,32);
  WriteUInt8(Analog1.Name,EEPROM.TD1name);
}

void SetTongDaoName_2(void)
{
  DisplayString(4,8,"测量要素",0);
  H_LINE(2,49,12);
  Analog2.Name = SetYaoSu(Analog2.Name,6,32);
  WriteUInt8(Analog2.Name,EEPROM.TD2name);
}

/*void SetTongDaoName_3(void)
{
  DisplayString(4,8,"测量要素",0);
  H_LINE(2,49,12);
  Analog3.Name = SetYaoSu(Analog3.Name,6,32);
  WriteUInt8(Analog3.Name,EEPROM.TD3name);
}

void SetTongDaoName_4(void)
{
  DisplayString(4,8,"测量要素",0);
  H_LINE(2,49,12);
  Analog4.Name = SetYaoSu(Analog4.Name,6,32);
  WriteUInt8(Analog4.Name,EEPROM.TD4name);
}*/

void SetTongDaoSType_1(void)
{
  setstring settemp ={Analog1.SType,{"4-20mA","0-2V",""}};

  DisplayString(4,8,"输入信号",0);
  H_LINE(2,49,12);
  if(settemp.user>1)    //超出范围 恢复默认值4-20mA
  {
    settemp.user = 0;
  }
  Analog1.SType = SetString(settemp,6,32);
  WriteUInt8(Analog1.SType,EEPROM.TD1stype);//保存设置值
}

void SetTongDaoSType_2(void)
{
  setstring settemp ={Analog2.SType,{"4-20mA","0-2V",""}};

  DisplayString(4,8,"输入信号",0);
  H_LINE(2,49,12);
  if(settemp.user>1)    //超出范围 恢复默认值4-20mA
  {
    settemp.user = 0;
  }
  Analog2.SType = SetString(settemp,6,32);
  WriteUInt8(Analog2.SType,EEPROM.TD2stype);//保存设置值
}

/*void SetTongDaoSType_3(void)
{
  setstring settemp ={Analog3.SType,{"4-20mA","0-2V",""}};

  DisplayString(4,8,"输入信号",0);
  H_LINE(2,49,12);
  if(settemp.user>1)    //超出范围 恢复默认值4-20mA
  {
    settemp.user = 0;
  }
  Analog3.SType = SetString(settemp,6,32);
  WriteUInt8(Analog3.SType,EEPROM.TD3stype);//保存设置值
}

void SetTongDaoSType_4(void)
{
  setstring settemp ={Analog4.SType,{"4-20mA","0-2V",""}};

  DisplayString(4,8,"输入信号",0);
  H_LINE(2,49,12);
  if(settemp.user>1)    //超出范围 恢复默认值4-20mA
  {
    settemp.user = 0;
  }
  Analog4.SType = SetString(settemp,6,32);
  WriteUInt8(Analog4.SType,EEPROM.TD4stype);//保存设置值
}*/

void SetTongDaoMax_1(void)
{
  setnum num={Analog1.Max,YaoSuInfo[Analog1.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最大值",0);
  H_LINE(2,49,12);
  Analog1.Max = SetNum(num,4,32);
  WriteSInt32(Analog1.Max,EEPROM.TD1max);//保存设置值
}

void SetTongDaoMax_2(void)
{
  setnum num={Analog2.Max,YaoSuInfo[Analog2.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最大值",0);
  H_LINE(2,49,12);
  Analog2.Max = SetNum(num,4,32);
  WriteSInt32(Analog2.Max,EEPROM.TD2max);//保存设置值
}

/*void SetTongDaoMax_3(void)
{
  setnum num={Analog3.Max,YaoSuInfo[Analog3.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最大值",0);
  H_LINE(2,49,12);
  Analog3.Max = SetNum(num,4,32);
  WriteSInt32(Analog3.Max,EEPROM.TD3max);//保存设置值
}

void SetTongDaoMax_4(void)
{
  setnum num={Analog4.Max,YaoSuInfo[Analog4.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最大值",0);
  H_LINE(2,49,12);
  Analog4.Max = SetNum(num,4,32);
  WriteSInt32(Analog4.Max,EEPROM.TD4max);//保存设置值
}*/

void SetTongDaoMin_1(void)
{
  setnum num={Analog1.Min,YaoSuInfo[Analog1.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最小值",0);
  H_LINE(2,49,12);
  Analog1.Min = SetNum(num,4,32);
  WriteSInt32(Analog1.Min,EEPROM.TD1min);//保存设置值
}

void SetTongDaoMin_2(void)
{
  setnum num={Analog2.Min,YaoSuInfo[Analog2.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最小值",0);
  H_LINE(2,49,12);
  Analog2.Min = SetNum(num,4,32);
  WriteSInt32(Analog2.Min,EEPROM.TD2min);//保存设置值
}

/*void SetTongDaoMin_3(void)
{
  setnum num={Analog3.Min,YaoSuInfo[Analog3.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最小值",0);
  H_LINE(2,49,12);
  Analog3.Min = SetNum(num,4,32);
  WriteSInt32(Analog3.Min,EEPROM.TD3min);//保存设置值
}

void SetTongDaoMin_4(void)
{
  setnum num={Analog4.Min,YaoSuInfo[Analog4.Name].DataType,Symbol,""};
  num.max = 99999999;
  num.min = -99999999;

  DisplayString(3,8,"量程最小值",0);
  H_LINE(2,49,12);
  Analog4.Min = SetNum(num,4,32);
  WriteSInt32(Analog4.Min,EEPROM.TD4min);//保存设置值
}
*/
/*显示系统信息*/
void SysInfo(void)
{
  DisplayString(2,8,Model,0);
  DisplayString(2,28,Version,0);
  WaitKey();  //等待按键按下
}

/*设置恢复出厂默认设置*/
void SetResetFactory(void)
{
  uint8_t i=0;

  DisplayString(2,10,"恢复出厂设置?",0);

  while(1)
  {
    if(1==i)
      DisplayString(5,30,"是  否",BITF+BITE);
    else
      DisplayString(5,30,"是  否",BITB+BITA);

    WaitKey();  //等待按键按下
    switch(key_event_handle)
    {
      case KEY_RIGHT:
      case KEY_LEFT: i = i==1?0:1;break;
      case KEY_SET: if(1==i)
                  {
                    DefaultConfig();
                    RainfallClear();
                    WriteConfig();
                  }
      case KEY_RETURN: return;
      default :break;
    }
    if(KEY_TIME_OUT)
      return;
  }
}


/*设置指定类型时间*/
void settimetype(uint8_t type,uint8_t H,uint8_t Ver)
{
  unsigned int  con=0X8000;     //反白控制
  uint8_t  i;             //设置位选择
  uint8_t  max;           //能够设置的最大值
  uint8_t  min;           //能够设置的最小值
  uint8_t  vale;          //设置项当前值
  uint8_t  valetemp;      //设置项值改变后存储

  ReadTime(&System.Time);                       //更新当前时间
  switch(type)  //初始化设置项
  {
    case H_YEAR:  max = 99; min = 0; vale = System.Time.H_YEAR;
          break;
    case H_MONTH:  max = 12; min = 1; vale = System.Time.H_MONTH;
          break;
    case H_DAY:  max = TimeH_DAYmax(System.Time); min = 1;vale = System.Time.H_DAY;
          break;
    case H_HOUR:  max = 23; min = 0; vale = System.Time.H_HOUR;
          break;
    case H_MINUTE:max = 59; min = 0; vale = System.Time.H_MINUTE;
          break;
    case H_SECOND:max = 59; min = 0; vale = System.Time.H_SECOND;
          break;
    default :break;

  }
  valetemp = vale;    //待设置的值初始化为当前值
  i = 1;              //初始化设置位置为十位
  while(1)
  {
    if(1==i)
      con = BITF;
    else
      con = BITE;

    if(valetemp > max) //设置值超过范围处理
    {
      valetemp = min;
    }
    else if(valetemp < min)
    {
      valetemp = max;
    }

    DisplayNum(H,Ver,valetemp,0X20,Zero,"",con);   //显示设置值

    WaitKey();     //等待按键按下

      switch(key_event_handle)
      {
        case KEY_SET:  PCF8563writeData(type,(((valetemp/10)<<4) + (valetemp%10)));   //设置时钟
              DisplayNum(H,Ver,valetemp,0X20,Zero,"",0);    //显示设置后的值
              ReadTime(&System.Time);
              return ;

        case KEY_RETURN:  DisplayNum(H,Ver,vale,0X20,Zero,"",0);     //没有确认输入 显示初始值
              return ;

        case KEY_UP:  if(1 == i)          //设置值增加
                valetemp += 10;
              else
                valetemp++;
              break;

        case KEY_DOWN:  if(valetemp == 0)//设置值减小
              {
                valetemp = max;      //设置值为0时 再减小则变为最大值
              }
              else
              {
                if(1 == i)           //当前选择为十位时
                {
                  if(valetemp < 10)  //设置值小于10
                  {
                    valetemp = max;    //再减小则变为最大值
                  }
                  else
                    valetemp -= 10;     //设置值减小10
                }
                else
                  valetemp--;           //当前选择为个位时 设置值减小1
              }
              break;

        case KEY_LEFT:  if(--i < 1) i = 2;     //用户选择左移
              break;

        case KEY_RIGHT:  if(++i > 2) i = 1;    //用户选择右移
              break;
        default :break;
      }
      if(KEY_TIME_OUT)       //等待按键超时
        return;
  }
}

/*时钟设置*/
void SetClock(void)
{
  uint8_t i;
  uint8_t UserChoose=1;  //用户选择

  ReadTime(&System.Time);                       //更新当前时间

  DisplayNum(5,8,System.Time.H_YEAR,0X20,Zero,"",0);   //初始化显示待设置的时间
  DisplayNum(5,24,System.Time.H_MONTH,0X20,Zero,"",0);
  DisplayNum(5,40,System.Time.H_DAY,0X20,Zero,"",0);
  DisplayNum(13,8,System.Time.H_HOUR,0X20,Zero,"",0);
  DisplayNum(13,24,System.Time.H_MINUTE,0X20,Zero,"",0);
  DisplayNum(13,40,System.Time.H_SECOND,0X20,Zero,"",0);

  while(1)
  {
    for(i=1;i<7;i++)    //用户选择显示
    {
      if(i == UserChoose)     //当显示用户选择项时
      {
        DisplayString(TimeSet[i-1].stringadd_x,TimeSet[i-1].stringadd_y,TimeSet[i-1].string,0XC000); // 反显
      }
      else
      {
        DisplayString(TimeSet[i-1].stringadd_x,TimeSet[i-1].stringadd_y,TimeSet[i-1].string,0);   //正常显示
      }
    }

    WaitKey();     //等待按键按下

      switch(key_event_handle)
      {         //确认输入 进入具体设置项
        case KEY_SET: (*TimeSet[UserChoose-1].Subs)(TimeSet[UserChoose-1].type,TimeSet[UserChoose-1].disadd_x,TimeSet[UserChoose-1].disadd_y);
              break;
        case KEY_RETURN: return;         //退出设置
        case KEY_DOWN:  if(++UserChoose > 6) UserChoose = 1;   break;   //用户选择递增
        case KEY_UP:  if(--UserChoose < 1) UserChoose = 6;   break;   //用户选择递减
        case KEY_RIGHT:

        case KEY_LEFT:  if(UserChoose > 3)      //用户跳跃左右选择
              {
                UserChoose -=3;
              }
              else
              {
                 UserChoose +=3;
              }
              break;
        default :break;
      }
      if(KEY_TIME_OUT)  //等待按键超时
      {
        lcd_clear_ddram();
        return;
      }
  }
}

/*设置安装高度*/
void SetHeight(void)
{
  setnum num={Height,0X53,0,"m"};
  num.max = 50000;
  num.min = 0;

  lcd_clear_ddram();
  DisplayString(4,8,"安装高度",0);
  H_LINE(2,49,12);
  Height = (unsigned int)SetNum(num,5,32);
  WriteUInt16(Height,EEPROM.AnZhuangGaoDu);//保存设置值
  lcd_clear_ddram();
}

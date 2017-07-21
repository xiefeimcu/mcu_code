/*********************************************************
*文件名称：Menu.c
*摘    要：菜单设置功能定义
* 
*作    者： 赵阳
*开始日期： 2013年10月11日 
*完成日期：
**********************************************************/

#include "menu.h" 

const struct Menu MainMenu[5]={            //主菜单 
{5,4,"系统设置",Null,(struct Menu*)SysSetMenu,Null},
{5,4,"报警设置",AlarmValueSet,Null,Null},
{5,4,"系统信息",SysInfo,Null,Null},
{5,4,"恢复出厂",SetResetFactory,Null,Null},
{5,4,"系统自检",SystemSelfTestMenu,Null,Null},
};

const struct Menu SysSetMenu[3]={            //系统设置 
{3,4,"翻斗分辨力",SetYLJFBL,Null,(struct Menu*)MainMenu},
{3,4,"时间日期",SetClock,Null,(struct Menu*)MainMenu},
{3,4,"雨量清零",SetRainClear,Null,(struct Menu*)MainMenu},
};

const struct TimeSetMenu TimeSet[6]={   //时间设置
{year,2,8,"年",5,8,settimetype},
{month,2,24,"月",5,24,settimetype},
{day,2,40,"日",5,40,settimetype},
{hour,10,8,"时",13,8,settimetype},
{minute,10,24,"分",13,24,settimetype},
{second,10,40,"秒",13,40,settimetype}
};

const AlarmSetClass AlarmSet[11]={
{&Alarm.PN05,AT_PN05I,AT_PN05II,AT_PN05III},
{&Alarm.PN10,AT_PN10I,AT_PN10II,AT_PN10III},
{&Alarm.PN30,AT_PN30I,AT_PN30II,AT_PN30III},
{&Alarm.P1,AT_P1I,AT_P1II,AT_P1III},
{&Alarm.P3,AT_P3I,AT_P3II,AT_P3III},
{&Alarm.P6,AT_P6I,AT_P6II,AT_P6III},
{&Alarm.P12,AT_P12I,AT_P12II,AT_P12III},
{&Alarm.P24,AT_P24I,AT_P24II,AT_P24III},
{&Alarm.PJ,AT_PJI,AT_PJII,AT_PJIII},
{&Alarm.P,AT_PI,AT_PII,AT_PIII},
{&Alarm.PT,AT_PTI,AT_PTII,AT_PTIII}
};

/*报警设置*/
void AlarmValueSet(void)
{
  char x=0;
  char y=1;

  setnum num={0,0X51,0,"mm",50000,0};
  
  ClearScreen;       //清屏
  while(True)
  {
    
    Draw(0,1,2,9,0,leftarrow);//左箭头
    Draw(14,1,2,9,0,rightarrow);//右箭头
    switch(x)
    {
      case 0: DisplayString(3,0," 5分钟雨量",0); break;
      case 1: DisplayString(3,0,"10分钟雨量",0); break;
      case 2: DisplayString(3,0,"30分钟雨量",0); break;
      case 3: DisplayString(3,0," 1小时雨量",0); break;
      case 4: DisplayString(3,0," 3小时雨量",0); break;
      case 5: DisplayString(3,0," 6小时雨量",0); break; 
      case 6: DisplayString(3,0,"12小时雨量",0); break; 
      case 7: DisplayString(3,0,"24小时雨量",0); break;
      case 8: DisplayString(3,0," 今日雨量 ",0); break;
      case 9: DisplayString(3,0," 本场雨量 ",0); break;
      case 10: DisplayString(3,0," 累计雨量 ",0); break;  
    }
    DisplayString(0,16,"立即转移",0);
    DisplayString(0,32,"准备转移",0);
    DisplayString(0,48,"警戒报警",0);
    
    switch(y)
    {
      case 1:
        {
          DisplayString(0,16,"立即转移",0XFFFF);
        }
        break;
      case 2:
        {
          DisplayString(0,32,"准备转移",0XFFFF);
        }
        break;
      case 3:
        {
          DisplayString(0,48,"警戒报警",0XFFFF);         
        }
        break;
    }
    DisplayNum(8,16,AlarmSet[x].pA->I,0X51,space,"mm",0);
    DisplayNum(8,32,AlarmSet[x].pA->II,0X51,space,"mm",0);
    DisplayNum(8,48,AlarmSet[x].pA->III,0X51,space,"mm",0);
    
    WaitKey();
    
    switch(System.Key)
    {
      case Up:  
        {
          if(--y < 1)
            y = 3;
        }
        break;
      case Down: 
        {
          if(++y > 3)
            y = 1;
        }
        break;
      case Right: 
        {
          if(++x > 10)
            x = 0;
        }
        break;
      case Left: 
        {
          if(0==x)
            x = 10;
          else
            x--;
        }
        break;
      case Esc: 
        {
          return;
        }
        break;
      case Enter: 
        {
          if(1==y)
          {
            num.value = AlarmSet[x].pA->I;
            AlarmSet[x].pA->I = (unsigned int)SetNum(num,8,16);
            WriteUInt16(AlarmSet[x].pA->I,AlarmSet[x].at_i,0);
          }
          else if(2==y)
          {
            num.value = AlarmSet[x].pA->II;
            AlarmSet[x].pA->II = (unsigned int)SetNum(num,8,32);
            WriteUInt16(AlarmSet[x].pA->II,AlarmSet[x].at_ii,0);
          }
          else if(3==y)
          {
            num.value = AlarmSet[x].pA->III;
            AlarmSet[x].pA->III = (unsigned int)SetNum(num,8,48);
            WriteUInt16(AlarmSet[x].pA->III,AlarmSet[x].at_iii,0);
          }
        }
        break;
    }
    if(Display.Status.Flag.KeyOut)
      return; 
    
  }
  
}




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
  ClearScreen;
  if(0!=PassWord())
  {
    SetMenu();  
  }
  SC_Reset();
  System.Key = Null;
  ReadTime(&System.Time); 
  ClearScreen;
  DisplayMon();      //显示监测界面
  Display.Status.Flag.KeyOut = RESET;
  Display.Status.Flag.SetMode = RESET;

}

/*******************************************************************************
函数名称：void ShowMenu(Menu *MenuUser,char DisplayPoint,char refresh)
函数介绍：显示菜单
输入参数：Menu *MenuUser---->显示菜单地址
          DisplayPoint----->显示点
          UserChoose------->当前用户选择
          USER------------->前一次用户选择
          refresh---------->全部刷新标志
输出参数：无
返回值  ：无
*******************************************************************************/
void ShowMenu(struct Menu *MenuUser,char DisplayPoint,signed char USER,signed char UserChoose,char refresh)
{
    char i=0;
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
  char i=0;                    //菜单层级
  signed char UserChoose = 0;           //当前用户选择
  signed char USER=0;                   //前一次用户选择 
  char  DisplayPoint = 0;      //显示点
  char  RefreshMenu = 1;       //刷新菜单标志
  struct Menu *MenuPoint = (struct Menu*)MainMenu; 
  
  ClearScreen;   
  while(1)
  {      
    ShowMenu(MenuPoint,DisplayPoint,USER,UserChoose,RefreshMenu);         //显示目录
  
    WaitKey();     //等待按键按下 
     
      switch(System.Key)
      {
        case Up:
                USER = UserChoose;                      //保存前一次用户选择
                UserChoose--;                           //用户选择上移
                if(UserChoose < 0)                      //当前用户选择为第一项时 再向上选择
                {
                  UserChoose = MenuPoint[0].MenuCount - 1;           //用户选择指向最后一项
                  if(MenuPoint[0].MenuCount > 4)                     //菜单长度大于显示屏最大行数
                  {
                    ClearScreen;                      //清屏
                    RefreshMenu = 1;
                  }
                }
                else if(((UserChoose+1)%4)==0)          //向上翻页
                {
                  ClearScreen; 
                  RefreshMenu = 1;
                }
                break;   
        case Down:                                      //用户向下选择
                USER = UserChoose;                      //保存前一次用户选择
                UserChoose++;
                if(UserChoose >= MenuPoint[0].MenuCount)             //用户选择大于菜单长度
                {
                  UserChoose = 0;                       //用户选择指向首项
                  if(MenuPoint[0].MenuCount > 4) 
                  {
                    ClearScreen;
                    RefreshMenu = 1;
                  }
                }
                break;
    
        case Esc:                                       //返回上级菜单
                if(MenuPoint == MainMenu)               //当前菜单为主菜单时 退出菜单
                {   
                  return;
                }    
                else if(MenuPoint[0].PreviousMenu != Null)
                {
                  MenuPoint = MenuPoint[0].PreviousMenu;
                  UserChoose = user[--i]; //恢复上级菜单用户选择
                  ClearScreen;
                  RefreshMenu = 1;
                }    
                break;
    
        case Enter:                                             //进入下一级菜单或设置                            
                if(MenuPoint[UserChoose].Subs != Null)          //进入对应设置
                {
                  ClearScreen;
                  (*MenuPoint[UserChoose].Subs)();
                }
                else if(MenuPoint[UserChoose].SubMenu != Null)  //进入下一级菜单
                {
                  MenuPoint=MenuPoint[UserChoose].SubMenu;
                  user[i++] = UserChoose;     //保存当前菜单的用户选择
                  UserChoose = 0;                                                     
                } 
                RefreshMenu = 1;
                ClearScreen;
                break;      
      }                     
      
    DisplayPoint = UserChoose-UserChoose%4;     // 4 指屏幕显示最大行数
    if((UserChoose+USER)%8 == 7)                // 上下翻页时清屏
    {
      ClearScreen;
      RefreshMenu = 1;
    }
    if(Display.Status.Flag.KeyOut)              //等待按键超时 返回监测界面
    {  
      Display.Status.Flag.KeyOut = False;
      ClearScreen;
      return;
    }      
  }    
}


/*等待按键*/
void WaitKey(void)
{
  unsigned int i=0;
  
  System.Key = Null; 
  while(!System.Key)
  {
    delayms(1);
    if(i++ > 30000)
    {
      Display.Status.Flag.KeyOut = True;
      System.Key = Null;
      return;
    }      
  }
}

/*******************************************************************************
函数名称：long int SetNum(setnum num,char H,char Ver)
函数介绍：设置数字
输入参数：setnum num----->设置数的数据结构 定义在menu.h
          H-------------->显示水平坐标
          Ver------------>显示垂直坐标
输出参数：无
返回值  ： 用户输入的值

*******************************************************************************/
long int SetNum(setnum num,char H,char Ver)
{
  char strnum[15]={0};
  char i=0,j=0,k=0;
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
    
    switch(System.Key)
    {
      case Up:  if('+'==strnum[user])
                  strnum[user] = '-';
                else if(('-'==strnum[user]))
                  strnum[user] = '+';
                else if(++strnum[user]>'9')
                  strnum[user] = '0';
                break;
      case Down: if('+'==strnum[user])
                   strnum[user] = '-';
                 else if(('-'==strnum[user]))
                   strnum[user] = '+';
                 else if(--strnum[user]<'0')
                   strnum[user] = '9';
                 break;
      case Right: user++;
                  if('.'==strnum[user])
                    user++;
                  else if((strnum[user]>'9')||(strnum[user]<'0'))
                    user = 0;
                  break;
      case Left:  user--;
                  if(user<0)
                    user = k-1;
                  else if('.'==strnum[user])
                    user--;
                  break;
      case Esc:   return num.value;
      case Enter:  
                  num.value = String_TO_SInt32(strnum);
                  Draw(H,Ver,16,16,0,Null);
                  if(num.value > num.max)
                    num.value = num.max;
                  else if(num.value < num.min)
                    num.value = num.min;
                  return num.value;
    }
    
    if(Display.Status.Flag.KeyOut)    //等待按键超时
      return num.value;
  }
}

/*******************************************************************************
函数名称：char SetString(setstring s,char H,char Ver)
函数介绍：选择字符
输入参数：setstring s----->提供的待选项
          H--------------->水平显示地址
          Ver------------->垂直显示地址
输出参数：无
返回值  ：用户选择项编号
*******************************************************************************/
char SetString(setstring s,char H,char Ver)
{
  char i=0,j=0;
  
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
    
    switch(System.Key)
    {
      case Up:  Draw(H,Ver,strlenght(s.str[s.user]),16,0,Null);
                s.user++;
                if(s.user > i)
                  s.user = 0;               
                break;
      case Down: Draw(H,Ver,strlenght(s.str[s.user]),16,0,Null);
                if(0==s.user)
                   s.user = i; 
                 else
                   s.user--;                                            
                 break;
      case Esc: return j;
      case Enter:    
                return s.user;
    }
    if(Display.Status.Flag.KeyOut)
      return j;  
  } 
}


char PassWord(void)
{
  long x=0;
  setnum num={0,0X60,0,"\0"};
  
  DisplayString(4,16,"输入密码",0);
         
  num.max = 9999999;
  num.min = 0; 
  x = SetNum(num,5,32);
    
  if(Display.Status.Flag.KeyOut)
      return 0;    
  if(UserPass == x)
    return 2;
  if(AdminPass == x)
    return 1;
  
  return 0;
}

/*设置指定类型时间*/
void settimetype(char type,char H,char Ver)
{
  unsigned int  con=0X8000;     //反白控制
  char  i;             //设置位选择  
  char  max;           //能够设置的最大值
  char  min;           //能够设置的最小值
  char  vale;          //设置项当前值
  char  valetemp;      //设置项值改变后存储

  ReadTime(&System.Time);                       //更新当前时间  
  switch(type)  //初始化设置项  
  {
    case year:  max = 99; min = 0; vale = System.Time.Year;
          break;
    case month:  max = 12; min = 1; vale = System.Time.Month;
          break;
    case day:  max = Timedaymax(System.Time); min = 1;vale = System.Time.Day;                              
          break;
    case hour:  max = 23; min = 0; vale = System.Time.Hour;
          break;
    case minute:max = 59; min = 0; vale = System.Time.Minute;
          break;
    case second:max = 59; min = 0; vale = System.Time.Second;
          break;          
    
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
    
      switch(System.Key)
      {
        case Enter:  PCF8563writeData(type,(((valetemp/10)<<4) + (valetemp%10)));   //设置时钟
              DisplayNum(H,Ver,valetemp,0X20,Zero,"",0);    //显示设置后的值            
              return ;
            
        case Esc:  DisplayNum(H,Ver,vale,0X20,Zero,"",0);     //没有确认输入 显示初始值
              return ;
              
        case Up:  if(1 == i)          //设置值增加    
                valetemp += 10;
              else
                valetemp++;
              break;

        case Down:  if(valetemp == 0)      //设置值减小
              {
                valetemp = max;      //设置值为0时 再减小则变为最大值
              }
              else
              {
                if(1 == i)       //当前选择为十位时
                {
                  if(valetemp < 10)  //设置值小于10
                  {
                    valetemp = max;    //再减小则变为最大值
                  }
                  else
                    valetemp -= 10;     //设置值减小10
                }
                else
                  valetemp--;      //当前选择为个位时 设置值减小1
              }            
              break;

        case Left:  if(--i < 1) i = 2;     //用户选择左移
              break;

        case Right:  if(++i > 2) i = 1;    //用户选择右移            
              break;
      }
      if(Display.Status.Flag.KeyOut)  //等待按键超时
        return;
  }   
}

/*时钟设置*/
void SetClock(void)
{
  char i;
  char UserChoose=1;  //用户选择
  
  ReadTime(&System.Time);                       //更新当前时间  
  
  DisplayNum(5,8,System.Time.Year,0X20,Zero,"",0);   //初始化显示待设置的时间
  DisplayNum(5,24,System.Time.Month,0X20,Zero,"",0);
  DisplayNum(5,40,System.Time.Day,0X20,Zero,"",0);
  DisplayNum(13,8,System.Time.Hour,0X20,Zero,"",0);
  DisplayNum(13,24,System.Time.Minute,0X20,Zero,"",0);
  DisplayNum(13,40,System.Time.Second,0X20,Zero,"",0);

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
       
      switch(System.Key)
      {         //确认输入 进入具体设置项
        case Enter: (*TimeSet[UserChoose-1].Subs)(TimeSet[UserChoose-1].type,TimeSet[UserChoose-1].disadd_x,TimeSet[UserChoose-1].disadd_y);    
              
                  break;
        case Esc: return;         //退出设置
        case Down:  if(++UserChoose > 6) UserChoose = 1;   break;   //用户选择递增
        case Up:  if(--UserChoose < 1) UserChoose = 6;   break;   //用户选择递减
        case Right:  
            
        case Left:  if(UserChoose > 3)      //用户跳跃左右选择
              {
                UserChoose -=3;
              }
              else
              { 
                 UserChoose +=3;
              }
              break; 
      }
      if(Display.Status.Flag.KeyOut)  //等待按键超时
      {
        ClearScreen;
        return;
      }
  }
}

/*******************************************************************************
函数名称：void SetYLJFBL(void)
函数介绍：设置雨量计分辨力
输入参数：无
输出参数：雨量计分辨力
返回值  ：无
*******************************************************************************/
void SetYLJFBL(void)
{
  setnum num={Rainfall.Resolution,0X21,0,"mm",99,0}; 
  
  DisplayString(4,8,"翻斗分辨力",0);
  H_LINE(2,49,12);
  Rainfall.Resolution = (char)SetNum(num,6,32);  
  WriteUInt8(Rainfall.Resolution,AT_FBL,0);//保存设置值 
}

/*设置雨量清零*/
void SetRainClear(void)
{
  char i=0;
  
  DisplayString(4,10,"雨量清零?",0);
  
  while(1)
  {
    if(1==i)
      DisplayString(5,30,"是  否",BITF+BITE);
    else
      DisplayString(5,30,"是  否",BITB+BITA);
    
    WaitKey();  //等待按键按下
    switch(System.Key)
    {
      case Right: 
      case Left: i = i==1?0:1;break;
      case Enter: if(1==i)
                  {
                    RainfallClear();
                     
                  }
                    
                  
      case Esc: return;
    }
    if(Display.Status.Flag.KeyOut)
      return;         
  } 
}


/*显示系统信息*/
void SysInfo(void)
{
  DisplayString(1,8,"YZ3000 V2.0",0);
  //DisplayString(1,28,"V2.0_20140918",0);
  WaitKey();  //等待按键按下
}

/*设置恢复出厂默认设置*/
void SetResetFactory(void)
{
  char i=0;
  
  DisplayString(2,10,"恢复出厂设置?",0);
  
  while(1)
  {
    if(1==i)
      DisplayString(5,30,"是  否",BITF+BITE);
    else
      DisplayString(5,30,"是  否",BITB+BITA);
    
    WaitKey();  //等待按键按下
    switch(System.Key)
    {
      case Right: 
      case Left: i = i==1?0:1;break;
      case Enter: if(1==i)
                  {
                    ResetFactory(); 
                  }
                    
      case Esc: return;
    }
    if(Display.Status.Flag.KeyOut)
      return;         
  } 
}

void SystemSelfTestMenu(void)
{
  SystemSelfTest();
  ATcmd_LedTest();
}

/*写入所有配置参数*/
/*void WriteConfig(void)
{
  
}*/

/*恢复出厂默认设置*/
void ResetFactory(void)
{
  RainfallClear();      //雨量清零
  DefaultConfig();      //配置参数恢复为预设值
  
}
/*参数恢复为默认值*/
void DefaultConfig(void)
{
 
  Rainfall.Resolution = 5;
  Alarm.PN05.I = Alarm.PN05.II = Alarm.PN05.III = 0;
  Alarm.PN10.I = Alarm.PN10.II = Alarm.PN10.III = 0;
  Alarm.PN30.I = Alarm.PN30.II = Alarm.PN30.III = 0;
  Alarm.P1.I = Alarm.P1.II = Alarm.P1.III = 0;
  Alarm.P3.I = Alarm.P3.II = Alarm.P3.III = 0;
  Alarm.P6.I = Alarm.P6.II = Alarm.P6.III = 0;
  Alarm.P12.I = Alarm.P12.II = Alarm.P12.III = 0;
  Alarm.P24.I = Alarm.P24.II = Alarm.P24.III = 0;
  Alarm.P.I = Alarm.P.II = Alarm.P.III = 0;
  Alarm.PJ.I = Alarm.PJ.II = Alarm.PJ.III = 0;
  Alarm.PT.I = Alarm.PT.II = Alarm.PT.III = 0;
  Flash.Page = 1;
  Flash.Bytes = 2;
  
  WriteUInt8(Rainfall.Resolution,AT_FBL,0);
  
  WriteUInt16(Alarm.PN05.I,AT_PN05I,0);
  WriteUInt16(Alarm.PN05.II,AT_PN05II,0);
  WriteUInt16(Alarm.PN05.III,AT_PN05III,0);
  WriteUInt16(Alarm.PN10.I,AT_PN10I,0);
  WriteUInt16(Alarm.PN10.II,AT_PN10II,0);
  WriteUInt16(Alarm.PN10.III,AT_PN10III,0);
  WriteUInt16(Alarm.PN30.I,AT_PN30I,0);
  WriteUInt16(Alarm.PN30.II,AT_PN30II,0);
  WriteUInt16(Alarm.PN30.III,AT_PN30III,0);
  WriteUInt16(Alarm.P1.I,AT_P1I,0);
  WriteUInt16(Alarm.P1.II,AT_P1II,0);
  WriteUInt16(Alarm.P1.III,AT_P1III,0);
  WriteUInt16(Alarm.P3.I,AT_P3I,0);
  WriteUInt16(Alarm.P3.II,AT_P3II,0);
  WriteUInt16(Alarm.P3.III,AT_P3III,0);
  WriteUInt16(Alarm.P6.I,AT_P6I,0);
  WriteUInt16(Alarm.P6.II,AT_P6II,0);
  WriteUInt16(Alarm.P6.III,AT_P6III,0);
  WriteUInt16(Alarm.P12.I,AT_P12I,0);
  WriteUInt16(Alarm.P12.II,AT_P12II,0);
  WriteUInt16(Alarm.P12.III,AT_P12III,0);
  WriteUInt16(Alarm.P24.I,AT_P24I,0);
  WriteUInt16(Alarm.P24.II,AT_P24II,0);
  WriteUInt16(Alarm.P24.III,AT_P24III,0);
  WriteUInt16(Alarm.P.I,AT_PI,0);
  WriteUInt16(Alarm.P.II,AT_PII,0);
  WriteUInt16(Alarm.P.III,AT_PIII,0);
  WriteUInt16(Alarm.PJ.I,AT_PJI,0);
  WriteUInt16(Alarm.PJ.II,AT_PJII,0);
  WriteUInt16(Alarm.PJ.III,AT_PJIII,0);
  WriteUInt16(Alarm.PT.I,AT_PTI,0);
  WriteUInt16(Alarm.PT.II,AT_PTII,0);
  WriteUInt16(Alarm.PT.III,AT_PTIII,0);
  WriteUInt16(Flash.Page,AT_PAGE,0);
  WriteUInt16(Flash.Bytes,AT_BYTE,0);
}


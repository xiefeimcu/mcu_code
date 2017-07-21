//---------------------------------------------------------
//文件名称：Rainfall.h
//摘    要：降水量模块声明
//
//作    者：赵阳
//版    本：1.0
//修改日期：2013年11月25日
//---------------------------------------------------------

#ifndef __RAINFALL_H
#define __RAINFALL_H

#define RAINPORT_1              BIT7                                    //雨量1输入端口
#define RAINPORT_2              BIT7                                    //雨量2输入端口
#define RAIN_IES                P2IES
#define RAIN_DIR                P2DIR
#define RAIN_IE                 P2IE
#define RAIN_IN                 P2IN
#define RAININ                  (RAIN_IN & (RAINPORT_1|RAINPORT_2))        //雨量输入
#define RAIN_1                  (RAIN_IN & RAINPORT_1)                     //雨量通道1输入
#define RAIN_2                  (RAIN_IN & RAINPORT_2)                     //雨量通道2输入
#define RAINTIME                10                                      //雨量检测保持时间 x ms   需保持x ms的低电平，雨量输入才判断为有效。
#define RAINJG                  1                                       //雨量输入信号有效间隔 0.5~1s



typedef struct
{
  //unsigned int Count;
  unsigned int PT;              //降水量累计值       N(5,1)  单位 毫米  
  unsigned int PN05;            //5分钟 时段降水量   N(5,1)
  unsigned int PN10;            //10分钟时段降水量   N(5,1)
  unsigned int PN30;            //30分钟时段降水量   N(5,1) 
  unsigned int P1;
  unsigned int P3;
  unsigned int P6;
  unsigned int P12;
  unsigned int P24;
  unsigned int PJ;              //当前降水量
  unsigned int P;
  char Resolution;     //分辨力
  char InDelay;        //输入间隔
}RainfallClass;

extern RainfallClass Rainfall;
extern char RainArray[288];

void RainInit(void);
void SysEventHandlers_Rain(void);
void RainfallClear(void);
void IniRainArray(char value);
void RainArrayRefresh(void);
unsigned int GetRain(unsigned int x);

#endif  /*#ifndef __RAINFALL_H*/
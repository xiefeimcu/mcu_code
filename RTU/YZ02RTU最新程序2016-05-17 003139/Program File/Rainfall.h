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

#define RAINPORT_1              BIT6                                    //雨量1输入端口
#define RAINPORT_2              BIT5                                    //雨量2输入端口
#define RAIN_IES                P1IES
#define RAIN_DIR                P1DIR
#define RAIN_IE                 P1IE
#define RAIN_IN                 P1IN
#define RAININ                  (RAIN_IN & (RAINPORT_1+RAINPORT_2))        //雨量输入
#define RAIN_1                  (RAIN_IN & RAINPORT_1)                     //雨量通道1输入
#define RAIN_2                  (RAIN_IN & RAINPORT_2)                     //雨量通道2输入
#define RAINTIME                10                                      //雨量检测保持时间 x ms   需保持x ms的低电平，雨量输入才判断为有效。
#define RAINJG                  2                                       //雨量输入信号有效间隔 0.5~1s


typedef struct
{
  unsigned int PT;              //降水量累计值       N(5,1)  单位 毫米  
  unsigned int PJ;              //当前降水量
  unsigned int PN05;            //5分钟降水量
  unsigned int PN10;            //10分钟降水量
  unsigned int PN30;            //30分钟降水量
  unsigned int JBFZ;            //加报阀值
  unsigned char YLJBSD;         //降水量加报方式/时段
  unsigned char PDTime;         //降水量日起始时间0~23
  unsigned char Type;           //雨量计类型         0未接 1 翻斗式雨量计
  unsigned char Resolution;     //分辨力
  unsigned char InDelay;        //输入间隔
}RainfallClass;

extern RainfallClass Rainfall;

void RainInit(void);
void SysEventHandlers_Rain(void);
void RainfallClear(void);

#endif  /*#ifndef __RAINFALL_H*/
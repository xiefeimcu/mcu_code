#ifndef RTU_H_
#define RTU_H_
 
#include "MSP430_hardware.h"


#define JD1_ON                  P4DIR |= BIT1;_NOP();P4OUT |= BIT1;     //打开继电器1
#define JD1_OFF                 P4DIR |= BIT1;_NOP();P4OUT &= ~BIT1;    //关闭继电器2
#define JD2_ON                  P4DIR |= BIT0;_NOP();P4OUT |= BIT0;     //打开继电器2
#define JD2_OFF                 P4DIR |= BIT0;_NOP();P4OUT &= ~BIT0;    //关闭继电器2

#define PNC                     0                                       //常闭
#define PNO                     1                                       //常开
#define PAUTO                   2                                       //自动

#define Com0Write(x,y)          WriteUSART0(x,y)                        //端口0发送数据
#define Com1Write(x,y)          WriteUSART1(x,y)                        //端口1发送数据

#define Analog_4ma              740                                     //4maAD测值
#define Analog_20ma             3718                                    //20maAD测值
#define Analog_0V               0                                       //0V AD测值
#define Analog_1V               1241                                    //1V AD测值
#define Analog_2V               2482                                    //2V AD测值

#define STYPE_4_20MA            0                                       //输入信号4-20ma
#define STYPE_0_2V              1                                       //输入信号0-2V

#define TONGDAO_1               6
#define TONGDAO_2               5
#define TONGDAO_3               7
#define TONGDAO_4               6

#define WMODE_1                 1                                       //自报
#define WMODE_2                 2                                       //自报确认
#define WMODE_3                 3                                       //查询应答
#define WMODE_4                 4                                       //调试维修

#define YAOSULENG               11                                      //可选择的要素最大值

typedef struct
{
  long ShuiWeiJiZhi;
  long XiuZhengJiZhi;
  long qwater;
  unsigned int CaiYangJianGe;
  unsigned int CunChuJianGe;
  unsigned int WaterChaLiang;
  unsigned int Voltage; //电压
  int Temp;             //温度 
  unsigned int Password;        //发报密码
  unsigned char ST[5];          //遥测站地址 BCD码存储
  unsigned char CenterAdd;      //中心站地址
  unsigned char RTUType;        //遥测站分类码
  unsigned char DingShiBaoTime;
  unsigned char JiaBaoTime;
  unsigned char WorkMode;

}ClassRTUConfig;
 
/*通信端口数据结构*/
typedef struct
{
  union
  {
    unsigned char value;                //状态标志寄存器
    struct
    {
      unsigned char Power       :1;     //电源状态
      unsigned char Online      :1;     //在线状态
      unsigned char Close       :1;     //电源关闭允许     
      unsigned char Txd         :1;     //正在发送中等待应答标志
      //unsigned char TxdTimeout  :1;     //发送超时标志
      unsigned char ComEvent    :1;     //通信事件标志
      //unsigned char RxdSwitch   :1;     //接收开关
    }Flag;   
  }Status;
  unsigned char PowerType;              //电源控制类型 0常闭 1常开 2自动
  unsigned char Com;                    //端口号
  unsigned char bps;                    //波特率
  unsigned char Preheat;                //预热时间 秒
  unsigned char TiPreheat;              //预热计时
  unsigned char CloseDelay;             //关闭电源延时时间 秒
  unsigned char TiClose;                //关闭电源计时
  unsigned char TxdMax;                 //最大重发次数 
  unsigned char TxdCount;               //发送计数
  unsigned char TxdTime;                //超时重发时间 秒
  unsigned char TiTxdTime;              //超时重发时间计时

}ComPortClass;

/*模拟量通道*/
typedef struct
{
  long Value;      //当前测值
  long Min;       //量程最小值
  long Max;       //量程最大值
  unsigned int ADValue;//AD测值
  unsigned char Name;           //测量要素名称
  unsigned char SType;          //信号类型
   
}ClassAnalog;

/*观测要素*/
typedef struct
{
  unsigned char *NameStr;
  unsigned char DataType;
  unsigned char *Bzf;
}ClassYaoSuInfo;

/*typedef struct
{
  union
  {
    unsigned char Register;
    struct
    {
      unsigned char Event       :1;
      unsigned char Run         :1;
      unsigned char Ok          :1;
    }Bit;
  }Flag;
  unsigned char AFN;
}ClassCaiYang;*/

typedef struct
{
  unsigned char Event;
  unsigned char AFN;
}ClassCaiYang;

typedef struct
{
  unsigned char PT;
  unsigned char PJ;
  unsigned char PN05;
  unsigned char PN10;
  unsigned char PN30;
  unsigned char RainJBFZ;
  unsigned char RainJBSD;
  unsigned char PDTime;
  unsigned char RainType;
  unsigned char RainFBL;
  unsigned char RTUtype;
  unsigned char CenterAdd;
  unsigned char ST;
  unsigned char WorkMode;
  unsigned char DingShiBaoTime;
  unsigned char JiaBaoTime;
  unsigned char CaiYangJianGe;
  unsigned char CunChuJianGe;
  unsigned char ShuiWeiJiZhi;
  unsigned char XiuZhengJiZhi;
  unsigned char ComPowerType;
  unsigned char ComPreheat;
  unsigned char ComCloseDelay;
  unsigned char ComBps;
  unsigned char ComTxdTime;
  unsigned char TxdMax;
  unsigned char WaterChaLiang;
  unsigned char RS485Bps;
  unsigned char RS485PowerType;
  unsigned char RS485Preheat;
  unsigned char RS485CgqType;
  unsigned char RS485Name;
  unsigned char TD1name;
  unsigned char TD1stype;
  unsigned char TD1max;
  unsigned char TD1min;
  unsigned char TD2name;
  unsigned char TD2stype;
  unsigned char TD2max;
  unsigned char TD2min;
  unsigned char TD3name;
  unsigned char TD3stype;
  unsigned char TD3max;
  unsigned char TD3min;
  unsigned char TD4name;
  unsigned char TD4stype;
  unsigned char TD4max;
  unsigned char TD4min;
  unsigned char Page;
  unsigned char Bytes;
  unsigned char AnZhuangGaoDu;
}ClassEEPROM;

extern ClassRTUConfig RTU;
extern const ClassYaoSuInfo YaoSuInfo[];
extern ClassAnalog Analog1;
extern ClassAnalog Analog2;
//extern ClassAnalog Analog3;
//extern ClassAnalog Analog4;
extern const ClassEEPROM EEPROM;
extern ClassCaiYang CaiYang;
extern unsigned int Relvtive;

void TiTimeHandlers(void);
void AnalogCaiYang(void);
long int AnalogCalc(unsigned char type,unsigned int advalue,long int max,long int min);
void OperComPort(ComPortClass *Pcom);
void CloseComPort(ComPortClass *Pcom);
void CaiYangHandlers(void);
void DefaultConfig(void);
void WriteConfig(void);
void AddCaiYangTask(unsigned char afn);
void CaiYangOk(void);
#endif /*#ifndef RTU_H_*/
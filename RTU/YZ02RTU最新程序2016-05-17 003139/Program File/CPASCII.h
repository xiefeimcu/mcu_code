/*********************************************************
*文件名称：CPASCII.h
*摘    要：通信协议编辑报文声明
*
*当前版本：1.0
*作    者：赵阳
*开始日期：2013年09月02日
*完成日期：
**********************************************************/
#ifndef __CPASCII_H
#define __CPASCII_H


/**************************控制字符定义**************************************/
#define         SOH             0X01            //帧起始
#define         STX             0X02            //传输正文起始
#define         SYN             0X16            //多包传输正文起
#define         ETX             0X03            //报文结束，后续无报文
#define         ETB             0X17            //报文结束，后续有报文
#define         ENQ             0X05            //询问
#define         EOT             0X04            //传输结束，退出
#define         ACK             0X06            //肯定确认，继续发送
#define         NAK             0X15            //否定应答，反馈重发
#define         ESC             0X1B            //传输结束，终端保持在线
/**************************遥测站分类码***************************************/
#define         RTUTYPE_JS      'P'             //遥测站分类码 降水 
#define         RTUTYPE_HD      'H'             //遥测站分类码 河道 
#define         RTUTYPE_SK      'K'             //遥测站分类码 水库（湖泊）
#define         RTUTYPE_ZB      'Z'             //遥测站分类码 闸坝
#define         RTUTYPE_BZ      'D'             //遥测站分类码 泵站
#define         RTUTYPE_CX      'T'             //遥测站分类码 潮汐
#define         RTUTYPE_SQ      'M'             //遥测站分类码 墒情
#define         RTUTYPE_DXS     'G'             //遥测站分类码 地下水
#define         RTUTYPE_SZ      'Q'             //遥测站分类码 水质
#define         RTUTYPE_QSK     'I'             //遥测站分类码 取水口
#define         RTUTYPE_PSK     'O'             //遥测站分类码 排水口
/****************************功能码定义***************************************/
#define         GNM_LLWC        0X2F            //链路维持报
#define         GNM_CSB         0X30            //测试报
#define         GNM_JYSD        0X31            //均有时段水文信息报
#define         GNM_DSB         0X32            //遥测站定时报
#define         GNM_JBB         0X33            //遥测站加报报
#define         GNM_XSB         0X34            //遥测站小时报
#define         GNM_RGZS        0X35            //遥测站人工置数报
#define         GNM_TPB         0X36            //遥测图片报或中心站查询遥测站图片采集信息
#define         GNM_CXSS        0X37            //中心站查询遥测站实时数据
#define         GNM_CXSD        0X38            //中心站查询遥测站时段数据
#define         GNM_CXRGZS      0X39            //中心站查询遥测站人工置数
#define         GNM_CXZDYS      0X3A            //中心站查询遥测指定要素数据
#define         GNM_XGJBPZ      0X40            //中心站修改遥测站基本配置表
#define         GNM_DQJBPZ      0X41            //中心站读取遥测站基本配置表/遥测站自报基本配置表
#define         GNM_XGYXCS      0X42            //中心站修改遥测运行参数配置表
#define         GNM_DQYXCS      0X43            //中心站读取遥测运行参数配置表/遥测站自报运行参数配置表
#define         GNM_CXSBDJ      0X44            //查询水泵电机实时工作数据
#define         GNM_CXRJBB      0X45            //查询遥测终端软件版本
#define         GNM_CXZTBJ      0X46            //查询遥测站状态和报警信息
#define         GNM_CSHSJ       0X47            //初始化固态存储数据
#define         GNM_HFCCSZ      0X48            //恢复终端机出厂设置
#define         GNM_XGMM        0X49            //修改密码
#define         GNM_SZSZ        0X4A            //设置遥测站时钟
#define         GNM_SWJB        0XE0            //自定义功能码 水位加报
#define         GNM_XGPZ        0XE1            //自定义功能码 修改自定义配置
#define         GNM_DQPZ        0XE2            //自定义功能码 读取自定义配置

#define         MessageTxdNByte         250             //上行报文最大长度/发送缓存长度
#define         MessageLength           10              //消息缓存长度
#define         DRPZLEN         240              //小时报缓存长度

typedef struct
{
  unsigned char Heartbeat;      //链路维持报
  unsigned char CeShi;          //测试报
  unsigned char JunYunShiDuan;  //均有时段水文信息报
  unsigned char DingShi;        //遥测站定时报
  unsigned char JiaBao;         //遥测站加报报
  unsigned char XiaoShi;        //遥测站小时报
  unsigned char RenGong;        //遥测站人工置数报
  unsigned char TuPian;         //遥测图片报或中心站查询遥测站图片采集信息
  unsigned char ChaXunShiShi;   //中心站查询遥测站实时数据
  unsigned char ChaXunShiDuan;  //中心站查询遥测站时段数据
  unsigned char ChaXuanRenGong; //中心站查询遥测站人工置数
  unsigned char CXZhiDingYaoSu; //中心站查询遥测指定要素数据
  unsigned char SetBasicConfig; //中心站修改遥基本配置表
  unsigned char GetBasicConfig; //中心站读取遥测站基本配置表
  unsigned char SetRunConfig;   //中心站修改遥测站运行参数配置表
  unsigned char GetRunConfig;   //中心站读取遥测站运行参数配置表
  unsigned char ChaXunShuiBeng; //查询水泵电机实时工作数据
  unsigned char ChaXunBanBen;   //查询遥测终端软件版本
  unsigned char ChaXunZhuangTai;//查询遥测站状态和报警信息
  unsigned char InitData;       //初始化固态存储数据
  unsigned char HuiFuChuChang;  //恢复终端机出厂设置
  unsigned char SetPassWord;    //修改密码
  unsigned char SetRTUTime;     //设置遥测站时钟
  unsigned char GetDefinConfig; //读取自定义配置
  unsigned char SetDefinConfig; //修改自定义配置
}ClassAFN;


/*观测时间*/
typedef struct
{
  unsigned char Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Minute;
}TT;


typedef struct
{
  unsigned int Page;
  unsigned int Bytes;
}ClassFlashDRPZ;

typedef struct
{ 
  long int Analog1;
  long int Analog2;
  //long int Analog3;
  //long int Analog4;
  long int RS485;
  unsigned int PT;
  unsigned int PN05;
  unsigned int PN10;
  unsigned int PN30;
  unsigned int PJ;
  unsigned char AFN;            //功能码
  
  TT RainTT;
  TT ATT;

}ClassMessage;

typedef struct
{
  ClassFlashDRPZ Now;
  ClassFlashDRPZ Array[DRPZLEN];
  unsigned int sp; 
}ClassDRPZ;

/*单个上报要素结构*/
typedef struct
{
  unsigned char *keyword;       //要素标识符
  long int value;               //要素数据值
  unsigned char DType;          //数据类型 BCD码 N(x,y)
}E_data;

typedef struct
{
  unsigned int PassWord;
  unsigned char CenterAdd;
  unsigned char AFN;    
  unsigned char TextLeng;
  unsigned char End;
  unsigned char ST[5];
  DataTime  RxdTime;
}ClassRxdMessage;

extern ClassMessage Message;
extern const ClassAFN AFN;
extern ComPortClass Com;
extern ClassDRPZ DRPZ;



void AddDRPZ(unsigned int page,unsigned int bytes);
void UpdateDRPZ(void);

void ComEventHandlers(void);
void ComSendMessage(void);
void AddMessengeTask(unsigned char gnm);
void SendMessenge(void);
unsigned int WriteMessage(unsigned char *p);
unsigned int WriteMessageHead(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteHeartbeatPackets(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageTextPublic(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageEnd(unsigned char *p,unsigned int  AddNbyte,unsigned char endcod);
unsigned int WriteMessageVariable(unsigned char *p,unsigned int  AddNbyte,E_data *pE);
unsigned int WriteMessageTextRain(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageTextTongDao(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageTextData(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageConfig(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageTextLiShiDada(unsigned char *p,unsigned int  AddNbyte);
unsigned int WriteMessageTextDRPZ(unsigned char *p,unsigned int  AddNbyte);
unsigned char* RxdMessageCrc(void);
void MessageHandlers(unsigned char *p);
unsigned char* SetConfiginfo(unsigned char *p);
void SetRunConfig(unsigned char *p);
#endif  /*#ifndef __CPASCII_H*/

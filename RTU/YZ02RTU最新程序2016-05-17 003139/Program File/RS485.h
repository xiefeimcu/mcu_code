#ifndef RS485_H_
#define RS485_H_

#include "include.h"

typedef struct
{
  long int Value;
  long int Max;
  unsigned char Name;
  unsigned char CgqType;
}ClassRs485;

typedef struct
{
  Uint16 StartSN;
  Uint16 Lenght;
  unsigned char SN;
  unsigned char AFN;
}ClassModbus;

extern ComPortClass RS485Com;
extern ClassRs485 RS485;
extern ClassModbus Modbus;
extern unsigned int Height;
void RS485RxdDataHandlers(void);
void RS485EventHandlers(void);
void RS485Read(void);
void RS485SendReadCmd(unsigned char st);
unsigned char RS485GetData(void);

void SendModbusCmd(void);
#endif  /*#ifndef RS485_H_*/



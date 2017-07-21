#ifndef LISHIDATA_H_
#define LISHIDATA_H_

#include "include.h"

typedef struct
{
  unsigned int PT;
  unsigned int A1;
  unsigned int A2;
  long Rs485;
  DataTime TT;
  
}ClassLsData;

typedef struct
{
  unsigned int Page;
  unsigned int Bytes;
  DataTime Satrtime;
  DataTime Endtime;
}ClassFlash;

extern ClassLsData LsData;
extern ClassFlash Flash;
extern unsigned char DRP[40];


void InitDRP(void);
void Flashadd(void);
void UpdateDRP(void);
void SaveDRP(void);
void WriteLishidata(void);
int SearchLishidata(unsigned char *p);
void ReadallLishiData(void);
unsigned char Writelishidatatext(void);
void ReadFlashHEX(void);
void flashtest(void);
void ReadDataMode(void);
#endif /*LISHIDATA_H_*/
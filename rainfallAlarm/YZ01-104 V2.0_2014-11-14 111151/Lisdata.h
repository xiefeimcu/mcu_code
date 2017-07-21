#ifndef LISDATA_H_
#define LISDATA_H_
#include "include.h"

typedef struct
{
  unsigned int PT;
  unsigned int P1;
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

void ReadLisdata(void);
void SaveLisdata(void);
unsigned char mychacou(unsigned char *s,unsigned char n,unsigned char c);
void sendtest(void);
#endif /*#ifndef LISDATA_H_*/
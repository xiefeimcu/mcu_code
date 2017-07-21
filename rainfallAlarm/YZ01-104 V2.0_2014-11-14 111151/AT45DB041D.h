
#ifndef __MEMORY_H
#define __MEMORY_H

#include "include.h"


/*******************FLASH相关引脚**************************/
#define Flash_CLK_H P3OUT |=BIT2
#define Flash_CLK_L P3OUT &=~BIT2
#define Flash_SI_H  P3OUT |=BIT1
#define Flash_SI_L  P3OUT &=~BIT1
#define Flash_CS_H  P3OUT |=BIT0
#define Flash_CS_L  P3OUT &=~BIT0

#define Flash_SO_in  P3DIR &=~BIT3
#define Flash_SO_val P3IN & BIT3


#define 	Write_Buffer1 							0x84//写Buffer1
#define 	Write_Buffer2 							0x87//写Buffer2

#define		Read_Buffer1							0xd4
#define		Read_Buffer2							0xd6
#define 	Status_Register_Read 						0xd7//读取flash状态寄存器
#define		Continuous_Array_Read 						0xe8//连续的数组读取
#define		Main_Memory_Page_Read						0xD2

#define 	Buffer1_To_MainMemory_PageProgram_WithBuilt_inErase		0x83//带预擦除先写入Buffer1,再写入指定页
#define 	Buffer2_To_MainMemory_PageProgram_WithBuilt_inErase 		0x86//带预擦除先写入Buffer2,再写入指定页

#define 	Buffer1_To_MainMemory_PageProgram_WithoutBuilt_inErase 		0x88//不带预擦除先写入Buffer1,再写入指定页
#define 	Buffer2_To_MainMemory_PageProgram_WithoutBuilt_inErase 		0x89//不带预擦除先写入Buffer2,再写入指定页

#define 	Page_Erase							0x81
#define 	Block_Erase							0x50

#define 	Main_Memory_Page_Program_Through_Buffer1			0x82
#define 	Main_Memory_Page_Program_Through_Buffer2			0x85

#define		Main_Memory_Page_To_Buffer1_Transfer				0x53
#define		Main_Memory_Page_To_Buffer2_Transfer				0x55

#define		Main_Memory_Page_To_Buffer1_Compare				0x60
#define		Main_Memory_Page_To_Buffer2_Compare				0x61

#define		Auto_Page_Rewrite_With_Buffer1					0x58
#define		Auto_Page_Rewrite_With_Buffer2					0x59
 
#define         ReadFalshData(x)    AT45DB041B_MainMemoryPageRead(x,0,buff,264)


char FindFlash(void);
char AT45DB041B_StatusRegisterRead(void);
void AT45DB041B_ContinuousArrayRead(uint PA, uint BFA, char *pHeader, uint len);
void Read_Buffer(uchar buffer, uint BFA, uchar *pHeader, uint len);
void AT45DB041B_BufferWrite(uchar buffer, uint BFA, uchar *pHeader, uint len); 
void AT45DB041B_MainMemoryPageRead(uint PA, uint BFA, char *pHeader, uint len);

void AT45DB041B_BufferToMainMemoryPageProgramWithoutBuilt_inErase(uchar buffer, uint PA, uint BFA, uchar *pHeader, uint len);

void AT45DB041B_PageErase(uint PA);
void AT45DB041B_BlockErase(uchar BLKA);

void Flash_to_buffer(uchar buffer, uint PA);
uchar AT45DB041B_MainMemoryPageToBufferCompare(uchar buffer, uint PA);

void AT45DB041B_MainMemoryPageProgramThroughBuffer(uchar buffer, uint PA, uint BFA, uchar *pHeader, uint len);
void AT45DB041B_AutoPageRewrite(uchar buffer, uint PA);
void SPI_Init(void);
void SPI_End(void);
void Save_data_to_Flash_inErase(char buffer, unsigned int PA, unsigned int BFA, char *pHeader, unsigned int len);

#endif   /* #ifndef __MEMORY_H */
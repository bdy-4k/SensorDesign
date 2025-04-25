#ifndef __KEY_H_
#define __KEY_H_


typedef volatile unsigned char vu8;
 
typedef struct
{
	vu8 Key_Flag;
	vu8 Key_Short;	//短按
	vu8 Key_Long;	//长按
	vu8 Key_Sada;	//按下时间
	vu8 Key_Status;	//当前状态
}_Key;


extern _Key One_Key[5];

void Get_Key(void);
void Reset_Key(void);

#include "main.h"
#endif

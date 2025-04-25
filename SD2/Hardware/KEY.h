#ifndef __KEY_H_
#define __KEY_H_


typedef volatile unsigned char vu8;
 
typedef struct
{
	vu8 Key_Flag;
	vu8 Key_Short;	//�̰�
	vu8 Key_Long;	//����
	vu8 Key_Sada;	//����ʱ��
	vu8 Key_Status;	//��ǰ״̬
}_Key;


extern _Key One_Key[5];

void Get_Key(void);
void Reset_Key(void);

#include "main.h"
#endif

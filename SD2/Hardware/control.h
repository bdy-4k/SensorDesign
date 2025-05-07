#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "main.h"
#include "spi.h"

extern uint16_t num5;

void DAC_SET_VALUE(uint16_t val);
void Get_ADC_Value(void);
void Get_Set_Alarm(void);
uint32_t ex_float2int(float value);
float ex_int2float(uint32_t value);
void Long_Turn_Byte(uint16_t MYadc[4],uint32_t MYHN3,uint32_t MYc1,uint32_t MYt1,uint16_t S_Num);
void Set_Data(void);

#endif

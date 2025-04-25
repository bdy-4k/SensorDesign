/*
 * aht20.h
 *
 *  Created on: Apr 25, 2024
 *      Author: lenovo
 */
 
#ifndef __MY_IIC_H_
#define __MY_IIC_H_
 
#include "main.h"

void AHT20_Init (void);
 
void AHT20_Read(float *Temperature , float *Humidity);

void MEMS_IIC_Read_Byte(uint16_t *reg);

#endif /* INC_AHT20_H_ */



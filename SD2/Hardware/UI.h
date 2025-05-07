#ifndef __UI_H_
#define __UI_H_

#include "main.h"
#include "KEY.h"



void UI_one(_Key *Key_One);
void UI_two(_Key *Key_two);
void UI_Therr(_Key *Key_the);
void UI_Init(void);
void OLED_DisplayCurve(uint16_t *data, uint8_t length);
#endif


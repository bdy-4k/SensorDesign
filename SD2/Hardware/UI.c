#include "UI.h"

void UI_one(_Key *Key_One)
{
	uint8_t Flag = 0;
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,sizeof(ADC_Value));
//	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
	OLED_ShowString(40, 0, "MAIN", OLED_8X16);
	OLED_ShowString(0, 20, "CO:", OLED_6X8);
	OLED_ShowString(70, 20, "H2S:", OLED_6X8);
	OLED_ShowString(0, 35, "CH2O:", OLED_6X8);
	OLED_ShowString(70, 35, "NH3:", OLED_6X8);
	OLED_ShowString(0, 50, "T1:", OLED_6X8);
	OLED_ShowString(70, 50, "C1:", OLED_6X8);
	OLED_ShowImage(0, 0, 16, 8, High);//90 
	OLED_Update();

	while(1)
	{
//		printf("kk%f----%f",t1,c1);
		OLED_ShowNum(90, 5, Sec_Num, 4, OLED_6X8);//电压值
		OLED_ShowNum(30, 20, ADC_Filter_Value[0], 4, OLED_6X8);//一氧化碳
		OLED_ShowNum(100, 20, ADC_Filter_Value[1], 4, OLED_6X8);//硫化氢
		OLED_ShowNum(30, 35, ADC_Filter_Value[2], 4, OLED_6X8); //甲醛
		OLED_ShowNum(95, 35, NH3_Value, 5, OLED_6X8); 			//氨气
		
		OLED_ShowFloatNum(20,50,t1,2,2,OLED_6X8);
		OLED_ShowFloatNum(90,50,c1,2,2,OLED_6X8);

		if(ADC_Filter_Value[3] < 1250 && Flag < 1)
		{
			OLED_ShowImage(0, 0, 16, 8, Middle);
			Flag = 1;
		}
		else if(ADC_Filter_Value[3] < 1170 && Flag < 2 && Flag != 0)
		{
			OLED_ShowImage(0, 0, 16, 8, Low);
			Flag = 2;
		}
		
		
		OLED_Update();
	  
	  if(Key_One[1].Key_Short)
	  {
		Reset_Key();
		OLED_Clear();				//清空显存数组
		OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
		  break;
	  }
	  if(Key_One[2].Key_Long)
	  {
		  W25QXX_Erase_Sector(0);
		  Sectors_Num[0] = (uint8_t)Sec_Num & 0xFF;
		  Sectors_Num[1] = ((uint8_t)Sec_Num >> 8) & 0xFF;
		  W25QXX_Page_Program(Sectors_Num, 0, 2);
		HAL_GPIO_WritePin(PWR_CTR_GPIO_Port,PWR_CTR_Pin,GPIO_PIN_RESET);

	  }
	  HAL_Delay(300);
	}
}

void UI_two(_Key *Key_two)
{
	uint8_t place = 20;
	uint16_t Target[4];
	uint8_t i;
	
	OLED_ShowString(40, 0, "alter", OLED_8X16);
	OLED_ShowString(0, 20, "CO:", OLED_6X8);
	OLED_ShowString(0, 30, "H2S:", OLED_6X8);
	OLED_ShowString(0, 40, "CH2O:", OLED_6X8);
	OLED_ShowString(0, 50, "NH3:", OLED_6X8);
	OLED_Update();

	for(i = 0;i<4;i++)
	{
		Target[i] = Gas_MAX[i];
	}
	while(1)
	{
		OLED_ShowNum(30, 20, Gas_MAX[0], 4, OLED_6X8);
		OLED_ShowNum(30, 30, Gas_MAX[1], 4, OLED_6X8);
		OLED_ShowNum(30, 40, Gas_MAX[2], 4, OLED_6X8); 
		OLED_ShowNum(30, 50, Gas_MAX[3], 5, OLED_6X8); 
							
		OLED_ShowNum(90, 20, Target[0], 4, OLED_6X8);
		OLED_ShowNum(90, 30, Target[1], 4, OLED_6X8);
		OLED_ShowNum(90, 40, Target[2], 4, OLED_6X8); 
		OLED_ShowNum(90, 50, Target[3], 5, OLED_6X8); 
		
		OLED_ShowString(80, place, "*", OLED_6X8);
		OLED_Update();
	  
		if(Key_two[0].Key_Short)
		{
			OLED_ShowString(80, place, " ", OLED_6X8);
			place += 10;
			if(place > 50)
				place = 20;
			Key_two[0].Key_Short = 0;
		}
		
		if(Key_two[2].Key_Short || Key_two[2].Key_Long)
		{
			switch(place)
			{
				case 20:
					if(Key_two[2].Key_Short)
						Target[0] += 100;
					else
						Target[0] += 1000;
					if(Target[0] > 3300)
						Target[0] = 100;
					break;
				case 30:
					if(Key_two[2].Key_Short)
						Target[1] += 100;
					else
						Target[1] += 1000;
					if(Target[1] > 3300)
						Target[1] = 100;
					break;
				case 40:
					if(Key_two[2].Key_Short)
						Target[2] += 100;
					else
						Target[2] += 1000;
					if(Target[2] > 3300)
						Target[2] = 100;
					break;
				case 50:
					if(Key_two[2].Key_Short)
						Target[3] += 100;
					else
						Target[3] += 1000;
					if(Target[3] > 65535)
						Target[3] = 1000;
					break;
			}
			Key_two[2].Key_Short = 0;
			Key_two[2].Key_Long = 0;			
		}
		
		if(Key_two[1].Key_Long)
		{
			for(i = 0;i<4;i++)
			{
				Gas_MAX[i] = Target[i];
			}
			Key_two[1].Key_Long = 0;
		}
	  if(Key_two[1].Key_Short)
	  {
		Reset_Key();
		OLED_Clear();				//清空显存数组
		OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
		  break;
	  }
	  HAL_Delay(300);
	}
}

void UI_Therr(_Key *Key_the)
{
	HAL_ADC_Stop_DMA(&hadc1);
	
	Set_Data();	
	Past_Data[6] = ex_int2float(Past_Data[6]);
	Past_Data[5] = ex_int2float(Past_Data[5]);
	OLED_ShowString(40, 0, "DATA", OLED_8X16);
	OLED_ShowString(0, 20, "CO:", OLED_6X8);
	OLED_ShowString(70, 20, "H2S:", OLED_6X8);
	OLED_ShowString(0, 35, "CH2O:", OLED_6X8);
	OLED_ShowString(70, 35, "NH3:", OLED_6X8);
	OLED_ShowString(0, 50, "T1:", OLED_6X8);
	OLED_ShowString(70, 50, "C1:", OLED_6X8);
	
	OLED_ShowNum(30, 20, Past_Data[0], 4, OLED_6X8);
	OLED_ShowNum(100, 20, Past_Data[1], 4, OLED_6X8);
	OLED_ShowNum(30, 35, Past_Data[2], 4, OLED_6X8); 
	OLED_ShowNum(95, 35, Past_Data[4], 5, OLED_6X8); 
	OLED_ShowFloatNum(20,50,Past_Data[6],2,2,OLED_6X8);
	OLED_ShowFloatNum(90,50,Past_Data[5],2,2,OLED_6X8);
	OLED_ShowNum(90,5,Past_Data[7],2,OLED_6X8);
	
	OLED_Update();
	
	while(1)
	{
	
		if(Key_the[0].Key_Short)
		{
			Set_Data();	
			Past_Data[6] = ex_int2float(Past_Data[6]);
			Past_Data[5] = ex_int2float(Past_Data[5]);
			OLED_ShowNum(30, 20, Past_Data[0], 4, OLED_6X8);
			OLED_ShowNum(100, 20, Past_Data[1], 4, OLED_6X8);
			OLED_ShowNum(30, 35, Past_Data[2], 4, OLED_6X8); 
			OLED_ShowNum(95, 35, Past_Data[4], 5, OLED_6X8); 
			OLED_ShowFloatNum(20,50,Past_Data[6],2,2,OLED_6X8);
			OLED_ShowFloatNum(90,50,Past_Data[5],2,2,OLED_6X8);
			OLED_ShowNum(90,5,Past_Data[7],2,OLED_6X8);
			OLED_Update();				
			Key_the[0].Key_Short = 0;
		}
		if(Key_the[1].Key_Short)
		  {
			Reset_Key();
			OLED_Clear();				//清空显存数组
			OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
			  break;
		  }
//		  if(Key_the[2].Key_Short)
//		  {
//			  Set_Data(0);
//			Key_the[2].Key_Short = 0;
//		  }
		  if(Key_the[2].Key_Long)
		  {
			  
			  W25QXX_Erase();
			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
			Key_the[2].Key_Long = 0;
		  }
		HAL_Delay(300);
	}
}



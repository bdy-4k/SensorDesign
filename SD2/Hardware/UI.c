#include "UI.h"

uint16_t data_buffer[128];
uint8_t buffer_index = 0;

typedef enum
{
  UI_M = 0u,
  UI_L
} UI_SHOW;

typedef enum
{
  C_CO = 0u,
  C_H2S,
	C_CH2O,
	C_NH3,
	C_ERR
} CT_PR;

CT_PR My_MState = C_ERR;		//首先赋一个错误的值
CT_PR My_LState = C_ERR;		//首先赋一个错误的值

void UI_one_Mson(CT_PR Mson_State)
{
	OLED_ShowString(40, 0, "MAIN", OLED_8X16);

	if(Mson_State != My_MState)
		switch(Mson_State)
		{
			case C_CO:
				OLED_ShowString(0, 20, "CO:", OLED_6X8);
				OLED_ShowString(70, 20, "H2S:", OLED_6X8);
				OLED_ShowString(0, 35, "CH2O:", OLED_6X8);
				OLED_ShowString(70, 35, "NH3:", OLED_6X8);
				OLED_ReverseArea(0,20,12,8);
				My_MState = Mson_State;
				break;
			case C_H2S:
				OLED_ShowString(0, 20, "CO:", OLED_6X8);
				OLED_ShowString(70, 20, "H2S:", OLED_6X8);
				OLED_ShowString(0, 35, "CH2O:", OLED_6X8);
				OLED_ShowString(70, 35, "NH3:", OLED_6X8);
				OLED_ReverseArea(70,20,18,8);
				My_MState = Mson_State;
				break;
			case C_CH2O:
				OLED_ShowString(0, 20, "CO:", OLED_6X8);
				OLED_ShowString(70, 20, "H2S:", OLED_6X8);
				OLED_ShowString(0, 35, "CH2O:", OLED_6X8);
				OLED_ShowString(70, 35, "NH3:", OLED_6X8);
				OLED_ReverseArea(0,35,24,8);
				My_MState = Mson_State;
				break;
			case C_NH3:
				OLED_ShowString(0, 20, "CO:", OLED_6X8);
				OLED_ShowString(70, 20, "H2S:", OLED_6X8);
				OLED_ShowString(0, 35, "CH2O:", OLED_6X8);
				OLED_ShowString(70, 35, "NH3:", OLED_6X8);
				OLED_ReverseArea(70,35,18,8);
				My_MState = Mson_State;
				break;
			case C_ERR:
				break;
		}
	
	OLED_ShowString(0, 50, "T1:", OLED_6X8);
	OLED_ShowString(70, 50, "C1:", OLED_6X8);
	OLED_ShowImage(0, 0, 16, 8, High);		//电池三格
//	OLED_Update();
	
	OLED_ShowNum(90, 5, Sec_Num, 4, OLED_6X8);//电压值
	OLED_ShowNum(30, 20, ADC_Filter_Value[0], 4, OLED_6X8);//一氧化碳
	OLED_ShowNum(100, 20, ADC_Filter_Value[1], 4, OLED_6X8);//硫化氢
	OLED_ShowNum(30, 35, ADC_Filter_Value[2], 4, OLED_6X8); //甲醛
	OLED_ShowNum(95, 35, NH3_Value, 5, OLED_6X8); 			//氨气
	
	OLED_ShowFloatNum(20,50,t1,2,2,OLED_6X8);
	OLED_ShowFloatNum(90,50,c1,2,2,OLED_6X8);
	
	OLED_Update();
}
void UI_one_Lson(CT_PR Lson_State)
{
	uint16_t data;
	if(Lson_State != My_LState)
		buffer_index = 0;
	switch(Lson_State)
	{
		case C_CO:
			OLED_ShowString(20, 0, "CO:", OLED_8X16);
			OLED_ShowNum(44, 0, ADC_Filter_Value[0], 4, OLED_8X16);//一氧化碳
			My_LState = Lson_State;
			data = ADC_Filter_Value[0];
			break;
		case C_H2S:
			OLED_ShowString(20, 0, "H2S:", OLED_8X16);
			OLED_ShowNum(52, 0, ADC_Filter_Value[1], 4, OLED_8X16);//硫化氢
			My_LState = Lson_State;
			data = ADC_Filter_Value[1];
			break;
		case C_CH2O:
			OLED_ShowString(20, 0, "CH2O:", OLED_8X16);
			OLED_ShowNum(65, 0, ADC_Filter_Value[2], 4, OLED_8X16); //甲醛
			My_LState = Lson_State;
			data = ADC_Filter_Value[2];
			break;
		case C_NH3:
			OLED_ShowString(20, 0, "NH3:", OLED_8X16);
			OLED_ShowNum(52, 0, NH3_Value, 5, OLED_8X16); 			//氨气
			My_LState = Lson_State;
			data = NH3_Value;
			break;
		case C_ERR:
			break;
	}
	OLED_Update();
	if (buffer_index >= 128) {
			for (int i = 0; i < 128 - 1; i++) {
					data_buffer[i] = data_buffer[i + 1];
			}
			buffer_index--;
	}
	data_buffer[buffer_index++] = data;
	
	OLED_DisplayCurve(data_buffer,buffer_index);	
	OLED_Update();
	My_MState = C_ERR;		//赋一个错误的值

}
/*
*		主界面设置了子界面
*		
*
*/
void UI_one(_Key *Key_One)				
{
	uint8_t Flag = 0;
	CT_PR ONE_STATE = C_CO;
	UI_SHOW ONE_CT = UI_M;
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,sizeof(ADC_Value));

	while(1)
	{
		if(ONE_CT == UI_M)				//根据标志位选择进入哪个子界面
			UI_one_Mson(ONE_STATE);
		else
			UI_one_Lson(ONE_STATE);
		
		if(Key_One[0].Key_Short && ONE_CT == UI_M)//保证在主界面才能切换选择目标
		{
			switch(ONE_STATE)
			{
				case C_CO:
					ONE_STATE = C_H2S;
					break;
				case C_H2S:
					ONE_STATE = C_CH2O;
					break;
				case C_CH2O:
					ONE_STATE = C_NH3;
					break;
				case C_NH3:
					ONE_STATE = C_CO;
					break;
				case C_ERR:
					break;
			}
			Key_One[0].Key_Short = 0;
		}
		
		if(ADC_Filter_Value[3] < 1250 && Flag < 1)		
		{
			OLED_ShowImage(0, 0, 16, 8, Middle);		//电池二格
			Flag = 1;
		}
		else if(ADC_Filter_Value[3] < 1170 && Flag < 2 && Flag != 0)
		{
			OLED_ShowImage(0, 0, 16, 8, Low);		//电池一格
			Flag = 2;
		}
		OLED_Update();
	  
	  if(Key_One[1].Key_Short && ONE_CT == UI_M)		//切换界面
	  {
			Reset_Key();
			OLED_Clear();					//清空显存数组
			OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
			break;
	  }
		if(Key_One[2].Key_Short)
		{
			if(ONE_CT == UI_M)
			{
				OLED_Clear();				//清空显存数组
				ONE_CT = UI_L;
				My_MState = C_ERR;		//首先赋一个错误的值
			}
			else
			{
				OLED_Clear();				//清空显存数组
				ONE_CT = UI_M;
				Reset_Key();
				My_MState = C_ERR;		//首先赋一个错误的值
			}
			Key_One[2].Key_Short = 0;
		}
	  if(Key_One[2].Key_Long && ONE_CT == UI_M)
	  {
		  W25QXX_Erase_Sector(0);															/*清空Flash第0页，将保存过数据的次数写入*/
		  Sectors_Num[0] = (uint8_t)Sec_Num & 0xFF;
		  Sectors_Num[1] = ((uint8_t)Sec_Num >> 8) & 0xFF;
		  W25QXX_Page_Program(Sectors_Num, 0, 2);
		  HAL_GPIO_WritePin(PWR_CTR_GPIO_Port,PWR_CTR_Pin,GPIO_PIN_RESET);//关机
	  }
	  HAL_Delay(100);
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
		
		if(Key_two[2].Key_Short)
		{
			switch(place)
			{
				case 20:
						Target[0] += 100;
					if(Target[0] > 3300)
						Target[0] = 100;
					break;
				case 30:
						Target[1] += 100;
					if(Target[1] > 3300)
						Target[1] = 100;
					break;
				case 40:
						Target[2] += 100;
					if(Target[2] > 3300)
						Target[2] = 100;
					break;
				case 50:
						Target[3] += 100;
					if(Target[3] > 65535)
						Target[3] = 1000;
					break;
			}
			Key_two[2].Key_Short = 0;		
		}
		
		if(Key_two[2].Key_Long)
		{
			for(i = 0;i<4;i++)				//把设置的值赋值
			{
				Gas_MAX[i] = Target[i];
			}
			Key_two[2].Key_Long = 0;
		}
	  if(Key_two[1].Key_Short)
	  {
			Reset_Key();
			OLED_Clear();				//清空显存数组
			OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
		  break;
	  }
	  HAL_Delay(100);
	}
}

void UI_Therr(_Key *Key_the)
{
	HAL_ADC_Stop_DMA(&hadc1);
	num5 = 16; 						//进来第三界面，从16页开始
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
				Reset_Key();				//清除按键标志
				My_MState = C_ERR;		//赋一个错误的值
				OLED_Clear();				//清空显存数组
				OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
			  break;
		  }
		  if(Key_the[2].Key_Long)
		  {
				OLED_Clear();				//清空显存数组
				OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
				OLED_ShowString(0,25,"Is being cleared.",OLED_6X8);		//
				OLED_Update();
			  W25QXX_Erase();			//擦除Flash芯片
				OLED_Clear();				//清空显存数组
				OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
				Sec_Num = 16;			//页码重置为16
				My_MState = C_ERR;		//赋一个错误的值
				Key_the[2].Key_Long = 0;
				break;									//擦除结束回到主界面
		  }
		HAL_Delay(100);
	}
}
/*
*		初始化界面，给传感器预热时间
*/
void UI_Init(void)
{
	OLED_ShowString(0,25,"Initializing",OLED_8X16);
	OLED_Update();
	HAL_Delay(1000);
	OLED_ShowString(98,25,".",OLED_8X16);
	OLED_Update();
	HAL_Delay(2000);
	OLED_ShowString(103, 25,".",OLED_8X16);
	OLED_Update();
	HAL_Delay(2000);
	OLED_ShowString(108, 25,".",OLED_8X16);
	OLED_Update();
	HAL_Delay(2000);
	OLED_ShowString(113, 25,".",OLED_8X16);
	OLED_Update();
	HAL_Delay(2000);
	OLED_ShowString(118, 25,".",OLED_8X16);
	OLED_Update();
	HAL_Delay(1000);
	OLED_Clear();				//清空显存数组
	OLED_Update();
}

void OLED_DisplayCurve(uint16_t *data, uint8_t length) 
{
    if (length > 128) {
        length = 128;
    }
	OLED_ClearArea(0,16,128,64);//清除图像部分的显存
    // 计算范围
    uint16_t range;

		if(data[0] > 1000)		//设置两个缩放比例
			range = 3300;
		else
			range = 1000;
    for (uint8_t i = 0; i < length - 1; i++) {
        // 线性映射到0 - 40的高度区间
        uint8_t y0 = data[i] * 40 / range;
        uint8_t y1 = data[i + 1] * 40 / range;

        OLED_DrawLine(i, 23+(40-y0), i + 1, 23+(40-y1));
    }
}

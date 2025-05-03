#include "control.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t TIM_num = 0;
	static uint8_t TIM_num2 = 0;
	uint32_t t1_int,c1_int;
	if(htim == &htim6)
	{
		TIM_num++;
		Get_Key(); 
		if(TIM_num >= 30)
		{
			AHT20_Read(&t1,&c1); 
			Get_ADC_Value();
			MEMS_IIC_Read_Byte(&NH3_Value); 
			Get_Set_Alarm();
			TIM_num2++;
			TIM_num = 0;
		}
		
		if(TIM_num2 >= 30)
		{
			HAL_ADC_Stop_DMA(&hadc1);
			t1_int = ex_float2int(t1);
			c1_int = ex_float2int(c1);
			Long_Turn_Byte(ADC_Filter_Value,NH3_Value,c1_int,t1_int,Sec_Num);
			if(Sec_Num % 16 == 0)
				W25QXX_Erase_Sector(Sec_Num / 16);
			W25QXX_Page_Program(write_buf,Sec_Num , 26);

			Sec_Num++;
			TIM_num2 = 0;
			HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,sizeof(ADC_Value));
		}
	}
}

void Get_ADC_Value(void)
{
	/********************************轮询*/
//	for(int i=0;i<4;i++)
//	{
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1,10);
//		ADC_Value[i]=HAL_ADC_GetValue(&hadc1);
//	}
//	for(int i2 = 0;i2 <4 ;i2++)
//	{
//		ADC_Value[i2] = (ADC_Value[i2] * 3300) >> 12;
//	}
	/**********************************/
	uint32_t count;
	uint32_t sum[4] = {0};

	for(count = 0;count < 40;count++)
	{
		sum[count%4] += ADC_Value[count%4];	
	}
	for(count = 0;count < 4;count++)
	{
		ADC_Filter_Value[count] = (sum[count] * 330)  >> 12;//
	}
}

void Get_Set_Alarm(void)
{
	if(ADC_Filter_Value[0] >= Gas_MAX[0] || ADC_Filter_Value[1] >= Gas_MAX[1] || ADC_Filter_Value[2] >= Gas_MAX[2] || NH3_Value >= Gas_MAX[3])
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		HAL_GPIO_WritePin(BUZZ_GPIO_Port,BUZZ_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BUZZ_GPIO_Port,BUZZ_Pin,GPIO_PIN_RESET);
	}
}

void Set_Data(void)
{
	static uint16_t num5 = 16;

	W25QXX_Read(read_buf, num5, 26);

	Past_Data[0] = ((uint16_t)read_buf[1] << 8) + (uint16_t)read_buf[0];//一氧化碳
	Past_Data[1] = ((uint16_t)read_buf[3] << 8) + (uint16_t)read_buf[2];//硫化氢
	Past_Data[2] = ((uint16_t)read_buf[5] << 8) + (uint16_t)read_buf[4];//甲醛
	Past_Data[3] = ((uint16_t)read_buf[7] << 8) + (uint16_t)read_buf[6];//电池
	
	Past_Data[4] = ((uint32_t)read_buf[15] << 24) + ((uint32_t)read_buf[14] << 16) + ((uint32_t)read_buf[13] << 8) + (uint32_t)read_buf[12];//氨气
	Past_Data[5] = ((uint32_t)read_buf[19] << 24) + ((uint32_t)read_buf[18] << 16) + ((uint32_t)read_buf[17] << 8) + (uint32_t)read_buf[16];//湿度
	Past_Data[6] = ((uint32_t)read_buf[23] << 24) + ((uint32_t)read_buf[22] << 16) + ((uint32_t)read_buf[21] << 8) + (uint32_t)read_buf[20];//温度
	Past_Data[7] = ((uint16_t)read_buf[25] << 8) + (uint16_t)read_buf[24];//序号
	
	num5++;
}

/*----------------------*/
/*	 浮点型转整型存储	*/
/*======================*/
uint32_t ex_float2int(float value){
//	定义联合体
	union{
		float float_value;
		uint32_t int_value;
	}c;
//	存储数据转换
	c.float_value = value;//以浮点型存入联合体
	return c.int_value;//以整型返回
}

float ex_int2float(uint32_t value){
//	定义联合体
	union{
		float float_value;
		uint32_t int_value;
	}c;
//	存储数据转换
	c.int_value = value;//以浮点型存入联合体
	return c.float_value;//以整型返回
}
/*
	函数名：把传入的值转化成uint8_t型
	参数：ADC4个通道的值
	参数：HN3
	参数：湿度
	参数：温度
	参数：页面编号

	说明：低位在前
*/
void Long_Turn_Byte(uint16_t MYadc[4],uint32_t MYHN3,uint32_t MYc1,uint32_t MYt1,uint16_t S_Num)
{

	write_buf[0] = (uint8_t)MYadc[0] & 0xFF;
	write_buf[1] = (uint8_t)(MYadc[0] >> 8) & 0xFF;
	
	write_buf[2] = (uint8_t)MYadc[1] & 0xFF;
	write_buf[3] = (uint8_t)(MYadc[1] >> 8) & 0xFF;

	write_buf[4] = (uint8_t)MYadc[2] & 0xFF;
	write_buf[5] = (uint8_t)(MYadc[2] >> 8) & 0xFF;
	
	write_buf[6] = (uint8_t)MYadc[3] & 0xFF;
	write_buf[7] = (uint8_t)(MYadc[3] >> 8) & 0xFF;
	
	write_buf[8] = 0xFF;
	write_buf[9] = 0xFF;
	write_buf[10] =0xFF;
	write_buf[11] =0xFF;
	
	write_buf[12] = (uint8_t)MYHN3 & 0xFF;
	write_buf[13] = (uint8_t)(MYHN3 >> 8) & 0xFF;
	write_buf[14] = (uint8_t)(MYHN3 >> 16) & 0xFF;
	write_buf[15] = (uint8_t)(MYHN3 >> 24) & 0xFF;
	
	write_buf[16] = (uint8_t)MYc1 & 0xFF;
	write_buf[17] = (uint8_t)(MYc1 >> 8) & 0xFF;
	write_buf[18] = (uint8_t)(MYc1 >> 16) & 0xFF;
	write_buf[19] = (uint8_t)(MYc1 >> 24) & 0xFF;

	write_buf[20] = (uint8_t)MYt1 & 0xFF;
	write_buf[21] = (uint8_t)(MYt1 >> 8) & 0xFF;
	write_buf[22] = (uint8_t)(MYt1 >> 16) & 0xFF;
	write_buf[23] = (uint8_t)(MYt1 >> 24) & 0xFF;

	write_buf[24] = (uint8_t)S_Num & 0xFF;
	write_buf[25] = (uint8_t)(S_Num >> 8) & 0xFF;
	
}                   


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	OLED_ShowString(0, 40, "ookk", OLED_8X16); 

}

//用0-3300代表0-3.3v
//设置通道输出电压
void DAC_SET_VALUE(uint16_t val)
{
	double temp = val;
	temp /= 1000;
	temp  = temp/3.3*4096;//转换成数字量，供ADC读取，然后我们在转换为模拟量
	if(temp>=4096)temp=4095;
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);
}



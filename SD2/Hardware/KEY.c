#include "KEY.h"
 
_Key One_Key[5] = {0};
void Get_Key(void)
{
	One_Key[0].Key_Status = HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
	One_Key[1].Key_Status = HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);
	One_Key[2].Key_Status = HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin);
//	One_Key[3].Key_Status = HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin); 
	
	for(int i = 0;i < 3;i++)
	{
		switch(One_Key[i].Key_Flag)
		{
			case 0:
				if(One_Key[i].Key_Status == 0)
					One_Key[i].Key_Flag = 1;
				break;
			case 1:
				if(One_Key[i].Key_Status == 0)
					One_Key[i].Key_Flag = 2;
				else
					One_Key[i].Key_Flag = 0;
				break;
			case 2:
				if(One_Key[i].Key_Status == 0)
				{
					if(One_Key[i].Key_Sada < 100)
						One_Key[i].Key_Sada++;
					if(One_Key[i].Key_Sada >= 100)
					{
						if(One_Key[i].Key_Sada != 120)
							One_Key[i].Key_Long = 1;
						One_Key[i].Key_Sada = 120;
					}
				}
				else
				{
					if(One_Key[i].Key_Sada >= 120)
						One_Key[i].Key_Sada = 0;
					else
						One_Key[i].Key_Short = 1;
					One_Key[i].Key_Sada = 0;
					One_Key[i].Key_Flag = 0;
				}
				break;
		}
	}
}

void Reset_Key(void)
{
	for(int i=0 ;i < 3;i++)
	{
		One_Key[i].Key_Short=0;
		One_Key[i].Key_Long=0;
		
	}
}



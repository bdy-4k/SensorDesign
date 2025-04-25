#include "MEMS.h"

//#define MEMS_ADDRESS 0x54 // 从机地址 
/*****************************************硬件IIC********************************************
void IIC_Read_Byte(uint16_t *result)
{
	uint8_t sendBuffer [1] = {0xA1};
	uint8_t readBuffer [2];
	HAL_I2C_Master_Transmit(&hi2c1, MEMS_ADDRESS, sendBuffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, MEMS_ADDRESS, readBuffer, 2, HAL_MAX_DELAY);
	
	*result = ((uint16_t)readBuffer[0] << 8) + (uint16_t)readBuffer[1];
//	printf("%d *---*%d----%d\r\n",readBuffer[0],readBuffer[1],*result);
	
}

void IIC_Write_Set_Zero(void)
{
	uint8_t sendBuffer [3] = {0x32,0x19,0x99};//后两位调清洁环境下气体浓度

	HAL_I2C_Master_Transmit(&hi2c1, MEMS_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);

}
*************************************************************************************/

















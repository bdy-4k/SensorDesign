#include "MY_IIC.h"
 
#define AHT20_ADDRESS 0x70 // 从机地址


//AHT20 的驱动程序
 void AHT20_Init () //AHT20初始化函数  记住要在"aht20.h"中声明
{
  uint8_t readBuffer;//用于接收状态信息
  HAL_Delay(40);
  HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);//I2C读取函数，读数据函数 readBuffer此时获得了一个字节的状态字。
  if((readBuffer & 0x08) == 0x00) //判断第三位是否为0 发送0xBE命令初始化
  {
	  uint8_t sendBuffer [3] = {0xBE , 0x08 , 0x00};
	  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);//I2C发送函数
  }
 
}
 
 void AHT20_Read(float *Temperature , float *Humidity) //AHT20读取温度湿度函数  记住要在"aht20.h"中声明
{
	uint8_t sendBuffer [3] = {0xAC , 0x33 , 0x00};
	uint8_t readBuffer [6];
	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
	HAL_Delay(75);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);
	if((readBuffer[0] & 0x80) == 0x00)
	{
		uint32_t date = 0;//接收温湿度需要2个半字节 所以要32
		date = ((uint32_t )readBuffer[3] >> 4) + ((uint32_t )readBuffer[2] << 4) + ((uint32_t )readBuffer[1] << 12);//对数据进行移位拼接.
		*Humidity = date * 100.0f / (1 << 20);//(1 << 20) 意为2的20次方. 乘100.0可以表示为百分数
 
		date = (((uint32_t )readBuffer[3] & 0x0F)<< 16) + ((uint32_t )readBuffer[4] << 8) + (uint32_t )readBuffer[5];//& 0x0F: 将这个无符号整数与十六进制数0x0F进行按位与操作。0x0F的二进制表示为00001111，这个操作会保留readBuffer[3]的低四位，即将高四位清零。
		*Temperature = date * 200.0f / (1 << 20) - 50;
	}
}

void IIC_Delay(void)
{
	uint16_t x= 500;
	
	while(x--);
}

/**
  * 函    数：IIC写SCL高低电平
  * 参    数：要写入SCL的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SCL时，此函数会被调用
  *           用户需要根据参数传入的值，将SCL置为高电平或者低电平
  *           当参数传入0时，置SCL为低电平，当参数传入1时，置SCL为高电平
  */
void IIC_W_SCL(uint8_t BitValue)
{
	/*根据BitValue的值，将SCL置高电平或者低电平*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, (GPIO_PinState)BitValue);
	
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
	IIC_Delay();
}

/**
  * 函    数：IIC写SDA高低电平
  * 参    数：要写入SDA的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SDA时，此函数会被调用
  *           用户需要根据参数传入的值，将SDA置为高电平或者低电平
  *           当参数传入0时，置SDA为低电平，当参数传入1时，置SDA为高电平
  */
void IIC_W_SDA(uint8_t BitValue)
{
	/*根据BitValue的值，将SDA置高电平或者低电平*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (GPIO_PinState)BitValue);
	
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
	IIC_Delay();

}

uint8_t IIC_R_SDA(void)
{
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
}

uint8_t IIC_Statr(void)
{
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	if(!IIC_R_SDA())
		return 0;
	IIC_W_SDA(0);
	if(IIC_R_SDA())
		return 0;
	IIC_W_SCL(0);
	return 1;
}

void IIC_SendByte(uint8_t SendByte)
{
	uint8_t i=8;
	IIC_W_SCL(0);
	for(i = 0;i < 8;i++)
	{
		if(SendByte & 0x80)
			IIC_W_SDA(1);
		else
			IIC_W_SDA(0);
		IIC_W_SCL(1);
		IIC_W_SCL(0);
		SendByte <<= 1;
	}
}

uint8_t IIC_WaitAck(void)
{
	uint16_t i=0;
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	while(IIC_R_SDA())
	{
		i++;
		if(i >= 1000)
			break;
	}
	if(IIC_R_SDA())
	{
		IIC_W_SCL(0);
		return 0;
	}
	IIC_W_SCL(0);
	return 1;
}

uint8_t IIC_RecvByte(void)
{
	uint8_t i = 8;
	uint8_t ReceiveByte = 0;
	IIC_W_SDA(1);
	for(i = 0;i < 8;i++)
	{
		ReceiveByte <<= 1;
		IIC_W_SCL(1);
		if(IIC_R_SDA())
			ReceiveByte |= 0x01;
		IIC_W_SCL(0);
	}
	return ReceiveByte;
}
	
void IIC_Stop(void)
{
	IIC_W_SCL(0);
	IIC_W_SDA(0);
	IIC_W_SCL(1);
	IIC_W_SDA(1);
}

void IIC_SendACK(uint8_t i)
{
	if( i ==1)
		IIC_W_SDA(1);
	else
		IIC_W_SDA(0);
	IIC_W_SCL(1);
	IIC_W_SCL(0);
}

/************************MEMS读取浓度***********************/
void MEMS_IIC_Read_Byte(uint16_t *reg)
{
	uint8_t rec_data; 
	uint8_t rec_data16;

	IIC_Statr();
	IIC_SendByte(0x54);
	IIC_WaitAck();
	IIC_SendByte(0xE1);
	IIC_WaitAck();
	IIC_Statr();
	IIC_SendByte(0x55);
	IIC_WaitAck();
	rec_data = IIC_RecvByte();
	rec_data16 = rec_data;
	IIC_SendACK(0);
	rec_data = IIC_RecvByte();
	*reg = (rec_data16 << 8) + rec_data;
	IIC_Stop();
	
}








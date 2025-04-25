#include "MY_IIC.h"
 
#define AHT20_ADDRESS 0x70 // �ӻ���ַ


//AHT20 ����������
 void AHT20_Init () //AHT20��ʼ������  ��סҪ��"aht20.h"������
{
  uint8_t readBuffer;//���ڽ���״̬��Ϣ
  HAL_Delay(40);
  HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);//I2C��ȡ�����������ݺ��� readBuffer��ʱ�����һ���ֽڵ�״̬�֡�
  if((readBuffer & 0x08) == 0x00) //�жϵ���λ�Ƿ�Ϊ0 ����0xBE�����ʼ��
  {
	  uint8_t sendBuffer [3] = {0xBE , 0x08 , 0x00};
	  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);//I2C���ͺ���
  }
 
}
 
 void AHT20_Read(float *Temperature , float *Humidity) //AHT20��ȡ�¶�ʪ�Ⱥ���  ��סҪ��"aht20.h"������
{
	uint8_t sendBuffer [3] = {0xAC , 0x33 , 0x00};
	uint8_t readBuffer [6];
	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
	HAL_Delay(75);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);
	if((readBuffer[0] & 0x80) == 0x00)
	{
		uint32_t date = 0;//������ʪ����Ҫ2�����ֽ� ����Ҫ32
		date = ((uint32_t )readBuffer[3] >> 4) + ((uint32_t )readBuffer[2] << 4) + ((uint32_t )readBuffer[1] << 12);//�����ݽ�����λƴ��.
		*Humidity = date * 100.0f / (1 << 20);//(1 << 20) ��Ϊ2��20�η�. ��100.0���Ա�ʾΪ�ٷ���
 
		date = (((uint32_t )readBuffer[3] & 0x0F)<< 16) + ((uint32_t )readBuffer[4] << 8) + (uint32_t )readBuffer[5];//& 0x0F: ������޷���������ʮ��������0x0F���а�λ�������0x0F�Ķ����Ʊ�ʾΪ00001111����������ᱣ��readBuffer[3]�ĵ���λ����������λ���㡣
		*Temperature = date * 200.0f / (1 << 20) - 50;
	}
}

void IIC_Delay(void)
{
	uint16_t x= 500;
	
	while(x--);
}

/**
  * ��    ����IICдSCL�ߵ͵�ƽ
  * ��    ����Ҫд��SCL�ĵ�ƽֵ����Χ��0/1
  * �� �� ֵ����
  * ˵    �������ϲ㺯����ҪдSCLʱ���˺����ᱻ����
  *           �û���Ҫ���ݲ��������ֵ����SCL��Ϊ�ߵ�ƽ���ߵ͵�ƽ
  *           ����������0ʱ����SCLΪ�͵�ƽ������������1ʱ����SCLΪ�ߵ�ƽ
  */
void IIC_W_SCL(uint8_t BitValue)
{
	/*����BitValue��ֵ����SCL�øߵ�ƽ���ߵ͵�ƽ*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, (GPIO_PinState)BitValue);
	
	/*�����Ƭ���ٶȹ��죬���ڴ����������ʱ���Ա��ⳬ��I2Cͨ�ŵ�����ٶ�*/
	//...
	IIC_Delay();
}

/**
  * ��    ����IICдSDA�ߵ͵�ƽ
  * ��    ����Ҫд��SDA�ĵ�ƽֵ����Χ��0/1
  * �� �� ֵ����
  * ˵    �������ϲ㺯����ҪдSDAʱ���˺����ᱻ����
  *           �û���Ҫ���ݲ��������ֵ����SDA��Ϊ�ߵ�ƽ���ߵ͵�ƽ
  *           ����������0ʱ����SDAΪ�͵�ƽ������������1ʱ����SDAΪ�ߵ�ƽ
  */
void IIC_W_SDA(uint8_t BitValue)
{
	/*����BitValue��ֵ����SDA�øߵ�ƽ���ߵ͵�ƽ*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (GPIO_PinState)BitValue);
	
	/*�����Ƭ���ٶȹ��죬���ڴ����������ʱ���Ա��ⳬ��I2Cͨ�ŵ�����ٶ�*/
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

/************************MEMS��ȡŨ��***********************/
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








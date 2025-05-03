#include "W25Q64.h"


/**
 * @brief    SPI����ָ�����ȵ�����
 * @param    buf  ���� �������ݻ������׵�ַ
 * @param    size ���� Ҫ�������ݵ��ֽ���
 * @retval   �ɹ�����HAL_OK
 */
static HAL_StatusTypeDef SPI_Transmit(uint8_t* send_buf, uint16_t size)
{
    return HAL_SPI_Transmit(&hspi1, send_buf, size, 100);
}


/**
 * @brief   SPI����ָ�����ȵ�����
 * @param   buf  ���� �������ݻ������׵�ַ
 * @param   size ���� Ҫ�������ݵ��ֽ���
 * @retval  �ɹ�����HAL_OK
 */
static HAL_StatusTypeDef SPI_Receive(uint8_t* recv_buf, uint16_t size)
{
   return HAL_SPI_Receive(&hspi1, recv_buf, size, 100);
}

/**
 * @brief   SPI�ڷ������ݵ�ͬʱ����ָ�����ȵ�����
 * @param   send_buf  ���� �������ݻ������׵�ַ
 * @param   recv_buf  ���� �������ݻ������׵�ַ
 * @param   size ���� Ҫ����/�������ݵ��ֽ���
 * @retval  �ɹ�����HAL_OK
 */
//static HAL_StatusTypeDef SPI_TransmitReceive(uint8_t* send_buf, uint8_t* recv_buf, uint16_t size)
//{
//   return HAL_SPI_TransmitReceive(&hspi1, send_buf, recv_buf, size, 100);
//}


/**
 * @brief   ��ȡFlash�ڲ���ID
 * @param   none
 * @retval  �ɹ�����device_id
 */
uint16_t W25QXX_ReadID(void)
{
    uint8_t recv_buf[2] = {0};    //recv_buf[0]���Manufacture ID, recv_buf[1]���Device ID
    uint16_t device_id = 0;
    uint8_t send_data[4] = {ManufactDeviceID_CMD,0x00,0x00,0x00};   //���������ݣ�����+��ַ
    
    /* ʹ��Ƭѡ */
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    /* ���Ͳ���ȡ���� */
    if (HAL_OK == SPI_Transmit(send_data, 4)) 
    {
        if (HAL_OK == SPI_Receive(recv_buf, 2)) 
        {
            device_id = (recv_buf[0] << 8) | recv_buf[1];
        }
    }
    
    /* ȡ��Ƭѡ */
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
    
    return device_id;
}

/**
 * @brief     ��ȡW25QXX��״̬�Ĵ�����W25Q64һ����2��״̬�Ĵ���
 * @param     reg  ���� ״̬�Ĵ������(1~2)
 * @retval    ״̬�Ĵ�����ֵ
 */
static uint8_t W25QXX_ReadSR(uint8_t reg)
{
    uint8_t result = 0; 
    uint8_t send_buf[4] = {0x00,0x00,0x00,0x00};
    switch(reg)
    {
        case 1:
            send_buf[0] = READ_STATU_REGISTER_1;
        case 2:
            send_buf[0] = READ_STATU_REGISTER_2;
        case 0:
        default:
            send_buf[0] = READ_STATU_REGISTER_1;
    }
    
     /* ʹ��Ƭѡ */
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    if (HAL_OK == SPI_Transmit(send_buf, 4)) 
    {
        if (HAL_OK == SPI_Receive(&result, 1)) 
        {
            HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
            
            return result;
        }
    }
    
    /* ȡ��Ƭѡ */
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief	�����ȴ�Flash���ڿ���״̬
 * @param   none
 * @retval  none
 */
static void W25QXX_Wait_Busy(void)
{
    while((W25QXX_ReadSR(1) & 0x01) == 0x01); // �ȴ�BUSYλ���
}

/**
 * @brief   ��ȡSPI FLASH����
 * @param   buffer      ���� ���ݴ洢��
 * @param   start_addr  ���� ��ʼ��ȡ�ĵ�ַ(���32bit)
 * @param   nbytes      ���� Ҫ��ȡ���ֽ���(���65535)
 * @retval  �ɹ�����0��ʧ�ܷ���-1
 */
int W25QXX_Read(uint8_t* buffer, uint32_t start_addr, uint16_t nbytes)
{
    uint8_t cmd = READ_DATA_CMD;
    
    start_addr = start_addr << 8;
    
	W25QXX_Wait_Busy();
    
     /* ʹ��Ƭѡ */
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    SPI_Transmit(&cmd, 1);
    
    if (HAL_OK == SPI_Transmit((uint8_t*)&start_addr, 3)) 
    {
        if (HAL_OK == SPI_Receive(buffer, nbytes)) 
        {
            HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
            return 0;
        }
    }
    
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
    return -1;
}

/**
 * @brief    W25QXXдʹ��,��S1�Ĵ�����WEL��λ
 * @param    none
 * @retval
 */
void W25QXX_Write_Enable(void)
{
    uint8_t cmd= WRITE_ENABLE_CMD;
    
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    SPI_Transmit(&cmd, 1);
    
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
    
    W25QXX_Wait_Busy();

}

/**
 * @brief    W25QXXд��ֹ,��WEL����
 * @param    none
 * @retval    none
 */
void W25QXX_Write_Disable(void)
{
    uint8_t cmd = WRITE_DISABLE_CMD;

    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    SPI_Transmit(&cmd, 1);
    
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
    
    W25QXX_Wait_Busy();
}

/**
 * @brief    W25QXX����һ������
 * @param   sector_addr    ���� ������ַ ����ʵ����������
 * @retval  none
 * @note    ��������
 */
void W25QXX_Erase_Sector(uint32_t sector_addr)
{
//    uint8_t cmd = SECTOR_ERASE_CMD;
//    
//    sector_addr *= 4096;    //ÿ������16��������ÿ�������Ĵ�С��4KB����Ҫ����Ϊʵ�ʵ�ַ
//    sector_addr <<= 8;
//    
//    W25QXX_Write_Enable();  //����������д��0xFF����Ҫ����дʹ��
//    W25QXX_Wait_Busy();        //�ȴ�дʹ�����
//   
//     /* ʹ��Ƭѡ */
//    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
//    
//    SPI_Transmit(&cmd, 1);
//    
//    SPI_Transmit((uint8_t*)&sector_addr, 3);
//    
//    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
//    
//    W25QXX_Wait_Busy();       //�ȴ������������
	
	
	

	uint8_t cmd[4] = {0};
	sector_addr *= 4096;
	
	W25QXX_Write_Enable();//дʹ��
	W25QXX_Wait_Busy();
	HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
	cmd[0] = SECTOR_ERASE_CMD;
	cmd[1] = ((uint8_t)(sector_addr>>16));
	cmd[2] = ((uint8_t)(sector_addr>>8));
	cmd[3] = ((uint8_t)sector_addr);
	HAL_SPI_Transmit(&hspi1,cmd,4,1000);
	HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
	W25QXX_Wait_Busy();

}
//оƬ����
void W25QXX_Erase(void)
{
    uint8_t cmd = CHIP_ERASE_CMD;
	
	W25QXX_Write_Enable();//дʹ��
	W25QXX_Wait_Busy();
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    SPI_Transmit(&cmd, 1);
    
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
    
    W25QXX_Wait_Busy();
}

/**
 * @brief    ҳд�����
 * @param    dat ���� Ҫд������ݻ������׵�ַ
 * @param    WriteAddr ���� Ҫд��ĵ�ַ
 * @param   byte_to_write ���� Ҫд����ֽ�����0-256��
 * @retval    none
 */
void W25QXX_Page_Program(uint8_t* dat, uint32_t WriteAddr, uint16_t nbytes)
{
    uint8_t cmd = PAGE_PROGRAM_CMD;
    
    WriteAddr <<= 8;
    
    W25QXX_Write_Enable();
    
    /* ʹ��Ƭѡ */
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    
    SPI_Transmit(&cmd, 1);

    SPI_Transmit((uint8_t*)&WriteAddr, 3);
    
    SPI_Transmit(dat, nbytes);
    
    HAL_GPIO_WritePin(W25Q64_CHIP_SELECT_GPIO_Port, W25Q64_CHIP_SELECT_Pin, GPIO_PIN_SET);
    
    W25QXX_Wait_Busy();
}







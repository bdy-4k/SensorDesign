/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dac.h"
#include "usart.h"
#include "adc.h"
#include "spi.h"
#include "i2c.h"
#include "tim.h"
#include "OLED.h"
#include "control.h"
#include "MEMS.h"
#include "UI.h"
#include "MY_IIC.h"
#include "KEY.h"
#include "W25Q64.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_SCL_Pin GPIO_PIN_10
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_11
#define OLED_SDA_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_12
#define KEY3_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_13
#define KEY2_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_14
#define KEY1_GPIO_Port GPIOB
#define PWR_CTR_Pin GPIO_PIN_15
#define PWR_CTR_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_6
#define LED_GPIO_Port GPIOC
#define BUZZ_Pin GPIO_PIN_7
#define BUZZ_GPIO_Port GPIOC
#define W25Q64_CHIP_SELECT_Pin GPIO_PIN_2
#define W25Q64_CHIP_SELECT_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
extern uint16_t ADC_Value[40];
extern uint16_t ADC_Filter_Value[4];
extern uint16_t Gas_MAX[4];
extern float c1,t1;
extern uint8_t Sectors_Num[2];
extern uint16_t Sec_Num;
extern uint16_t NH3_Value;
extern uint8_t read_buf[30];
extern uint8_t write_buf[30];
extern uint32_t Past_Data[8];
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define DRDY_Pin GPIO_PIN_2
#define DRDY_GPIO_Port GPIOE
#define CS_I2C_SPI_Pin GPIO_PIN_3
#define CS_I2C_SPI_GPIO_Port GPIOE
#define MEMS_INT3_Pin GPIO_PIN_4
#define MEMS_INT3_GPIO_Port GPIOE
#define MEMS_INT4_Pin GPIO_PIN_5
#define MEMS_INT4_GPIO_Port GPIOE
#define MEMS_INT4_EXTI_IRQn EXTI9_5_IRQn
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOF
#define SW20_TIM21_Pin GPIO_PIN_0
#define SW20_TIM21_GPIO_Port GPIOA
#define SW21_Pin GPIO_PIN_1
#define SW21_GPIO_Port GPIOA
#define SW10_TIM31_Pin GPIO_PIN_6
#define SW10_TIM31_GPIO_Port GPIOA
#define SW11_TIM32_Pin GPIO_PIN_7
#define SW11_TIM32_GPIO_Port GPIOA
#define ENU7_Pin GPIO_PIN_1
#define ENU7_GPIO_Port GPIOB
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOE
#define LD3_Pin GPIO_PIN_9
#define LD3_GPIO_Port GPIOE
#define LD5_Pin GPIO_PIN_10
#define LD5_GPIO_Port GPIOE
#define LD7_Pin GPIO_PIN_11
#define LD7_GPIO_Port GPIOE
#define LD9_Pin GPIO_PIN_12
#define LD9_GPIO_Port GPIOE
#define LD10_Pin GPIO_PIN_13
#define LD10_GPIO_Port GPIOE
#define LD8_Pin GPIO_PIN_14
#define LD8_GPIO_Port GPIOE
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOE
#define KEY4_Pin GPIO_PIN_8
#define KEY4_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_9
#define KEY3_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_10
#define KEY2_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_11
#define KEY1_GPIO_Port GPIOD
#define SW40_Pin GPIO_PIN_12
#define SW40_GPIO_Port GPIOD
#define SW41_Pin GPIO_PIN_13
#define SW41_GPIO_Port GPIOD
#define KEY5_Pin GPIO_PIN_14
#define KEY5_GPIO_Port GPIOD
#define SW50_Pin GPIO_PIN_6
#define SW50_GPIO_Port GPIOC
#define SW51_Pin GPIO_PIN_7
#define SW51_GPIO_Port GPIOC
#define IN1U4_Pin GPIO_PIN_9
#define IN1U4_GPIO_Port GPIOC
#define ENBU2_Pin GPIO_PIN_10
#define ENBU2_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define ENAU4_Pin GPIO_PIN_6
#define ENAU4_GPIO_Port GPIOF
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define IN4U2_Pin GPIO_PIN_15
#define IN4U2_GPIO_Port GPIOA
#define IN1U2_Pin GPIO_PIN_11
#define IN1U2_GPIO_Port GPIOC
#define IN2U2_Pin GPIO_PIN_12
#define IN2U2_GPIO_Port GPIOC
#define ENAU2_Pin GPIO_PIN_0
#define ENAU2_GPIO_Port GPIOD
#define IN3U3_Pin GPIO_PIN_1
#define IN3U3_GPIO_Port GPIOD
#define IN4U3_Pin GPIO_PIN_2
#define IN4U3_GPIO_Port GPIOD
#define ENBU3_Pin GPIO_PIN_3
#define ENBU3_GPIO_Port GPIOD
#define IN2U3_Pin GPIO_PIN_4
#define IN2U3_GPIO_Port GPIOD
#define IN1U3_Pin GPIO_PIN_5
#define IN1U3_GPIO_Port GPIOD
#define ENAU3_Pin GPIO_PIN_6
#define ENAU3_GPIO_Port GPIOD
#define ENBU4_Pin GPIO_PIN_7
#define ENBU4_GPIO_Port GPIOD
#define IN3U4_Pin GPIO_PIN_3
#define IN3U4_GPIO_Port GPIOB
#define IN4U4_Pin GPIO_PIN_4
#define IN4U4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

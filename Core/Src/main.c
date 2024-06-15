/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <usbd_cdc_if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim8;

/* USER CODE BEGIN PV */
	uint8_t PWM_DUTYCYCLE_MIN = 10;
	uint16_t PWM_DUTYCYCLE_MAX = 50;
	uint16_t PWM_PERIOD_TICKS = 50;
	uint16_t duty_cycle_1 = 0;
	uint16_t duty_cycle_2 = 0;
	uint16_t duty_cycle_3 = 0;
	uint16_t duty_cycle_4 = 0;
	uint16_t duty_cycle_5 = 0;

	float Kp_1 = 1; //1
	float Ki_1 = 0.01; //0.22; //0.12
	float Kd_1 = 3;//4; //15

	float Kp_2 = 1; //1.2
	float Ki_2 = 0.01; //0.5
	float Kd_2 = 3;

	float Kp_3 = 1;
	float Ki_3 = 0.01;
	float Kd_3 = 3;

	float Kp_45 = 1;
	float Ki_45 = 0.01;
	float Kd_45 = 3;

	float Kp_gr = 0;
	float Ki_gr = 0;
	float Kd_gr = 0;

	float error_current_1 = 0;
	float error_previous_1 = 0;
	float error_int_1 = 0;
	float error_dif_1 = 0;
	float U_1 = 0;

	float error_current_2 = 0;
	float error_previous_2 = 0;
	float error_int_2 = 0;
	float error_dif_2 = 0;
	float U_2 = 0;

	float error_current_3 = 0;
	float error_previous_3 = 0;
	float error_int_3 = 0;
	float error_dif_3 = 0;
	float U_3 = 0;

	float error_current_4 = 0;
	float error_previous_4 = 0;
	float error_int_4 = 0;
	float error_dif_4 = 0;
	float U_4 = 0;

	float error_current_5 = 0;
	float error_previous_5 = 0;
	float error_int_5 = 0;
	float error_dif_5 = 0;
	float U_5 = 0;

	float error_current_gr = 0;
	float error_previous_gr = 0;
	float error_int_gr = 0;
	float error_dif_gr = 0;
	float U_gr = 0;

	float v_error_current_1 = 0;
	float v_error_previous_1 = 0;
	float v_error_int_1 = 0;
	float v_error_dif_1 = 0;
	float v_U_1 = 0;

	int16_t deltaT = 0;
	int16_t currT = 0;

	volatile uint16_t dir_1 = 0;
	volatile uint16_t dir_2 = 0;
	volatile uint16_t dir_3 = 0;
	volatile uint16_t dir_4 = 0;
	volatile uint16_t dir_5 = 0;
	volatile uint16_t dir_gr = 0;

	uint8_t buffer[128];
	uint8_t rxFlag = 0;
	uint8_t homeFlag = 0;
	uint8_t stopFlag = 0;
	uint8_t velocity_flag = 0;

	char str_1[64];

	uint8_t axis_5 = 0;

	int32_t counter_1 = 0;
	int16_t count_1 = 0;

	int32_t counter_2 = 0;
	int16_t count_2 = 0;

	int32_t counter_3 = 0;
	int16_t count_3 = 0;

	int32_t counter_4 = 0;
	int16_t count_4 = 0;

	int32_t counter_5 = 0;
	int16_t count_5 = 0;

	int16_t count_6 = 0;

	float T5_prev = 0;
	float T4 = 0;
	float T5 = 0;
	float gr = 0;

	float T1_1 = 0;
	float T2_1 = 0;
	float T3_1 = 0;
	float T4_1 = 0;
	float T5_1 = 0;

	float velocity_1 = 0; /* deg/s */
	float velocity_2 = 0;
	float velocity_3 = 0;
	float velocity_4 = 0;
	float velocity_5 = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	counter_1 = TIM3->CNT;
	count_1 = (int16_t)counter_1;

	counter_2 = TIM2->CNT;
	count_2 = (int16_t)counter_2;

	counter_3 = TIM1->CNT;
	count_3 = (int16_t)counter_3;

	counter_4 = TIM4->CNT;
	count_4 = (int16_t)counter_4;

	counter_5 = TIM8->CNT;
	count_5 = (int16_t)counter_5;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);

  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);

  HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);

  HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);

  HAL_TIM_Encoder_Start_IT(&htim8, TIM_CHANNEL_ALL);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){

	  if (rxFlag == 1)
	  {
//		  ready_flag = 0;
		  if (buffer[0] == 'p')
		  {
			  char *token = strtok(buffer, " ");
			  while (token != NULL) {
				  int code = atoi(token);
			      switch (code) {
			          case 11:
			        	  sscanf(strtok(NULL, " "), "%f", &T1_1);
			              break;
			          case 12:
			        	  sscanf(strtok(NULL, " "), "%f", &T2_1);
			              break;
			          case 13:
			        	  sscanf(strtok(NULL, " "), "%f", &T3_1);
			              break;
			          case 14:
			        	  sscanf(strtok(NULL, " "), "%f", &T4_1);
			        	  T5_1 = -T4_1 + T5;
			        	  T4_1 = -T4_1 - T5;
			              break;
			          case 15:
			        	  sscanf(strtok(NULL, " "), "%f", &T5);
			        	  T4_1 = T4_1 - T5;
			        	  T5_1 = T5_1 + T5;
			        	  break;
			          case 21:
			        	  sscanf(strtok(NULL, " "), "%f", &velocity_1);
			              break;
			          case 22:
			        	  sscanf(strtok(NULL, " "), "%f", &velocity_2);
			              break;
			          case 23:
			        	  sscanf(strtok(NULL, " "), "%f", &velocity_3);
			              break;
			          case 24:
			        	  sscanf(strtok(NULL, " "), "%f", &velocity_4);
			              break;
			          case 25:
			        	  sscanf(strtok(NULL, " "), "%f", &velocity_5);
			        	  break;
			        }
			      token = strtok(NULL, " ");
			  }
		  }
		  else if (buffer[0] == '0')
		  {
			  char *token = strtok(buffer, " ");
			  while (token != NULL) {
				  int code = atoi(token);
			      switch (code) {
			          case 11:
			        	  sscanf(strtok(NULL, " "), "%f", &T1_1);
			              break;
			          case 12:
			        	  sscanf(strtok(NULL, " "), "%f", &T2_1);
			              break;
			          case 13:
			        	  sscanf(strtok(NULL, " "), "%f", &T3_1);
			              break;
			          case 14:
			        	  sscanf(strtok(NULL, " "), "%f", &T4_1);
			        	  T5_1 = -T4_1 + T5;
			        	  T4_1 = -T4_1 - T5;
			              break;
			          case 15:
			        	  sscanf(strtok(NULL, " "), "%f", &T5);
			        	  T4_1 = T4_1 - T5;
			        	  T5_1 = T5_1 + T5;
			        	  break;
			        }
			      token = strtok(NULL, " ");
			  }
		  }
		  else if (buffer[0] == '5')
		  {
			  char *token = strtok(buffer, " ");
			  while (token != NULL)
			  {
				  int code = atoi(token);
			      switch (code)
			      {
			          case 11:
			        	  sscanf(strtok(NULL, " "), "%f", &velocity_1);
			        	  break;
				      case 12:
				       	  sscanf(strtok(NULL, " "), "%f", &velocity_2);
				          break;
				      case 13:
				       	  sscanf(strtok(NULL, " "), "%f", &velocity_3);
				          break;
				      case 14:
				       	  sscanf(strtok(NULL, " "), "%f", &velocity_4);
				          break;
				      case 15:
				       	  sscanf(strtok(NULL, " "), "%f", &velocity_5);
				       	  break;
			      }
				      token = strtok(NULL, " ");
			  }
		  }
		  else if (buffer[0]== '3' && buffer [1] =='1')
		  {
			  homeFlag = 1;
		  }
		  else if (buffer [0] == '9' && buffer [1] == '9')
		  {
			  if (stopFlag == 0)
			  {
				  stopFlag = 1;
			  }
			  else
			  {
				  stopFlag = 0;
			  }
		  }
		  else if (buffer[0]=='1')
		  {
			  if (buffer[1] == '1')
			  {
				  sscanf(buffer, "%*d %f", &T1_1);
			  }
			  if (buffer[1] =='2')
			  {
				  sscanf(buffer, "%*d %f", &T2_1);
			  }
			  if (buffer[1] =='3')
			  {
				  sscanf(buffer, "%*d %f", &T3_1);
			  }
			  if (buffer[1] =='5')
			  {
				  sscanf(buffer, "%*d %f", &T5);
				  T4_1 = T4_1 - (T5 - T5_prev);
				  T5_1 = T5_1 + (T5 - T5_prev);
				  T5_prev = T5;
			  }
			  if (buffer[1] =='4')
			  {
			  	  sscanf(buffer, "%*d %f", &T4_1);
			  	  T5_1 = - T4_1 + T5;
			  	  T4_1 = - T4_1 - T5;
			  }
			  else if (buffer[1] == '6')
			  {
//				  sscanf(buffer, "%*d %f", &gr);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
				  HAL_Delay(3000);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			  }
			  if (buffer[1] == '7')
			  {
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_Delay(3000);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			  }
		 	 }

		  else if (buffer[0]== '6')
		 	 {
				  if (buffer[1] == '1')
				  {
					  sscanf(buffer, "%*d %f", &velocity_1);
				  }
				  else if (buffer[1] =='2')
				  {
					  sscanf(buffer, "%*d %f", &velocity_2);
				  }
				  else if (buffer[1] =='3')
				  {
					  sscanf(buffer, "%*d %f", &velocity_3);
				  }
				  else if (buffer[1] =='4')
				  {
				  	  sscanf(buffer, "%*d %f", &velocity_4);
				  }
				  else if (buffer[1] =='5')
				  {
					  sscanf(buffer, "%*d %f", &velocity_5);
				  }
				  velocity_flag = 1;
		 	 }
		 rxFlag = 0;
	  }

	if (stopFlag == 1)
	{
		T1_1 = count_1;
		T2_1 = count_2;
		T3_1 = count_3;
		T4_1 = count_4;
		T5_1 = count_5;
		velocity_1 = 0;
		velocity_2 = 0;
		velocity_3 = 0;
		velocity_4 = 0;
		velocity_5 = 0;
	}

	if (velocity_flag == 1)
			{
			if (velocity_1 > 0)
			{
				duty_cycle_1 = velocity_1*2.5;
				dir_1 = 1;
			}
			else if (velocity_1 < 0)
			{
				duty_cycle_1 = -velocity_1*2.5;
				dir_1 = 2;
			}
			else if (velocity_1 == 0)
			{
				duty_cycle_1 = 0;
				dir_1 = 0;
			}

			if (velocity_2 > 0)
			{
				duty_cycle_2 = velocity_2*2.5;
				dir_2 = 2;
			}
			else if (velocity_2 < 0)
			{
				duty_cycle_2 = -velocity_2*2.5;
				dir_2 = 1;
			}
			else if (velocity_2 == 0)
			{
				duty_cycle_2 = 0;
				dir_2 = 0;
			}

			if (velocity_3 > 0)
			{
				duty_cycle_3 = velocity_3*2.5;
				dir_3 = 1;
			}
			else if (velocity_3 < 0)
			{
				duty_cycle_3 = -velocity_3*2.5;
				dir_3 = 2;
			}
			else if (velocity_3 == 0)
			{
				duty_cycle_3 = 0;
				dir_3 = 0;
			}

//			if (velocity_4 > 0)
//			{
//				duty_cycle_4 = velocity_4*2.5;
//				dir_4 = 1;
//				duty_cycle_5 = velocity_4*2.5;
//				dir_5 = 1;
//			}
//			else if (velocity_4 < 0)
//			{
//				duty_cycle_4 = -velocity_4*2.5;
//				dir_4 = 2;
//				duty_cycle_5 = -velocity_4*2.5;
//				dir_5 = 2;
//			}
//			else if (velocity_4 == 0)
//			{
//				duty_cycle_4 = 0;
//				dir_4 = 0;
//				duty_cycle_5 = 0;
//				dir_5 = 0;
//			}
			if (velocity_4 > 0)
			{
				duty_cycle_4 = velocity_4*2.5;
				dir_4 = 1;
				duty_cycle_5 = velocity_4*2.5;
				dir_5 = 1;
			}
			else if (velocity_4 < 0)
			{
				duty_cycle_4 = -velocity_4*2.5;
				dir_4 = 2;
				duty_cycle_5 = -velocity_4*2.5;
				dir_5 = 2;
			}
			else if (velocity_4 == 0 && velocity_5 == 0)
			{
				duty_cycle_4 = 0;
				dir_4 = 0;
				duty_cycle_5 = 0;
				dir_5 = 0;
			}
			if (velocity_5 > 0)
			{
				duty_cycle_4 = velocity_5*2.5;
				dir_4 = 1;
				duty_cycle_5 = velocity_5*2.5;
				dir_5 = 2;
			}
			else if (velocity_5 < 0)
			{
				duty_cycle_4 = -velocity_5*2.5;
				dir_4 = 2;
				duty_cycle_5 = -velocity_5*2.5;
				dir_5 = 1;
			}
			else if (velocity_5 == 0 && velocity_4 == 0)
			{
				duty_cycle_4 = 0;
				dir_4 = 0;
				duty_cycle_5 = 0;
				dir_5 = 0;
			}
			T1_1 = count_1;
			T2_1 = count_2;
			T3_1 = count_3;
			T4_1 = count_4;
			T5_1 = count_5;

			if (velocity_1 == 0 && velocity_2 == 0 && velocity_3 == 0 && velocity_4 == 0 && velocity_5 == 0)
			{
				velocity_flag = 0;
			}
		}
	if (deltaT >= 5 && stopFlag == 0 && velocity_flag == 0)
	{
		/* PID for 1 axis */

		error_current_1 = T1_1 - count_1;
		if (error_int_1 < 10 && error_int_1 >-10)
		{
			error_int_1 += error_current_1 * deltaT;
		}
		error_dif_1 = (error_current_1 - error_previous_1)/deltaT;
		U_1 = Kp_1 * error_current_1 + Ki_1*error_int_1 + Kd_1*error_dif_1;
		if (U_1 > 0)
		{
			duty_cycle_1 = (uint16_t)U_1;
		}
		else
		{
			duty_cycle_1 = -(int16_t)U_1;
		}

		if (U_1> 5)
		{
			dir_1 = 1;
		}
		else if (U_1 < -5)
		{
			dir_1 = 2;
		}
		else if (U_1 <= 5 && U_1 >= -5)
		{
			dir_1 = 0;
		}
//		if (velocity_1 > 0)
//		{
//		if (duty_cycle_1 > velocity_1 * 2.5)
//		{
//			duty_cycle_1 = velocity_1*2.5;
//		}
//		}
//		else if (velocity_1 < 0)
//		{
//			if (duty_cycle_1 > (-(int16_t)velocity_1 * 2.5))
//			{
//				duty_cycle_1 = -(int16_t)velocity_1*2.5;
//			}
//		}
		if (duty_cycle_1 < PWM_DUTYCYCLE_MIN)
		{
			duty_cycle_1 = PWM_DUTYCYCLE_MIN;
		}
		error_previous_1 = error_current_1;

/* PID for 2 axis */

		error_current_2 = T2_1 - count_2;
		if (error_int_2 < 10 && error_int_2 > -10)
		{
			error_int_2 += error_current_2 * deltaT;
		}
		error_dif_2 = (error_current_2 - error_previous_2)/deltaT;
		U_2 = Kp_2 * error_current_2 + Ki_2*error_int_2 + Kd_2*error_dif_2;
		if (U_2 > 0)
		{
			duty_cycle_2 = (uint16_t)U_2;
		}
		else
		{
			duty_cycle_2 = -(int16_t)U_2;
		}
		if (U_2> 5)
		{
			dir_2 = 2;
		}
		else if (U_2 < -5)
		{
			dir_2 = 1;
		}
		else if (U_2 <= 5 && U_2 >= -5)
		{
			dir_2 = 0;
		}
//		if (velocity_2 > 0)
//		{
//		if (duty_cycle_2 > velocity_2 * 2.5)
//		{
//			duty_cycle_2 = velocity_2*2.5;
//		}
//		}
//		else if (velocity_2 < 0)
//		{
//			if (duty_cycle_2 > (-(int16_t)velocity_2 * 2.5))
//			{
//				duty_cycle_2 = -(int16_t)velocity_2*2.5;
//			}
//		}
		if (duty_cycle_2 < PWM_DUTYCYCLE_MIN)
		{
			duty_cycle_2 = PWM_DUTYCYCLE_MIN;
		}
		error_previous_2 = error_current_2;
//
///* PID for 3 axis*/
//
		error_current_3 = T3_1 - count_3;
		if (error_int_3 < 10 && error_int_3 > -10)
		{
			error_int_3 += error_current_3 * deltaT;
		}
		error_dif_3 = (error_current_3 - error_previous_3)/deltaT;
		U_3 = Kp_3 * error_current_3 + Ki_3*error_int_3 + Kd_3*error_dif_3;
		if (U_3 > 0)
		{
			duty_cycle_3 = (uint16_t)U_3;
		}
		else
		{
			duty_cycle_3 = -(int16_t)U_3;
		}
		if (U_3> 5)
		{
			dir_3 = 1;
		}
		else if (U_3 < -5)
		{
			dir_3 = 2;
		}
		else if (U_3 <= 5 && U_3 >= -5)
		{
			dir_3 = 0;
		}
//		if (velocity_3 > 0)
//		{
//		if (duty_cycle_3 > velocity_3 * 2.5)
//		{
//			duty_cycle_3 = velocity_3*2.5;
//		}
//		}
//		else if (velocity_3 < 0)
//		{
//			if (duty_cycle_3 > (-(int16_t)velocity_3 * 2.5))
//			{
//				duty_cycle_3 = -(int16_t)velocity_3*2.5;
//			}
//		}
		if (duty_cycle_3 < PWM_DUTYCYCLE_MIN)
		{
			duty_cycle_3 = PWM_DUTYCYCLE_MIN;
		}
		error_previous_3 = error_current_3;
//
///* PID for pitch and roll*/
//
		error_current_4 = T4_1 - count_4;
		if (error_int_4 < 10 && error_int_4 > -10)
		{
			error_int_4 += error_current_4 * deltaT;
		}
		error_dif_4 = (error_current_4 - error_previous_4)/deltaT;
		U_4 = Kp_45 * error_current_4 + Ki_45*error_int_4 + Kd_45*error_dif_4;
		if (U_4 > 0)
		{
			duty_cycle_4 = (uint16_t)U_4;
		}
		else
		{
			duty_cycle_4 = -(int16_t)U_4;
		}
		if (U_4> 5)
		{
			dir_4 = 1;
		}
		else if (U_4 < -5)
		{
			dir_4 = 2;
		}
		else if (U_4 <= 5 && U_4 >= -5)
		{
			dir_4 = 0;
		}
		if (duty_cycle_4 > PWM_DUTYCYCLE_MAX)
		{
			duty_cycle_4 = PWM_DUTYCYCLE_MAX;
		}
		if (duty_cycle_4 < PWM_DUTYCYCLE_MIN)
		{
			duty_cycle_4 = PWM_DUTYCYCLE_MIN;
		}
		error_previous_4 = error_current_4;

		error_current_5 = T5_1 - count_5;
		if (error_int_5 < 10 && error_int_5 > -10)
		{
			error_int_5 += error_current_5 * deltaT;
		}
		else
		error_dif_5 = (error_current_5 - error_previous_5)/deltaT;
		U_5 = Kp_45 * error_current_5 + Ki_45*error_int_5 + Kd_45*error_dif_5;
		if (U_5 > 0)
		{
			duty_cycle_5 = (uint16_t)U_5;
		}
		else
		{
			duty_cycle_5 = -(int16_t)U_5;
		}
		if (U_5> 5)
		{
			dir_5 = 1;
		}
		else if (U_5 < -5)
		{
			dir_5 = 2;
		}
		else if (U_5 <= 5 && U_5 >= -5)
		{
			dir_5 = 0;
		}

		if (duty_cycle_5 > PWM_DUTYCYCLE_MAX)
		{
			duty_cycle_5 = PWM_DUTYCYCLE_MAX;
		}
		if (duty_cycle_5 < PWM_DUTYCYCLE_MIN)
		{
			duty_cycle_5 = PWM_DUTYCYCLE_MIN;
		}
		error_previous_5 = error_current_5;

/*		error_current_gr = gr - count_6;
		if (error_int_gr < 10 && error_int_gr > -10)
		{
			error_int_gr += error_current_gr * deltaT;
		}
		error_dif_gr = (error_current_gr - error_previous_gr)/deltaT;
		U_gr = Kp_gr * error_current_gr + Ki_gr*error_int_gr + Kd_gr*error_dif_gr;
		if (U_gr > 0)
		{
			duty_cycle_gr = (uint16_t)U_gr;
		}
		else
		{
			duty_cycle_gr = -(int16_t)U_gr;
		}
		if (U_gr < count_6)
		{
			dir_gr = 1;
		}
		if (U_gr > count_6)
		{
			dir_gr = 2;
		}

		if (duty_cycle_gr > PWM_DUTYCYCLE_MAX)
		{
			duty_cycle_gr = PWM_DUTYCYCLE_MAX;
		}
		if (duty_cycle_gr < PWM_DUTYCYCLE_MIN)
		{
			duty_cycle_gr = PWM_DUTYCYCLE_MIN;
		}
		error_previous_gr = error_current_gr;
*/
		deltaT = 0;

}

	if (currT % 10 == 0)
	{
		sprintf(str_1, "%d %d %d %d %d\n", count_1,count_2,count_3, count_4, count_5);
		CDC_Transmit_FS((uint8_t*)str_1, strlen(str_1));
		HAL_Delay(1);
		currT = 0;
	}

//		sprintf(str_1, "1 %d, %d, %d, %d\n", velocity_1_count, rpm_1, duty_cycle_1, count_1);
//		CDC_Transmit_FS((uint8_t*)str_1, strlen(str_1));
//		HAL_Delay(1);

//		sprintf(str_1, "2 %d, %d, %d, %d\n", velocity_2, rpm_2, duty_cycle_2, count_2);
//		CDC_Transmit_FS((uint8_t*)str_1, strlen(str_1));
//		HAL_Delay(1);
//
//		sprintf(str_1, "3 %d, %d, %d, %d\n", velocity_3, rpm_3, duty_cycle_3, count_3);
//		CDC_Transmit_FS((uint8_t*)str_1, strlen(str_1));
//		HAL_Delay(1);
//
//		sprintf(str_1, "45 %d, %d, %d, %d\n", velocity_45, rpm_45, duty_cycle_4, count_4);
//		CDC_Transmit_FS((uint8_t*)str_1, strlen(str_1));
//		HAL_Delay(1);

//	if (U_1 <= 2 && U_1 >= -2 && U_2 <= 2 && U_2 >= -2 && U_3 <= 2 && U_3 >= -2 && U_4 <= 2 && U_4 >= -2 && U_5 <= 2 && U_5 >= -2 && ready_flag == 0)
//	{
//		CDC_Transmit_FS((uint8_t*)str_rd, strlen(str_rd));
//		ready_flag = 1;
//	}


/* HOME */
	if (homeFlag == 1 && stopFlag == 0)
	{
			/* while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == GPIO_PIN_SET){
						  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
						  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
						  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
						  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
					  }
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
					  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
			HAL_Delay(500);
			*/

		while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_SET)
		{
			if (rxFlag == 1)
			{
				if (buffer [0] == '9' && buffer [1] == '9')
				{
					stopFlag = 1;
				}
			}
			if (stopFlag == 1)
			{
				break;
			}
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
		}
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);

		while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET)
		{
			if (rxFlag == 1)
			{
				if (buffer [0] == '9' && buffer [1] == '9')
				{
					stopFlag = 1;
				}
			}
			if (stopFlag == 1)
			{
				break;
			}
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		  }
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//		HAL_Delay(500);

		while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET){
		if (rxFlag == 1)
		{
			if (buffer [0] == '9' && buffer [1] == '9')
			{
				stopFlag = 1;
			}
		}
		if (stopFlag == 1)
		{
			break;
		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
//		HAL_Delay(500);

		while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_SET){
		if (rxFlag == 1)
		{
			if (buffer [0] == '9' && buffer [1] == '9')
			{
			stopFlag = 1;
			}
		}
		if (stopFlag == 1)
		{
			break;
		}
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		}
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
//		HAL_Delay(500);


		while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_SET){
			if (rxFlag == 1)
			{
				if (buffer [0] == '9' && buffer [1] == '9')
				{
					stopFlag = 1;
				}
			}
			if (stopFlag == 1)
			{
				break;
			}
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
		}
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
//		HAL_Delay(500);

		if (stopFlag == 0 )
		{
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			HAL_Delay(2000);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			HAL_Delay(100);
		}
		if (stopFlag == 0)
		{
			TIM1->CNT = 0;
			TIM2->CNT = 0;
			TIM3->CNT = 0;
			TIM4->CNT = 0;
			TIM8->CNT = 0;
			count_6 = 0;

			T1_1 = 0;
			T2_1 = 0;
			T3_1 = 0;
			T4_1 = 0;
			T5_1 = 0;
			gr = 0;

			dir_1 = 0;
			dir_2 = 0;
			dir_3 = 0;
			dir_4 = 0;
			dir_5 = 0;
			dir_gr = 0;
		}
		homeFlag = 0;
		rxFlag = 0;
		deltaT = 0;
	  }
  }
}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_TIM8;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 8;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 8;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 8;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 8;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 8;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 8;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 8;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 8;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 65535;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 8;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 8;
  if (HAL_TIM_Encoder_Init(&htim8, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|ENBU2_Pin|IN4U2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ENU7_Pin|IN3U4_Pin|IN4U4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, IN1U4_Pin|IN1U2_Pin|IN2U2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ENAU4_GPIO_Port, ENAU4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ENAU2_Pin|IN3U3_Pin|IN4U3_Pin|ENBU3_Pin
                          |IN2U3_Pin|IN1U3_Pin|ENAU3_Pin|ENBU4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PF9 PF10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 ENBU2_Pin IN4U2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|ENBU2_Pin|IN4U2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ENU7_Pin IN3U4_Pin IN4U4_Pin */
  GPIO_InitStruct.Pin = ENU7_Pin|IN3U4_Pin|IN4U4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY4_Pin KEY3_Pin KEY2_Pin KEY1_Pin
                           KEY5_Pin */
  GPIO_InitStruct.Pin = KEY4_Pin|KEY3_Pin|KEY2_Pin|KEY1_Pin
                          |KEY5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : IN1U4_Pin IN1U2_Pin IN2U2_Pin */
  GPIO_InitStruct.Pin = IN1U4_Pin|IN1U2_Pin|IN2U2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ENAU4_Pin */
  GPIO_InitStruct.Pin = ENAU4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENAU4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ENAU2_Pin IN3U3_Pin IN4U3_Pin ENBU3_Pin
                           IN2U3_Pin IN1U3_Pin ENAU3_Pin ENBU4_Pin */
  GPIO_InitStruct.Pin = ENAU2_Pin|IN3U3_Pin|IN4U3_Pin|ENBU3_Pin
                          |IN2U3_Pin|IN1U3_Pin|ENAU3_Pin|ENBU4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1){
  }
}


  /* USER CODE END Error_Handler_Debug */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

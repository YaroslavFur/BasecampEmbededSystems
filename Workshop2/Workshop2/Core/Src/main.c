/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int isOn = 1;
int scheme = 1;
int customDelay = 500;
const uint16_t roundPins[] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 };

void setPin(uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_RESET)
		HAL_GPIO_WritePin(GPIOD, GPIO_Pin, GPIO_PIN_SET);
}

void resetPin(uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_SET)
		HAL_GPIO_WritePin(GPIOD, GPIO_Pin, GPIO_PIN_RESET);
}

void setAllPins()
{
	for (int i = 0; i < 4; i++)
	{
		setPin(roundPins[i]);
	}
}

void resetAllPins()
{
	for (int i = 0; i < 4; i++)
	{
		resetPin(roundPins[i]);
	}
}

// this function delays for customDelay time and checks whether it's time to stop blinking
int delayAndCheck(int blinkingScheme)
{
	HAL_Delay(customDelay);
	if (isOn != 1 || blinkingScheme != scheme)
		return 1;
	return 0;
}


// first blinking scheme
void blink1()
{
	int round = 0;
	while (1)
	{
		round -= 1;		// one pin back

		if (round < 0)	// it's for not to exceed the array borders
			round += 4;

		resetPin(roundPins[round]);

		round += 2;		// two pins forward

		if (round > 3)	// it's for not to exceed the array borders
			round -= 4;

		setPin(roundPins[round]);

		if (delayAndCheck(1))
			break;
	}
	resetAllPins();		// for excessive safety
}

// second blinking scheme
void blink2()
{
	while (1)	// I made second scheme as a linear algorithm
	{
		setPin(roundPins[1]);
		setPin(roundPins[3]);

		if (delayAndCheck(2))
			break;

		resetPin(roundPins[1]);
		resetPin(roundPins[3]);

		setPin(roundPins[0]);
		setPin(roundPins[2]);

		if (delayAndCheck(2))
			break;

		setPin(roundPins[1]);
		setPin(roundPins[3]);

		if (delayAndCheck(2))
			break;

		resetAllPins();

		if (delayAndCheck(2))
			break;
	}
	resetAllPins();		// for excessive safety

}

// third blinking scheme
void blink3()
{
	int round;
	while (1)
	{
		round = 0;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				setPin(roundPins[round]);	// set pin
				if (delayAndCheck(3))
					break;

				resetPin(roundPins[round]);	// and reset it
				if (delayAndCheck(3))
					break;

				round += 2;					// then take opposite and do the same

				if (round > 3)	// it's for not to exceed the array borders
					round -= 4;
			}
			if (isOn != 1 || scheme != 3)
				break;

			round += 1;						// then take another 2 pins and do the same

			if (round > 3)		// it's for not to exceed the array borders
				round -= 4;
		}
		if (isOn != 1 || scheme != 3)
			break;

		setAllPins();						// set all pins

		if (delayAndCheck(3))
			break;

		resetAllPins();						// and reset all pins

		if (delayAndCheck(3))
			break;
	}
	resetAllPins();		// for excessive safety

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (isOn == 1)
	  {
		  if (scheme == 1)
			  blink1();

		  if (scheme == 2)
			  blink2();

		  if (scheme == 3)
			  blink3();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC8 PC9 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

// This function is executed each time button interruption happens
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	if (GPIO_PIN == GPIO_PIN_15)	// on/off scheme
	{
		if (isOn == 1)
		{
			isOn = 0;
			resetAllPins();
		}
		else
			isOn = 1;
	}
	if (GPIO_PIN == GPIO_PIN_6)		// increase speed
	{
		if (customDelay < 1000)
			customDelay *= 2;
	}
	if (GPIO_PIN == GPIO_PIN_8)		// decrease speed
	{
		if (customDelay > 100)
			customDelay /= 2;
	}
	if (GPIO_PIN == GPIO_PIN_9)		// next scheme
	{
		if (scheme < 3)
		{
			scheme++;
		}
		else
		{
			scheme = 1;
		}
		resetAllPins();
	}
	if (GPIO_PIN == GPIO_PIN_11)	// previous scheme
	{
		if (scheme > 1)
		{
			scheme--;
		}
		else
		{
			scheme = 3;
		}
		resetAllPins();
	}
}

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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

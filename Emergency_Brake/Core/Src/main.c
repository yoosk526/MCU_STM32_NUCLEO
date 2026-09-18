/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
# define WARNING		(1U << 0)
# define BRAKE_TIMEOUT	(1U << 1)
# define EMERGENCY		(1U << 2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask",
		.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for SpeedTask */
osThreadId_t SpeedTaskHandle;
const osThreadAttr_t SpeedTask_attributes = { .name = "SpeedTask", .priority =
		(osPriority_t) osPriorityLow1, .stack_size = 128 * 4 };
/* Definitions for DistanceTask */
osThreadId_t DistanceTaskHandle;
const osThreadAttr_t DistanceTask_attributes = { .name = "DistanceTask",
		.priority = (osPriority_t) osPriorityLow, .stack_size = 128 * 4 };
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes =
		{ .name = "ControlTask", .priority =
				(osPriority_t) osPriorityBelowNormal, .stack_size = 128 * 4 };
/* Definitions for WarningTask */
osThreadId_t WarningTaskHandle;
const osThreadAttr_t WarningTask_attributes = { .name = "WarningTask",
		.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for BrakeTask */
osThreadId_t BrakeTaskHandle;
const osThreadAttr_t BrakeTask_attributes = { .name = "BrakeTask", .priority =
		(osPriority_t) osPriorityHigh, .stack_size = 128 * 4 };
/* Definitions for SpeedQueue */
osMessageQueueId_t SpeedQueueHandle;
const osMessageQueueAttr_t SpeedQueue_attributes = { .name = "SpeedQueue" };
/* Definitions for DistanceQueue */
osMessageQueueId_t DistanceQueueHandle;
const osMessageQueueAttr_t DistanceQueue_attributes =
		{ .name = "DistanceQueue" };
/* Definitions for ResponseTimer */
osTimerId_t ResponseTimerHandle;
const osTimerAttr_t ResponseTimer_attributes = { .name = "ResponseTimer" };
/* Definitions for SafetyEventFlags */
osEventFlagsId_t SafetyEventFlagsHandle;
const osEventFlagsAttr_t SafetyEventFlags_attributes = { .name =
		"SafetyEventFlags" };
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void StartSpeedTask(void *argument);
void StartDistanceTask(void *argument);
void StartControlTask(void *argument);
void StartWarningTask(void *argument);
void StartBrakeTask(void *argument);
void ResponseTimerCallback(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void uart_print(const char *s) {
	HAL_UART_Transmit(&huart2, (uint8_t*) s, (uint16_t) strlen(s), 100);
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* Create the timer(s) */
	/* creation of ResponseTimer */
	ResponseTimerHandle = osTimerNew(ResponseTimerCallback, osTimerOnce, NULL,
			&ResponseTimer_attributes);

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of SpeedQueue */
	SpeedQueueHandle = osMessageQueueNew(16, sizeof(uint16_t),
			&SpeedQueue_attributes);

	/* creation of DistanceQueue */
	DistanceQueueHandle = osMessageQueueNew(16, sizeof(uint16_t),
			&DistanceQueue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* creation of SpeedTask */
	SpeedTaskHandle = osThreadNew(StartSpeedTask, NULL, &SpeedTask_attributes);

	/* creation of DistanceTask */
	DistanceTaskHandle = osThreadNew(StartDistanceTask, NULL,
			&DistanceTask_attributes);

	/* creation of ControlTask */
	ControlTaskHandle = osThreadNew(StartControlTask, NULL,
			&ControlTask_attributes);

	/* creation of WarningTask */
	WarningTaskHandle = osThreadNew(StartWarningTask, NULL,
			&WarningTask_attributes);

	/* creation of BrakeTask */
	BrakeTaskHandle = osThreadNew(StartBrakeTask, NULL, &BrakeTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* Create the event(s) */
	/* creation of SafetyEventFlags */
	SafetyEventFlagsHandle = osEventFlagsNew(&SafetyEventFlags_attributes);

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_GREEN_Pin */
	GPIO_InitStruct.Pin = LED_GREEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartSpeedTask */
/**
 * @brief Function implementing the SpeedTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSpeedTask */
void StartSpeedTask(void *argument) {
	/* USER CODE BEGIN StartSpeedTask */
	uint16_t speed = 0;
	uint8_t stopped = 0;
	char msg[64];

	/* Infinite loop */
	for (;;) {
		if (stopped == 0) {
			snprintf(msg, sizeof(msg), "[Speed] %u km/h\r\n",
					(unsigned int) speed);
			uart_print(msg);

			osMessageQueuePut(SpeedQueueHandle, &speed, 0, 0);

			if ((osEventFlagsGet(SafetyEventFlagsHandle) & EMERGENCY) != 0) {
				if (speed > 0) {
					speed -= 10;
				}

				if (speed == 0) {
					uart_print("[System] Vehicle Stopped\r\n");
					stopped = 1;
				}
			}

			else {
				if (speed < 60) {
					speed += 10;
				}
			}
		}

		osDelay(1000);
	}
	/* USER CODE END StartSpeedTask */
}

/* USER CODE BEGIN Header_StartDistanceTask */
/**
 * @brief Function implementing the DistanceTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDistanceTask */
void StartDistanceTask(void *argument) {
	/* USER CODE BEGIN StartDistanceTask */
	uint16_t distance = 50;
	char msg[64];

	/* Infinite loop */
	for (;;) {
		if ((osEventFlagsGet(SafetyEventFlagsHandle) & EMERGENCY) == 0) {
			snprintf(msg, sizeof(msg), "[Distance] %u m\r\n",
					(unsigned int) distance);

			uart_print(msg);

			osMessageQueuePut(DistanceQueueHandle, &distance, 0, 0);

			if (distance > 10) {
				distance -= 5;
			}
		}

		osDelay(1000);
	}
	/* USER CODE END StartDistanceTask */
}

/* USER CODE BEGIN Header_StartControlTask */
/**
 * @brief Function implementing the ControlTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument) {
	/* USER CODE BEGIN StartControlTask */
	uint16_t speed = 0;
	uint16_t distance = 50;
	uint8_t warning_set = 0;

	/* Infinite loop */
	for (;;) {
		osMessageQueueGet(SpeedQueueHandle, &speed, NULL, osWaitForever);
		osMessageQueueGet(DistanceQueueHandle, &distance, NULL, osWaitForever);

		if ((speed >= 40) && (distance <= 30) && (warning_set == 0)) {
			osEventFlagsSet(SafetyEventFlagsHandle, WARNING);
			warning_set = 1;
		}

		osDelay(100);
	}
	/* USER CODE END StartControlTask */
}

/* USER CODE BEGIN Header_StartWarningTask */
/**
 * @brief Function implementing the WarningTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartWarningTask */
void StartWarningTask(void *argument) {
	/* USER CODE BEGIN StartWarningTask */
	/* Infinite loop */
	for (;;) {
		osEventFlagsWait(SafetyEventFlagsHandle,
						 WARNING,
						 osFlagsWaitAny | osFlagsNoClear,
						 osWaitForever);

		uart_print("[Warning] Forward Collision Warning\r\n");
		uart_print("[Timer] Driver Response Timer Start\r\n");

		osTimerStart(ResponseTimerHandle, 3000);

		for (;;) {
			osDelay(1000);
		}
	}
	/* USER CODE END StartWarningTask */
}

/* USER CODE BEGIN Header_StartBrakeTask */
/**
 * @brief Function implementing the BrakeTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartBrakeTask */
void StartBrakeTask(void *argument) {
	/* USER CODE BEGIN StartBrakeTask */
	/* Infinite loop */
	for (;;) {
		osEventFlagsWait(SafetyEventFlagsHandle,
						 BRAKE_TIMEOUT,
						 osFlagsWaitAny | osFlagsNoClear,
						 osWaitForever);

		uart_print("[Timer] Driver Response Timeout\r\n");
		uart_print("[Brake] Emergency Brake Activated\r\n");

		osEventFlagsSet(SafetyEventFlagsHandle, EMERGENCY);

		/* 한 번만 수행 */
		for (;;) {
			osDelay(1000);
		}
	}
	/* USER CODE END StartBrakeTask */
}

/* ResponseTimerCallback function */
void ResponseTimerCallback(void *argument) {
	/* USER CODE BEGIN ResponseTimerCallback */
	osEventFlagsSet(SafetyEventFlagsHandle, BRAKE_TIMEOUT);
	/* USER CODE END ResponseTimerCallback */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TASK1 */
osThreadId_t TASK1Handle;
const osThreadAttr_t TASK1_attributes = {
  .name = "TASK1",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for TASK2 */
osThreadId_t TASK2Handle;
const osThreadAttr_t TASK2_attributes = {
  .name = "TASK2",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for LP */
osThreadId_t LPHandle;
const osThreadAttr_t LP_attributes = {
  .name = "LP",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for MP */
osThreadId_t MPHandle;
const osThreadAttr_t MP_attributes = {
  .name = "MP",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for HP */
osThreadId_t HPHandle;
const osThreadAttr_t HP_attributes = {
  .name = "HP",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for MyTask */
osThreadId_t MyTaskHandle;
const osThreadAttr_t MyTask_attributes = {
  .name = "MyTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for PrintMutex */
osMutexId_t PrintMutexHandle;
const osMutexAttr_t PrintMutex_attributes = {
  .name = "PrintMutex"
};
/* Definitions for ResourceMutex */
osMutexId_t ResourceMutexHandle;
const osMutexAttr_t ResourceMutex_attributes = {
  .name = "ResourceMutex"
};
/* Definitions for MyMutex */
osMutexId_t MyMutexHandle;
const osMutexAttr_t MyMutex_attributes = {
  .name = "MyMutex"
};
/* Definitions for BinarySem */
osSemaphoreId_t BinarySemHandle;
const osSemaphoreAttr_t BinarySem_attributes = {
  .name = "BinarySem"
};
/* Definitions for ResourceSem */
osSemaphoreId_t ResourceSemHandle;
const osSemaphoreAttr_t ResourceSem_attributes = {
  .name = "ResourceSem"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void StartTASK1(void *argument);
void StartTASK2(void *argument);
void StartLP(void *argument);
void StartMP(void *argument);
void StartHP(void *argument);
void StartMyTask(void *argument);

/* USER CODE BEGIN PFP */
static void uart_print(const char *s);
static void OuterFunction(void);
static void InnerFunction(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void uart_print(const char *s)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)s, (uint16_t)strlen(s), 100);
}

static void OuterFunction(void)
{
	osMutexAcquire(MyMutexHandle, osWaitForever);
	uart_print("Outer start\r\n");

	InnerFunction();

	uart_print("Outer end\r\n");
	osMutexRelease(MyMutexHandle);
}

static void InnerFunction(void)
{
	osMutexAcquire(MyMutexHandle, osWaitForever);
	uart_print("   Inner\r\n");
	osMutexRelease(MyMutexHandle);
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of PrintMutex */
  PrintMutexHandle = osMutexNew(&PrintMutex_attributes);

  /* creation of ResourceMutex */
  ResourceMutexHandle = osMutexNew(&ResourceMutex_attributes);

  /* creation of MyMutex */
  MyMutexHandle = osMutexNew(&MyMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem */
  BinarySemHandle = osSemaphoreNew(1, 1, &BinarySem_attributes);

  /* creation of ResourceSem */
  ResourceSemHandle = osSemaphoreNew(1, 1, &ResourceSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TASK1 */
//  TASK1Handle = osThreadNew(StartTASK1, NULL, &TASK1_attributes);

  /* creation of TASK2 */
//  TASK2Handle = osThreadNew(StartTASK2, NULL, &TASK2_attributes);

  /* creation of LP */
//  LPHandle = osThreadNew(StartLP, NULL, &LP_attributes);

  /* creation of MP */
//  MPHandle = osThreadNew(StartMP, NULL, &MP_attributes);

  /* creation of HP */
//  HPHandle = osThreadNew(StartHP, NULL, &HP_attributes);

  /* creation of MyTask */
  MyTaskHandle = osThreadNew(StartMyTask, NULL, &MyTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
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
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
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
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTASK1 */
/**
* @brief Function implementing the TASK1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTASK1 */
void StartTASK1(void *argument)
{
  /* USER CODE BEGIN StartTASK1 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTASK1 */
}

/* USER CODE BEGIN Header_StartTASK2 */
/**
* @brief Function implementing the TASK2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTASK2 */
void StartTASK2(void *argument)
{
  /* USER CODE BEGIN StartTASK2 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTASK2 */
}

/* USER CODE BEGIN Header_StartLP */
/**
* @brief Function implementing the LP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLP */
void StartLP(void *argument)
{
  /* USER CODE BEGIN StartLP */
  /* Infinite loop */
//  osSemaphoreAcquire(ResourceSemHandle, osWaitForever);
  osMutexAcquire(ResourceMutexHandle, osWaitForever);

  uart_print("[LP] Resource acquired \r\n");

  for (int i = 0; i < 10; i++)
  {
	  uart_print("    [LP] Running...\r\n");
	  HAL_Delay(200);
  }
  uart_print("[LP] Done \r\n");

  uart_print("[LP] Resource released \r\n");

//  osSemaphoreRelease(ResourceSemHandle);
  osMutexRelease(ResourceMutexHandle);

  osThreadExit();
  /* USER CODE END StartLP */
}

/* USER CODE BEGIN Header_StartMP */
/**
* @brief Function implementing the MP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMP */
void StartMP(void *argument)
{
  /* USER CODE BEGIN StartMP */
  /* Infinite loop */
	osDelay(1000);

	uart_print("[MP] Started \r\n");

	for (int i = 0; i < 10; i++)
	{
	  uart_print("    [MP] Running...\r\n");
	  HAL_Delay(200);
	}
	uart_print("[MP] Done \r\n");

	osThreadExit();
  /* USER CODE END StartMP */
}

/* USER CODE BEGIN Header_StartHP */
/**
* @brief Function implementing the HP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHP */
void StartHP(void *argument)
{
  /* USER CODE BEGIN StartHP */
  /* Infinite loop */
	osDelay(500);

	uart_print("[HP] Requesting resource \r\n");

//	osSemaphoreAcquire(ResourceSemHandle, osWaitForever);
	osMutexAcquire(ResourceMutexHandle, osWaitForever);

	uart_print("[HP] Resource acquired \r\n");

	for (int i = 0; i < 10; i++)
	{
	  uart_print("    [HP] Running...\r\n");
	  HAL_Delay(200);
	}
	uart_print("[HP] Done \r\n");

//	osSemaphoreRelease(ResourceSemHandle);
	osMutexRelease(ResourceMutexHandle);


	osThreadExit();
  /* USER CODE END StartHP */
}

/* USER CODE BEGIN Header_StartMyTask */
/**
* @brief Function implementing the MyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMyTask */
void StartMyTask(void *argument)
{
  /* USER CODE BEGIN StartMyTask */
	uart_print("MyTask started\r\n");

  /* Infinite loop */
	for(;;)
	{
		OuterFunction();
		osDelay(1000);
	}
  /* USER CODE END StartMyTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
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

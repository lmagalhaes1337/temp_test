/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usart.h"
#include "stdlib.h"
#include "cmsis_os2.h"
#include "utils.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Constants for random number generation */
#define RNG_MULTIPLIER 1664525U   /* Multiplier for Linear Congruential Generator */
#define RNG_INCREMENT  1013904223U /* Increment for Linear Congruential Generator */
#define RNG_MODULUS    0xFFFFFFFFU /* Modulus for Linear Congruential Generator */


/* Application-specific defines */

#define TEMP_LOG_DELAY_MS   1000 /* Delay between temperature log entries in ms */
#define PRINT_LOG_DELAY_MS  5000 /* Delay between log prints in ms */
#define TEMP_WRITE_DELAY 1000U /* Delay for writing the temperature log entry */
#define LOG_PRINT_DELAY  5000U /* Delay for printing the temperature log */
#define MAX_RETRIES			10 /* Maximum number of retries for the mutex acquire and release functions */

#define MAX_MUTEX_WAIT		500 /* Maximum mutex wait time in miliseconds */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

osMutexId_t logMutexHandle;

/* USER CODE END Variables */
/* Definitions for printToTerm */
osThreadId_t printToTermHandle;
const osThreadAttr_t printToTerm_attributes = {
  .name = "printToTerm",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for getTemp */
osThreadId_t getTempHandle;
const osThreadAttr_t getTemp_attributes = {
  .name = "getTemp",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
	logMutexHandle = osMutexNew (0);
	if (logMutexHandle == NULL) {
	    while(1);
	}
  /* USER CODE END RTOS_MUTEX */

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
  /* creation of printToTerm */
  printToTermHandle = osThreadNew(StartDefaultTask, NULL, &printToTerm_attributes);

  /* creation of getTemp */
  getTempHandle = osThreadNew(StartTask02, NULL, &getTemp_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function responsible for updating the temperature log
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	osStatus_t status;
	uint8_t retries = 0;
	/* Infinite loop to continuously log temperatures */
  for(;;)
  {


	/* Acquire mutex to ensure exclusive access to temperature log */
	while(retries < MAX_RETRIES){
	  status = osMutexAcquire(logMutexHandle, MAX_MUTEX_WAIT);
	  if(status == osOK){
		  break;
	  }
	  retries++;
	}
	if(status != osOK){
	  continue;
	}
	retries = 0;
	/* Simulate logging a temperature value */
	add_new_temp_value();
	/* Release the mutex after updating the log */
	while(retries < MAX_RETRIES){
	  status = osMutexRelease(logMutexHandle);
	  if(status == osOK){
		  break;
	  }
	  retries++;
	}
	retries = 0;


	  osDelay(TEMP_WRITE_DELAY); /* Delay between temperature log entries */
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
  * @brief  Function that prints the temperature log every 5 seconds..
  * @param  argument: Not used
  * @retval None
  * @note	If the printing fails, the task will be suspended since it has no purpose other than to print the log.
  */
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
	HAL_StatusTypeDef status = HAL_OK;
	osStatus_t osStatus = osOK;
	uint8_t retries = 0;
	uint8_t suspend_flag = 0;
	/* Infinite loop to continuously print the temperature log */
  for(;;)
  {
	  /* Acquire mutex to ensure exclusive access to temperature log */
	  while(retries < MAX_RETRIES){
		  osStatus = osMutexAcquire(logMutexHandle, MAX_MUTEX_WAIT);
		  if(osStatus == osOK){
			  break;
		  }
		  retries++;
	  }
	  if(osStatus != osOK){
		  continue;
	  }
	  retries = 0;

	  /* Simulate printing the temperature log */
	  status = Print_Temperature_Log(temperature_log);
	  if(status != HAL_OK){
		  /* If printing the temperature log failed, then we suspend the task. However, we need to release the mutex first */
		  suspend_flag = 1;
	  }
	  /* Release the mutex after printing the log */
	  while(retries < MAX_RETRIES){
		  osStatus = osMutexRelease(logMutexHandle);
		  if(osStatus != osOK){
			  break;
		  }
		  retries++;
	  }
	  retries = 0;

	  if(suspend_flag == 1){
		  osThreadSuspend(NULL); // Suspend the current task
	  }

	  /* Delay between printing logs */
	  osDelay(LOG_PRINT_DELAY);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


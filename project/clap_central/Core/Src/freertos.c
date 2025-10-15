/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_ahrs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_200HZ */
osThreadId_t Task_200HZHandle;
uint32_t Task_200HZBuffer[ 256 ];
osStaticThreadDef_t Task_200HZControlBlock;
const osThreadAttr_t Task_200HZ_attributes = {
  .name = "Task_200HZ",
  .cb_mem = &Task_200HZControlBlock,
  .cb_size = sizeof(Task_200HZControlBlock),
  .stack_mem = &Task_200HZBuffer[0],
  .stack_size = sizeof(Task_200HZBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void task_200hz_process(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Task_200HZ */
  Task_200HZHandle = osThreadNew(task_200hz_process, NULL, &Task_200HZ_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  uint32_t tick = osKernelGetTickCount();
  for(;;)
  {
    HAL_GPIO_TogglePin(LEDl_GPIO_Port,LEDl_Pin);

    tick+=500;
    osDelayUntil(tick);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_task_200hz_process */
/**
* @brief Function implementing the Task_200HZ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_task_200hz_process */
app_ahrs_t g_ahrs;
void task_200hz_process(void *argument)
{
  /* USER CODE BEGIN task_200hz_process */
  /* Infinite loop */
  uint32_t tick = osKernelGetTickCount();

  app_ahrs_init(&g_ahrs);

  for(;;)
  {
    app_ahrs_update(&g_ahrs,0.01f);
    jcom_float_print(0,g_ahrs.roll,g_ahrs.pitch,g_ahrs.yaw);

    tick+=10;
    osDelayUntil(tick);
  }
  /* USER CODE END task_200hz_process */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "main.h"
#include "rtc.h"
#include "modbus.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId sensorSampleHandle;
osThreadId keyAndUiHandle;
osThreadId communicationHandle;
osThreadId ldleHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void sensor_sample(void const * argument);
void interaction(void const * argument);
void process_comm(void const * argument);
void sys_ldle(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of sensorSample */
  osThreadDef(sensorSample, sensor_sample, osPriorityAboveNormal, 0, 512);
  sensorSampleHandle = osThreadCreate(osThread(sensorSample), NULL);

  /* definition and creation of keyAndUi */
  osThreadDef(keyAndUi, interaction, osPriorityNormal, 0, 128);
  keyAndUiHandle = osThreadCreate(osThread(keyAndUi), NULL);

  /* definition and creation of communication */
  osThreadDef(communication, process_comm, osPriorityHigh, 0, 512);
  communicationHandle = osThreadCreate(osThread(communication), NULL);

  /* definition and creation of ldle */
  osThreadDef(ldle, sys_ldle, osPriorityIdle, 0, 128);
  ldleHandle = osThreadCreate(osThread(ldle), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* sensor_sample function */
void sensor_sample(void const * argument)
{
  /* USER CODE BEGIN sensor_sample */
	dev_modbus_handle_t hmodbus;
	
	creat_dev_inf(&hmodbus,MODBUS_RTU_TEST);
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
		modbus_read_request(&hmodbus);
  }
  /* USER CODE END sensor_sample */
}

/* interaction function */
void interaction(void const * argument)
{
  /* USER CODE BEGIN interaction */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END interaction */
}

/* process_comm function */
void process_comm(void const * argument)
{
  /* USER CODE BEGIN process_comm */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END process_comm */
}

/* sys_ldle function */
void sys_ldle(void const * argument)
{
  /* USER CODE BEGIN sys_ldle */
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
  /* Infinite loop */
  for(;;)
  {	
		
		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
		
		printf("sys ldle %x-%x-%x-%x:%x:%x\r\n",date.Year,date.Month,date.Date,time.Hours,time.Minutes,time.Seconds);
		TOGGLE_LED1();
    osDelay(1000);
  }
  /* USER CODE END sys_ldle */
}

/* USER CODE BEGIN Application */

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){
	
	}
	else if(huart->Instance == USART2){
	
	}
	else if(huart->Instance == USART3){
	
	}
	else if(huart->Instance == UART5){
	
	}
	TOGGLE_LED2();
}
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

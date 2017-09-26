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
#include "usart.h"
#include "lcd_12864.h"
#include "key.h"
#include "gui.h"
#include "string.h"
#include "AT24C04.h"
#include "hydrologyStack.h"
#include "usart.h"
#include "adc.h"
#include "hyetal.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId sensorSampleHandle;
osThreadId keyAndUiHandle;
osThreadId communicationHandle;
osThreadId ldleHandle;

/* USER CODE BEGIN Variables */
SemaphoreHandle_t senseGetDataSemaphore;
uint8_t uarto_rx_buf[32];

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void sensor_sample(void const * argument);
void interaction(void const * argument);
void process_comm(void const * argument);
void sys_ldle(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

void power_standby_mode(void) {
	/*
	 * 关闭传感器电源
	 * */

	/*
	 * 关闭3.3V外围芯片电源
	 * */

	/*
	 *关闭部分外设，按键IO不能关闭！！
	 */
	 __HAL_RCC_GPIOC_CLK_DISABLE();
	 __HAL_RCC_GPIOA_CLK_DISABLE();
	 __HAL_RCC_GPIOB_CLK_DISABLE();
	 __HAL_RCC_GPIOD_CLK_DISABLE();

	 /*
	 * 关闭外部高速时钟
	 * */

}

void power_norm_mode(void){

	/*恢复外设时钟
	 * */
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOD_CLK_ENABLE();

	/*
	 * 打开传感器电源
	 * */

	/*
	 * 打开3.3V外围芯片电源
	 * */

	/*
	 * 打开外部高速时钟
	 * */
}

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
  osThreadDef(sensorSample, sensor_sample, osPriorityAboveNormal, 0, 1024);
  sensorSampleHandle = osThreadCreate(osThread(sensorSample), NULL);

  /* definition and creation of keyAndUi */
  osThreadDef(keyAndUi, interaction, osPriorityBelowNormal, 0, 1024);
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
	vTaskDelete(defaultTaskHandle);
  /* USER CODE END StartDefaultTask */
}

/* sensor_sample function */
void sensor_sample(void const * argument)
{
  /* USER CODE BEGIN sensor_sample */
	dev_modbus_handle_t hmodbus;

	senseGetDataSemaphore = xSemaphoreCreateBinary();

	creat_dev_inf(&hmodbus, MODBUS_RTU_TEST);
	HAL_UART_Receive_IT(&huart3, uarto_rx_buf, 0);
	/* Infinite loop */
	for (;;) {
		TOGGLE_LED1();
		xSemaphoreTake(senseGetDataSemaphore, portMAX_DELAY );
		//sprintf((char*)testStr + 9,"%08ld",get_rainFall(&waterInf,1));
		//HAL_UART_Transmit(&RS2322_UART_HANDLE,testStr,sizeof(testStr),10);

		//osDelay(1000);
	}
  /* USER CODE END sensor_sample */
}

/* interaction function */
void interaction(void const * argument)
{
  /* USER CODE BEGIN interaction */

	lcd_init();
	/* Infinite loop */
	for (;;) {
		portENTER_CRITICAL();
		gui_main_windows();
		TOGGLE_LED2();
		portEXIT_CRITICAL();
		osDelay(1000);
	}
  /* USER CODE END interaction */
}

/* process_comm function */
void process_comm(void const * argument)
{
  /* USER CODE BEGIN process_comm */
	load_config_Default();
	clear_element_from_message(&messageHandle,-1);

	add_sim_waterInf_data(&hydrologyInf);

	/* Infinite loop */
	for (;;) {
//		creat_msg(&messageHandle,FUN_CODE_XSB);
//		HAL_UART_Transmit(&RS2322_UART_HANDLE,get_addr_txBuf(),getLen_of_txBuf(),10);
		osDelay(1000);
	}
  /* USER CODE END process_comm */
}

/* sys_ldle function */
void sys_ldle(void const * argument)
{
  /* USER CODE BEGIN sys_ldle */
	/* Infinite loop */
	for (;;) {
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	}
  /* USER CODE END sys_ldle */
}

/* USER CODE BEGIN Application */

void vApplicationIdleHook(){
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	UBaseType_t uxSavedInterruptStatus;
	static  BaseType_t xHigherPriorityTaskWoken;

	xSemaphoreGiveFromISR( senseGetDataSemaphore, &xHigherPriorityTaskWoken );

	portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

}
/*
 * 雨量计的中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	UBaseType_t uxSavedInterruptStatus;
	static  BaseType_t xHigherPriorityTaskWoken;

	portSET_INTERRUPT_MASK_FROM_ISR();
	if(GPIO_Pin == GPIO_PIN_6){
		trigger_rain_signal(&waterInf);
		 xSemaphoreGiveFromISR( senseGetDataSemaphore, &xHigherPriorityTaskWoken );

	}
	portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	UBaseType_t uxSavedInterruptStatus;
	portSET_INTERRUPT_MASK_FROM_ISR();

	portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc){
	UBaseType_t uxSavedInterruptStatus;
	portSET_INTERRUPT_MASK_FROM_ISR();
	portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	UBaseType_t uxSavedInterruptStatus;

	portSET_INTERRUPT_MASK_FROM_ISR();

	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, uarto_rx_buf, 5);
	} else if (huart->Instance == USART2) {

	} else if (huart->Instance == USART3) {
		HAL_UART_Receive_IT(&huart3, uarto_rx_buf, 1);
	} else if (huart->Instance == UART5) {

	}
	TOGGLE_LED2();
	portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

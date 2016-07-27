/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"

/* USER CODE BEGIN 0 */
#include "blink.h"
#include "my_func.h"
#include "level.h"
#include "error.h"


uint32_t adc_data_I_ = 0;
uint32_t adc_data_U_ = 0;
uint32_t adc_val[2] = {0,0};
extern ADC_HandleTypeDef hadc;
uint32_t adc_disp = 0;

extern uint32_t calculate_val_ac(uint32_t);
extern void store_val(void);
extern void get_real_val(void);

uint32_t I_accum = 0;
uint32_t U_accum = 0;
uint32_t cnt = 0;
uint32_t I_val = 0;
uint32_t U_val = 888;      // "888" - initialise display befor first ADC measuring

extern uint32_t adc_disp;


extern Chanel chanel;
extern Butt_state button;

extern Tank tank;
Tank old_tank = {0, 0, 0, 0};

extern uint8_t work_counter;

extern Error error_type;

extern uint8_t err_toggle_count;

extern uint8_t delay_count;

extern uint8_t u_err_count;
extern uint8_t i_err_count;

extern volatile SW_TIMER soft_timer[SwTimerCount];

extern uint8_t u_delay;
extern uint8_t i_delay;
extern Disp_sym disp_errs[7][3];

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles TIM1 break, update, trigger and commutation interrupts.
*/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

  SwTimerWork(soft_timer, SwTimerCount);
	
  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}

/**
* @brief This function handles TIM3 global interrupt.
*/
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	
	err_toggle_count++;
	
  /* USER CODE END TIM3_IRQn 1 */
}

/**
* @brief This function handles TIM14 global interrupt.
*/
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */

  /* USER CODE END TIM14_IRQn 0 */
  HAL_TIM_IRQHandler(&htim14);
  /* USER CODE BEGIN TIM14_IRQn 1 */
	
	work_counter++;															// change level counter
  
  /* USER CODE END TIM14_IRQn 1 */
}

/**
* @brief This function handles TIM16 global interrupt.
*/
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */
	
//	if (error_type != E_OFF){
//		
//		if (error_type == E_U || error_type == E_I ||	error_type == ELI || error_type == ELO){
//					
//			HAL_TIM_Base_Start_IT(&htim3);
//			err_disp_toggle();
//		}
//		else {
//			Val_on_disp odv_e = e_err_tp;
//			dig_to_disp (error_type, &odv_e);
//		}
//	} //if (error_type != E_OFF)
//	
//	else{
//		Val_on_disp odv_c = e_cnl;	
//		dig_to_disp( disp_chanel(), &odv_c );
//	}
//--------------------------------------------	
//	static const Disp_sym out_sym = tE;
//	static Disp_sym message[] = {8, 0, 9};
//	Val_on_disp odv_c = e_cnl;
//	dig_to_disp_a(message, &odv_c);
//----------------------------------------------

	static Disp_sym out_sym[] = {0, 0, 0};
	
	if (error_type != E_OFF){
		
		if (error_type == E_U || error_type == E_I ||	error_type == ELI || error_type == ELO){
					
			HAL_TIM_Base_Start_IT(&htim3);
			err_disp_toggle_a();
		}
		else {
			Val_on_disp odv_e = e_err_tp;
			dig_to_disp_a (disp_errs[error_type], &odv_e);
		}
	} //if (error_type != E_OFF)
	
	else{
		Val_on_disp odv_c = e_cnl;
		int_to_sym(disp_chanel(), out_sym);
		dig_to_disp_a( out_sym, &odv_c );
	}
	
//	HAL_IWDG_Refresh(&hiwdg);		
  /* USER CODE END TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM16_IRQn 1 */
	
  /* USER CODE END TIM16_IRQn 1 */
}

/**
* @brief This function handles TIM17 global interrupt.
*/
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */

  /* USER CODE END TIM17_IRQn 0 */
  HAL_TIM_IRQHandler(&htim17);
  /* USER CODE BEGIN TIM17_IRQn 1 */
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 1);
	
	adc_val[_U_] = HAL_ADC_GetValue(&hadc);				
	adc_val[_I_] = HAL_ADC_GetValue(&hadc);
	
	store_val();														// adding new adc_val to storage var
	cnt++;
	
	if (cnt > 10000){											// make 10000 measuring in 1 second
		get_real_val();												// calculate real value of I & U
	}
	
	HAL_ADC_Stop(&hadc);

  /* USER CODE END TIM17_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

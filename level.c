//by Sir0ga, 12.04.2016

#include "level.h"
#include "stm32f0xx_hal.h"


//---------------------------------------------------------------------------------------
extern Well_level 	w_level;
extern Tank 				tank;
extern Sens					sen1;
extern Sens					sen2;

extern TIM_HandleTypeDef htim14;

uint8_t wait_flag = 0;

uint8_t work_counter = 0;

Tank last_tank = {0, 0, 0, 0};

extern uint8_t input_delay;
//---------------------------------------------------------------------------------------for level_struct
Tank get_tank_level(void){
	
	static Tank local_tank = {0, 0, 0, 0};
	
	if (HAL_GPIO_ReadPin(input_lev1_GPIO_Port, input_lev1_Pin) == sen1.dry){								// if L1-OFF
		
		if (HAL_GPIO_ReadPin(input_lev2_GPIO_Port, input_lev2_Pin) == sen2.in_water){					// & L2-ON
			local_tank.full = 				0;
			local_tank.half_full = 		0;
			local_tank.empty = 				0;
			local_tank.error_level = 	1;																								// wrong placement of sensors
			return local_tank;
		}
		else
			local_tank.half_full = 		0;
			local_tank.empty = 				1;
			local_tank.error_level = 	0;										// if before was error
	}
	else {
		local_tank.half_full = 			1;
		local_tank.error_level = 		0;										// if before was error
		local_tank.empty = 					0;										// if before was empty
	}
	
	
	if (HAL_GPIO_ReadPin(input_lev2_GPIO_Port, input_lev2_Pin) == sen2.in_water){				// right work of Lev2{
		local_tank.full = 					1;
		local_tank.error_level = 		0;											// if before was error
		return local_tank;
	}
	else {
		local_tank.full = 					0;
		local_tank.error_level = 		0;											// if before was error
		return local_tank;
	}
	
}
//----------------------------------------------------------------------------------------
void get_well_level(void){
	if (HAL_GPIO_ReadPin(input_fail_GPIO_Port, input_fail_Pin) == RESET)
		w_level = full;
	else 
		w_level = dry;
}
//----------------------------------------------------------------------------------------
void level_indication(void){
	
	if (tank.full == 1)
		HAL_GPIO_WritePin(led_lev2_GPIO_Port, led_lev2_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(led_lev2_GPIO_Port, led_lev2_Pin, GPIO_PIN_RESET);
	
	if (tank.half_full == 1)
		HAL_GPIO_WritePin(led_lev1_GPIO_Port, led_lev1_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(led_lev1_GPIO_Port, led_lev1_Pin, GPIO_PIN_RESET);
	
	if (w_level == dry)
		HAL_GPIO_WritePin(led_fail_GPIO_Port, led_fail_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(led_fail_GPIO_Port, led_fail_Pin, GPIO_PIN_RESET);

}
//---------------------------------------------------------------------------------------

void level_work(void){
	
	Tank func_tank = {0, 0, 0, 0};
	
	if (wait_flag == RESET){
		
		func_tank = get_tank_level();
	
		if (func_tank.full 				!= 	tank.full 				||
				func_tank.half_full 	!= 	tank.half_full 		||
				func_tank.error_level != 	tank.error_level 	||
				func_tank.empty 			!= 	tank.empty) {
					
					HAL_TIM_Base_Start_IT(&htim14);
					
					wait_flag = SET;
					last_tank = func_tank;
				}
		}
	
	else if	(work_counter >= input_delay){
		
		wait_flag = RESET;
		HAL_TIM_Base_Stop_IT(&htim14);
		tank = last_tank;
		work_counter = 0;
	
	}
	
}

//---------------------------------------------------------------------------------------

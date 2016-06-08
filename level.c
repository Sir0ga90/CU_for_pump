//by Sir0ga, 12.04.2016

#include "level.h"
#include "stm32f0xx_hal.h"


//---------------------------------------------------------------------------------------


uint8_t wait_flag = 0;

uint8_t work_counter = 0;

Tank last_tank = {0, 0, 0, 0};

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
Well_level get_well_level(void){
	Well_level local_well;
	
	if (HAL_GPIO_ReadPin(input_fail_GPIO_Port, input_fail_Pin) == RESET)
		local_well = full;
	else 
		local_well = dry;
	
	return local_well;
}
//----------------------------------------------------------------------------------------
uint8_t flag_well;

void well_level_work(void){
	Well_level func_well;
	func_well = get_well_level();
	
	if (func_well != w_level && flag_well == RESET){
		OnSwTimer(&soft_timer[well_in_tim], SWTIMER_MODE_WAIT_ON, input_delay);
		soft_timer[well_in_tim].On = 1;
		flag_well = SET;
	}
	else if (soft_timer[well_in_tim].Out == SET && flag_well == SET){
		w_level = func_well;
		
		flag_well = RESET;
		
		soft_timer[well_in_tim].Out = RESET;
		soft_timer[well_in_tim].On = RESET;
	}
}
	
//----------------------------------------------------------------------------------------
void level_indication(void){
	
	if (tank.error_level == 1){
		HAL_GPIO_WritePin(led_lev2_GPIO_Port, led_lev1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led_lev1_GPIO_Port, led_lev2_Pin, GPIO_PIN_RESET);
		return;
	}
	
	if (tank.full == 1)
		HAL_GPIO_WritePin(led_lev2_GPIO_Port, led_lev2_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(led_lev2_GPIO_Port, led_lev2_Pin, GPIO_PIN_RESET);
	
	if (tank.half_full == 1)
		HAL_GPIO_WritePin(led_lev1_GPIO_Port, led_lev1_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(led_lev1_GPIO_Port, led_lev1_Pin, GPIO_PIN_RESET);
	
}
//---------------------------------------------------------------------------------------
uint8_t flag_1_lev = 0;
uint8_t flag_2_lev = 0;
uint8_t flag_err_lev = 0;

void level_work(void){
	
	Tank func_tank = {0, 0, 0, 0};

	func_tank = get_tank_level();
	
	if(w_level != w_err){																	// avoid work before first take of well_level
	
		if ( (func_tank.empty != tank.empty 					|| 
					func_tank.half_full != tank.half_full)	&& 
					flag_1_lev == RESET){
			OnSwTimer(&soft_timer[lev_1_tim], SWTIMER_MODE_WAIT_ON, input_delay);
			soft_timer[lev_1_tim].On = 1;
			flag_1_lev = SET;
		}
		else if (soft_timer[lev_1_tim].Out == SET && flag_1_lev == SET){
			tank.empty = func_tank.empty;
			tank.half_full = func_tank.half_full;
			
			flag_1_lev = RESET;
			
			soft_timer[lev_1_tim].Out = RESET;
			soft_timer[lev_1_tim].On = RESET;
		}
		
		
		
		if (func_tank.full != tank.full && flag_2_lev == RESET){
			OnSwTimer(&soft_timer[lev_2_tim], SWTIMER_MODE_WAIT_ON, input_delay);
			soft_timer[lev_2_tim].On = 1;
			flag_2_lev = SET;
		}
		else if (soft_timer[lev_2_tim].Out == SET && flag_2_lev == SET){
			tank.full = func_tank.full;
			
			flag_2_lev = RESET;
			
			soft_timer[lev_2_tim].Out = RESET;
			soft_timer[lev_2_tim].On = RESET;
		}
		
		
		if (func_tank.error_level != tank.error_level && flag_err_lev == RESET){
			OnSwTimer(&soft_timer[lev_err_tim], SWTIMER_MODE_WAIT_ON, input_delay);
			soft_timer[lev_err_tim].On = 1;
			flag_err_lev = SET;
		}
		else if (soft_timer[lev_err_tim].Out == SET && flag_err_lev == SET){
			tank.error_level = func_tank.error_level;
			
			flag_err_lev = RESET;
			
			soft_timer[lev_err_tim].Out = RESET;
			soft_timer[lev_err_tim].On = RESET;
		}
	}
}

//---------------------------------------------------------------------------------------

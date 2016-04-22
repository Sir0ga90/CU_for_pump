//by Sir0ga, 12.04.2016

#include "level.h"
#include "stm32f0xx_hal.h"

//---------------------------------------------------------------------------------------
extern Level 				t_level;
extern Well_level 	w_level;
extern Tank 				tank;
//---------------------------------------------------------------------------------------for level_enum
//	void read_level(void){
//  if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin) == SET){
//		t_level = half_full;
//		HAL_GPIO_WritePin(on_sig_led_GPIO_Port, on_sig_led_Pin, GPIO_PIN_SET);
//	}
//	
//	if (HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET)
//		t_level = full;
//	
//	if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin == RESET) 
//										&& HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET) 			// if level_1 not in water but level_2 in
//		t_level = error;																														
//	
//	if (HAL_GPIO_ReadPin(wms_GPIO_Port, wms_Pin) == SET)
//		w_level = dry;
//}
//---------------------------------------------------------------------------------------for level_struct
void get_tank_level(void){
	
	if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin) == SET){								// if L1-OFF
		
		if (HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == RESET){						// & L2-ON
			tank.full = 0;
			tank.half_full = 0;
			tank.error_level = 1;																								// wrong placement of sensors
			return;
		}
		else
			tank.half_full = 0;
			tank.empty = 1;
			tank.error_level = 0;									// if before was error
	}
	else {
		tank.half_full = 1;
		tank.error_level = 0;										// if before was error
	}
	
	
	if (HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET){
		tank.full = 0;
		tank.error_level = 0;										// if before was error
		return;
	}
	else {
		tank.full = 1;
		tank.error_level = 0;										// if before was error
		return;
	}
	
}
//----------------------------------------------------------------------------------------


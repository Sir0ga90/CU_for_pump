//by Sir0ga, 12.04.2016

#include "level.h"
#include "stm32f0xx_hal.h"

//---------------------------------------------------------------------------------------
extern Well_level 	w_level;
extern Tank 				tank;
extern Sens					sen1;
extern Sens					sen2;
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
	
	if (HAL_GPIO_ReadPin(input_lev1_GPIO_Port, input_lev1_Pin) == sen1.dry){								// if L1-OFF
		
		if (HAL_GPIO_ReadPin(input_lev2_GPIO_Port, input_lev2_Pin) == sen2.in_water){					// & L2-ON
			tank.full = 				0;
			tank.half_full = 		0;
			tank.empty = 				0;
			tank.error_level = 	1;																								// wrong placement of sensors
			return;
		}
		else
			tank.half_full = 		0;
			tank.empty = 				1;
			tank.error_level = 	0;										// if before was error
	}
	else {
		tank.half_full = 			1;
		tank.error_level = 		0;										// if before was error
		tank.empty = 					0;										// if before was empty
	}
	
	
	if (HAL_GPIO_ReadPin(input_lev2_GPIO_Port, input_lev2_Pin) == sen2.in_water){				// right work of Lev2{
		tank.full = 					1;
		tank.error_level = 		0;											// if before was error
		return;
	}
	else {
		tank.full = 					0;
		tank.error_level = 		0;											// if before was error
		return;
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


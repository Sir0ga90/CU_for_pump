//by Sir0ga, 12.04.2016

#include "level.h"
#include "stm32f0xx_hal.h"

//---------------------------------------------------------------------------------------
extern Level 				t_level;
extern Well_level 	w_level;
extern Tank 				tank;
//---------------------------------------------------------------------------------------
	void read_level(void){
  if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin) == SET)
		t_level = half_full;
	
	if (HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET)
		t_level = full;
	
	if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin == RESET) 
										&& HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET) 			// if level_1 not in water but level_2 in
		t_level = error;																														
	
	if (HAL_GPIO_ReadPin(wms_GPIO_Port, wms_Pin) == SET)
		w_level = dry;
}
//---------------------------------------------------------------------------------------
void read_tank(void){
	if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin) == SET)
		tank.half_full = 1;
	
	if (HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET)
		tank.full = 1;
	
	if (HAL_GPIO_ReadPin(lev_1_GPIO_Port, lev_1_Pin ==RESET) 
											&& HAL_GPIO_ReadPin(lev_2_GPIO_Port, lev_2_Pin) == SET)
		tank.empty = 1;
	
	if (HAL_GPIO_ReadPin(wms_GPIO_Port, wms_Pin) == SET)
		w_level = dry;
}
//---------------------------------------------------------------------------------------

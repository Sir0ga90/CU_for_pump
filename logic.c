#include <logic.h>

extern Tank tank;
extern Logic logic;

//------------------------------------------------------------------------------------
void pump(void){

	if (tank.full == SET || tank.error_level == SET){
		HAL_GPIO_WritePin(rel_GPIO_Port, rel_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led_work_GPIO_Port, led_work_Pin, GPIO_PIN_RESET);
		return;
	}
	
	if (tank.empty == SET){
		HAL_GPIO_WritePin(rel_GPIO_Port, rel_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led_work_GPIO_Port, led_work_Pin, GPIO_PIN_SET);
	}
	
}
//------------------------------------------------------------------------------------
void drain(void){

	if (tank.empty == SET || tank.error_level == SET){
		HAL_GPIO_WritePin(rel_GPIO_Port, rel_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led_work_GPIO_Port, led_work_Pin, GPIO_PIN_RESET);
		return;
	}
	
	if (tank.full == SET){
		HAL_GPIO_WritePin(rel_GPIO_Port, rel_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led_work_GPIO_Port, led_work_Pin, GPIO_PIN_SET);
	}
		
}
//-------------------------------------------------------------------------------------
void work_logic(void){
	if (logic == e_pump){
		pump();
	}
	else if (logic == e_drain){
		drain();
	}
}

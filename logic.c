#include <logic.h>
#include "stm32f0xx_hal.h"
#include "dip_sw.h"

//------------------------------------------------------------------------------------
void pump(Motor_state *motor){

	if (tank.full == SET){
		motor_off( motor );
		return;
	}
	
	if (tank.empty == SET){
		motor_on( motor );
	}
	
}
//------------------------------------------------------------------------------------
void drain(Motor_state *motor){

	if (tank.empty == SET || tank.error_level == SET){
		motor_off( motor );
		return;
	}
	
	if (tank.full == SET){
		motor_on( motor );
	}

}
//-------------------------------------------------------------------------------------
void stop	(Motor_state *motor){
	motor_off( motor );
}
//-------------------------------------------------------------------------------------
void work_logic(Motor_state *motor){
	
	if (logic == e_pump){
		pump(motor);
	}
	
	else if (logic == e_drain){
		drain(motor);
	}
	
	else {
		logic = e_err;
		stop( motor );		
	} 
	
}
//--------------------------------------------------------------------------------------
void motor_on(Motor_state *motor){
	HAL_GPIO_WritePin(rel_GPIO_Port, rel_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(led_work_GPIO_Port, led_work_Pin, GPIO_PIN_SET);
	*motor = m_on;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void motor_off(Motor_state *motor){
	HAL_GPIO_WritePin(rel_GPIO_Port, rel_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(led_work_GPIO_Port, led_work_Pin, GPIO_PIN_RESET);
	*motor = m_off;
}

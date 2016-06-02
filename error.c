#include "error.h"
#include "my_func.h"
#include "dip_sw.h"
#include "blink.h"
#include "dip_sw.h"
#include "level.h"

//--------------------------------------------------------------


uint8_t err_toggle_count = 0;

uint8_t delay_count = 0;

Val_on_disp on_display_value = e_no_val;

//------------------------------------------------------------------
uint8_t flag_err_u = 0;
uint8_t u_err_cnt = 0;
uint8_t cur_cnt = 0;

void check_u(void){
		if ((U_val > (nominal_U + tres_U)) || 
				(U_val < (nominal_U - tres_U) ) || flag_err_u == 0) {
					flag_err_u = 1;
		}
		else if (flag_err_u == 1) {
			cur_cnt = u_err_cnt;
			
		}
		else 
			error_type = E_OFF;
				
}
//-------------------------------------------------------------------
uint8_t flag_err_i = 0;
uint8_t i_err_cnt = 0;
void check_i(void){
	if (I_val > ( tres_I + (tres_I * 0.2) ) ){ 						// + 20%
		error_type = E_I;
	}
			
	else if (I_val < ( tres_I - (tres_I * 0.4) ) ){				// -40%
		error_type = ELI;
	}
	
	else if (error_type != E_U) error_type = E_OFF;
}

//-------------------------------------------------------------------
void check_well(void){
	if (w_level == dry){
		error_type = ELO;
	}
}
//-------------------------------------------------------------------
void check_lvls(Motor_state *motor){
	if (tank.error_level == SET){
		stop( motor );
		error_type = ELL;
	}
}
//-------------------------------------------------------------------
void while_error_delay(Motor_state *motor){
	HAL_GPIO_WritePin(led_fail_GPIO_Port, led_fail_Pin, GPIO_PIN_SET);
	
	stop( motor );
	
	switch (error_type) {
		case E_U:
			while (error_type == E_U){
				check_u();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		case E_I:
			while (error_type == E_I){
				check_i();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		case ELI:
			while (error_type == ELI){
				check_i();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		case ELO:
			while (w_level == dry){
			get_well_level();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		case ELL:
			while (tank.error_level == SET){
			level_work();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		default:
			error_type = E_OFF;
			break;
	}
}
//-------------------------------------------------------------------
uint8_t work_flag = 0;
uint8_t st_flag = 0;

void error_check(Motor_state *motor){
	
	if ( *motor == m_on ){
		if (delay_count == 0 && work_flag == 0){
			HAL_TIM_Base_Start_IT(&htim1);
		}
			
		if (delay_count >= 5){
			check_i();
			work_flag = 1;
			st_flag = 0;
		}
		
		if (delay_count >= 10){
			check_u();
		}
	} // if ( *motor == m_on )
	
		if ( *motor == m_off && st_flag == 0){
			delay_count = 0;
			work_flag = 0;
			st_flag = 1;
			HAL_TIM_Base_Stop_IT(&htim1);
		} //if ( *motor == m_off && st_flag == 0)
	
	check_well();
	check_lvls( motor );
		
	if (error_type != E_OFF){
		while_error_delay(motor);
		HAL_GPIO_WritePin(led_fail_GPIO_Port, led_fail_Pin, GPIO_PIN_RESET);
	} // if (error_type != E_OFF)
}
		
//--------------------------------------------------------------------
void err_disp_toggle(void){
	if ( err_toggle_count <= 2 ){
		on_display_value = e_err_tp;
		dig_to_disp(error_type, &on_display_value);
	}
	else if (err_toggle_count <= 5){
		on_display_value = e_cnl;
		dig_to_disp( disp_err_chanel(), &on_display_value);
		
		if (err_toggle_count == 5){
			err_toggle_count = 0;
		}
	} // if err_toggle_count <= 5
}
//---------------------------------------------------------------------

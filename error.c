#include "error.h"
#include "my_func.h"
#include "blink.h"
#include "dip_sw.h"
#include "level.h"
#include "sw_timer.h"
#include "dip_sw.h"

uint8_t work_flag = 0;
uint8_t st_flag = 0;
uint8_t start_flag_u = 0;
uint8_t start_flag_i = 0;

//--------------------------------------------------------------
#if (SwTimerCount>0)
 SW_TIMER volatile soft_timer[SwTimerCount]; //declaration of sw_tim's
#endif

uint8_t u_delay = 10;
uint8_t i_delay = 5;
uint16_t well_err_del = 120;   //while debug

uint8_t err_toggle_count = 0;

uint8_t delay_count = 0;

Val_on_disp on_display_value = e_no_val;

//------------------------------------------------------------------
uint8_t flag_err_u = 0;
uint8_t u_err_cnt = 0;
uint8_t cur_cnt = 0;

void check_u(void){
		if ((U_val > (nominal_U + tres_U)) || 
				(U_val < (nominal_U - tres_U) )) {
					flag_err_u = 1;
					error_type = E_U;
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
	
	
	soft_timer[st_u_tim].On = 0;
	soft_timer[st_u_tim].Off = 1;
	
	
	soft_timer[st_i_tim].On = 0;
	soft_timer[st_i_tim].Off = 1;
	
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
			well_level_work();
			}
			if (flag_auto_blocking == SET){				
				while_well_err_delay();
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
//------------------------------------------------------------------
void error_check(Motor_state *motor){
	
	if ( *motor == m_off ){
		start_flag_u = RESET;
		start_flag_i = RESET;	
	}
		
	if (start_flag_i == RESET){		
		start_u_i_check( motor );
	}
	else check_i();
	
	if (start_flag_u == RESET){		
		start_u_i_check( motor );
	}
	else check_u();
	
	check_well();
	check_lvls( motor );
		
	if (error_type != E_OFF){
		while_error_delay(motor);
		HAL_GPIO_WritePin(led_fail_GPIO_Port, led_fail_Pin, GPIO_PIN_RESET);
		error_type = E_OFF;
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
void start_u_i_check( Motor_state *motor){
	if ( *motor == m_on ){
			if (work_flag == 0){
				OnSwTimer(&soft_timer[st_u_tim], SWTIMER_MODE_WAIT_ON, u_delay);
				soft_timer[st_u_tim].On = 1;
				soft_timer[st_u_tim].Off = 0;
				
				OnSwTimer(&soft_timer[st_i_tim], SWTIMER_MODE_WAIT_ON, i_delay);
				soft_timer[st_i_tim].On = 1;
				soft_timer[st_i_tim].Off = 0;
				
				work_flag = 1;
			}
			
			if (soft_timer[st_u_tim].Out == 1 && start_flag_u == RESET){
				start_flag_u = SET;
				soft_timer[st_u_tim].On = 0;
				soft_timer[st_u_tim].Off = 1;
			}
			if (soft_timer[st_i_tim].Out == 1){
				start_flag_i = SET;
				soft_timer[st_i_tim].On = 0;
				soft_timer[st_i_tim].Off = 1;
			}
		}
}
//---------------------------------------------------------------------while well error delay & subfunc's
void while_well_err_delay(void){
	OnSwTimer(&soft_timer[well_err_tim], SWTIMER_MODE_WAIT_ON, well_err_del);
	soft_timer[well_err_tim].On = SET;
	
	static const uint8_t count_val = 9;						// 9 for 5-cycle w_level toggling (full -> dry = 1, dry -> full = +1 &s.o.) 
	
	volatile uint8_t lev_toggle_cnt = 0;					// counter of toggling fail input
	Well_level last_input_state = full;
	
	while (soft_timer[well_err_tim].Out != SET){
		toggling_cnt(&lev_toggle_cnt, &last_input_state);
		
		if (lev_toggle_cnt >= count_val){
			wait_rst_but();
			return;
		}
		
		if (w_level == full){
			read_button();
			if (button == w_rst){
				button = off;
				soft_timer[well_err_tim].On = RESET;
				return;
			}
		}
	}
	soft_timer[well_err_tim].On = RESET;
	soft_timer[well_err_tim].Out = RESET;
	
	if (w_level == full) return;
	else{
		while (w_level == dry) well_level_work();
		wait_rst_but();
		if (button == w_rst){
			button = off;
			return;
		}
	}
}
//----toggling counter
void toggling_cnt(volatile uint8_t *cnt, Well_level *state){
	well_level_work();
	if (w_level != *state){
		*cnt++;
		
		if (*cnt % 2 == 0) *state = full;
		else *state = dry;
	}
}
//----wait only wrst but after 5time toggling of input
void wait_rst_but(void){
	while (button != w_rst){
		while (w_level == dry) well_level_work();
		read_button();
	}
	button = off;
}

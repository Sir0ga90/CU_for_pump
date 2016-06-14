#include "error.h"
#include "my_func.h"
#include "blink.h"
#include "dip_sw.h"
#include "level.h"
#include "sw_timer.h"
#include "dip_sw.h"

//--------------------------------------------------------------
#if (SwTimerCount>0)
 SW_TIMER volatile soft_timer[SwTimerCount]; //declaration of sw_tim's
#endif

static const uint8_t u_delay = 10;					// start delay in second's
static const uint8_t i_delay = 5;						// start delay
static const uint16_t well_err_del = 60;   // val while debug, must be 300
static const uint8_t out_of_range_del = 5;  // delay for filtering out of range values of U
static const uint8_t i_del_hi = 60;					// delay for filtering out of range value of I on HI-level
static const uint8_t i_del_low = 30;				// delay for filtering out of range values of I on LOW-level

uint8_t err_toggle_count = 0;

uint8_t delay_count = 0;

Val_on_disp on_display_value = e_no_val;

uint32_t last_val_before_err = 0;						// val that will be toggling on display after I-error

uint8_t work_flag = 0;
uint8_t st_flag = 0;
uint8_t start_flag_u = 0;
uint8_t start_flag_i = 0;
uint8_t flag_toggle_err_num = 0;
//-----------------------------------------------------------------
Error check_u(void){
	if ((U_val > (nominal_U + tres_U)) || 
			(U_val < (nominal_U - tres_U))   ){
				return E_U;
		}
	else 
		return E_OFF;
				
}
//-------------------------------------------------------------------
uint8_t flag_err_u = 0;

void filter_u_check(void){
	Error func_u_err = E_OFF;
	func_u_err = check_u();
	
	if (func_u_err != error_type && flag_err_u == RESET){
		start_time_filter(wk_u_tim, out_of_range_del);
		flag_err_u = SET;
	}
	
	else if (soft_timer[wk_u_tim].Out == SET && flag_err_u == SET){
		error_type = func_u_err;
		stop_time_filter(wk_u_tim);
		flag_err_u = RESET;
	}
	
}
//-------------------------------------------------------------------
Error check_i(void){
	if (I_val > ( tres_I + (tres_I * 0.2) ) ){ 						// + 20%
		return E_I;
	}
			
	else if (I_val < ( tres_I - (tres_I * 0.4) ) ){				// -40%
		return ELI;
	}
	
	else
		return E_OFF;
}

//-------------------------------------------------------------------
uint8_t flag_err_i = 0;

void filter_i_check(void){
	Error func_i_err = E_OFF;
	func_i_err = check_i();
	
	if (func_i_err != error_type && flag_err_i == RESET){
		if (func_i_err == E_I) start_time_filter(wk_i_tim, i_del_hi);
		else 
			if (func_i_err == ELI) start_time_filter(wk_i_tim, i_del_low);
		flag_err_i = SET;
	}
	
	else if (soft_timer[wk_i_tim].Out == SET && flag_err_i == SET){
		error_type = func_i_err;
		last_val_before_err = I_val;
		stop_time_filter(wk_i_tim);
		flag_err_i = RESET;
	}
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
				filter_u_check();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		case E_I:
			while (error_type == E_I){
				wait_i_rst_but();
			}
			HAL_TIM_Base_Stop_IT(&htim3);
			break;
			
		case ELI:
			while (error_type == ELI){
				wait_i_rst_but();
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
	else filter_i_check();
	
	if (start_flag_u == RESET){		
		start_u_i_check( motor );
	}
	else filter_u_check();
	
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
	}//if ( err_toggle_count <= 2 )
	
	else if (err_toggle_count <= 6){
		if (error_type != ELO){
			on_display_value = e_cnl;
			dig_to_disp( disp_err_chanel(), &on_display_value);
		}//if (error_type != ELO)
		
		else if (w_level == full && flag_toggle_err_num == RESET){
			on_display_value = e_r_timer;
			dig_to_disp( revers_timer(), &on_display_value);
		}//else if (w_level == full)
		
		else if (w_level == dry && flag_toggle_err_num == RESET){
			on_display_value = e_err_tp;
			dig_to_disp(error_type, &on_display_value);
		}//else if (w_level == dry)
		
		else if (flag_toggle_err_num == SET) { init_disp(); }							//clear the display
		
		if (err_toggle_count > 5 ){
			err_toggle_count = 0;
		}
	} //else if (err_toggle_count <= 6)
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
			}//if (work_flag == 0)
			
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
		} //if ( *motor == m_on )
}
//---------------------------------------------------------------------while well error delay & subfunc's
void while_well_err_delay(void){
	OnSwTimer(&soft_timer[well_err_tim], SWTIMER_MODE_WAIT_ON, well_err_del);
	soft_timer[well_err_tim].On = SET;
	
	static const uint8_t count_val = 3;						// 9 for 5-cycle w_level toggling (full -> dry = 1, dry -> full = +1 &s.o.) 
	uint8_t lev_toggle_cnt = 0;					// counter of toggling fail input
	Well_level last_input_state = full;
	
	while (soft_timer[well_err_tim].Out != SET){
		toggling_cnt(&lev_toggle_cnt, &last_input_state);
		
		if (lev_toggle_cnt >= count_val){
			flag_toggle_err_num = SET;
			wait_well_rst_but();
			return;
		}
		
		if (w_level == full){
			read_button();
			if (button == w_rst){
				button = off;
				soft_timer[well_err_tim].On = RESET;
				return;
			}
		}//if (w_level == full)
	}//while (soft_timer[well_err_tim].Out != SET)
	
	soft_timer[well_err_tim].On = RESET;
	soft_timer[well_err_tim].Out = RESET;
	
	if (w_level == full) return;
	else{
		while (w_level == dry) well_level_work();
		wait_well_rst_but();
		if (button == w_rst){
			button = off;
			return;
		}
	}
}
//----toggling counter
void toggling_cnt(uint8_t *cnt, Well_level *state){
	well_level_work();
	if (w_level != *state){
		(*cnt)++;
		*state = w_level;
	}
}
//----wait only wrst but after 5time toggling of input
void wait_well_rst_but(void){
	while (button != w_rst){
		while (w_level == dry) well_level_work();
		read_button();
	}
	button = off;
}
//----revers timer
uint32_t revers_timer(void){
	uint16_t func_delay = 0;
	uint16_t min = 0;
	uint8_t sec = 0;
	uint16_t disp_val = 0;
	
	func_delay = soft_timer[well_err_tim].LocalCount;
	min = func_delay / 60;
	if (min){
		sec = func_delay - min * 60;
		disp_val = min * 100 + sec;
	}
	else 
		disp_val = func_delay;
	return disp_val;
}
//--------------------------------------------------------------------filter timer's fun's
void start_time_filter(Timers tim, uint8_t del){
	OnSwTimer(&soft_timer[tim], SWTIMER_MODE_WAIT_ON, del);
	soft_timer[tim].On = SET;	
}
//---------------------------------------
void stop_time_filter(Timers tim){
	soft_timer[tim].On = RESET;
	soft_timer[tim].Out = RESET;
} 
//-----whait only wrst button after I error
void wait_i_rst_but(void){
	read_button();
		if (button == w_rst){
			error_type = E_OFF;
			button = off;
		}
}

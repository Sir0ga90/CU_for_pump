// by Sir0ga90, 23.04.2016

#include "dip_sw.h"
#include "level.h"
#include "logic.h"
//------------------------------------------------------------------------------
extern dip_sw dip;

//extern uint32_t U_val;
//extern uint32_t I_val;
extern double tres_U;
extern double tres_I;
extern Sens sen1;
extern Sens sen2;

extern uint8_t input_delay;
extern Logic logic;

void get_dip_sw_vals(void){
	
	dip.fail_blocking = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7);    		// on/off automatic blocking
	dip.lev1_logic    = HAL_GPIO_ReadPin(dip_lev_1_logic_GPIO_Port, dip_lev_1_logic_Pin);				// on
	dip.lev2_logic    = HAL_GPIO_ReadPin(dip_lev_2_logic_GPIO_Port, dip_lev_2_logic_Pin);
	dip.logic_inv     = HAL_GPIO_ReadPin(dip_logic_inv_GPIO_Port, dip_logic_inv_Pin);
	dip.input_delay  	= HAL_GPIO_ReadPin(dip_input_time_GPIO_Port, dip_input_time_Pin);
	dip.threshold_U   = HAL_GPIO_ReadPin(dip_prot_tres_u_GPIO_Port, dip_prot_tres_u_Pin);
	
	static uint16_t i;
	i = GPIOB->IDR & 960;        	// 960 = 0000 0011 1100 0000 - for bits of PORT_B wich are 4_current DIP_switches
	i >>= 6;											// 0000 0011 1100 0000 -> 0000 0000 0000 1111 = 6 time right-shifting
	dip.i_sw = i;
	
}
//------------------------------------------------------------------------------
//void fail_blocking_toggle(void){
//	switch (dip.fail_blocking){
//		case RESET:
//			break;
//		case SET:
//	}
//			
//}
////------------------------------------------------------------------------------
//void enable_func_vals(void){
//	if (dip.fail_blocking == SET)
//		
//}
////------------------------------------------------------------------------------
//void fail_blocking(void){
//	
//}
void set_U_tres(void){
	
	static uint8_t percent_15_U_ = 33;				//15% of 220
	static uint8_t percent_10_U_ = 22;				//10% of 220
	
 	if (dip.threshold_U == RESET)  		// because in ON state switch on the LOW level
		tres_U = (double)percent_15_U_;
	else
		tres_U = (double)percent_10_U_;
	
}
//---------------------------------------------------------------------------------
void set_I_tres(void){							
	tres_I = (double)(dip.i_sw + 1);
}
//---------------------------------------------------------------------------------
void set_logic_lev1(void){
	if (dip.lev1_logic == RESET){     // if rev. logic is ON (because in ON state switch on the LOW level)
		sen1.dry      = 0;							// revers logic
		sen1.in_water = 1;
	}
	else{															// else 
		sen1.dry      = 1;							// forward logic
		sen1.in_water = 0;
	}
		
}
//----------------------------------------------------------------------------------
void set_logic_lev2(void){
	if (dip.lev2_logic == RESET){     // if rev. logic is ON (RESET because in ON state switch on the LOW level)
		sen2.dry      = 0;							// revers logic
		sen2.in_water = 1;
	}
	else{															// else 
		sen2.dry      = 1;							// forward logic
		sen2.in_water = 0;
	}
		
}
//------------------------------------------------------------------------------------
void set_input_delay(void){
	if (dip.input_delay == RESET){
		input_delay = 10;
	}
	else
		input_delay = 5;
}
//------------------------------------------------------------------------------------
void set_work_logic(void){
	if (dip.logic_inv == RESET)
		logic = e_drain;
	else if (dip.logic_inv == SET)
		logic = e_pump;
	else
		logic = err;
}


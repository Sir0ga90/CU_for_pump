// by Sir0ga90, 23.04.2016

#include "dip_sw.h"

//------------------------------------------------------------------------------
extern Dip_sw dip;


void get_dip_sw_vals(void){
	
	dip.fail_blocking = HAL_GPIO_ReadPin(dip_blocking_fail_GPIO_Port, dip_blocking_fail_Pin);    		// on/off automatic blocking
	dip.lev1_logic    = HAL_GPIO_ReadPin(dip_lev_1_logic_GPIO_Port, dip_lev_1_logic_Pin);						// on - NC, 			off - NO
	dip.lev2_logic    = HAL_GPIO_ReadPin(dip_lev_2_logic_GPIO_Port, dip_lev_2_logic_Pin);						// on - NC, 			off - NO
	dip.logic_inv     = HAL_GPIO_ReadPin(dip_logic_inv_GPIO_Port, dip_logic_inv_Pin);								// on - drain, 		off - pump
	dip.input_delay  	= HAL_GPIO_ReadPin(dip_input_time_GPIO_Port, dip_input_time_Pin);							// on - 10sec,    off - 5sec
	dip.threshold_U   = HAL_GPIO_ReadPin(dip_prot_tres_u_GPIO_Port, dip_prot_tres_u_Pin);						// on - 15%,			off - 10%
	
	static uint16_t i = 0;
	i = GPIOB->IDR & 960;        	// 960 = 0000 0011 1100 0000 - for bits of PORT_B wich are 4_current DIP_switches
	i >>= 6;											// 0000 0011 1100 0000 -> 0000 0000 0000 1111 = 6 time right-shifting
	dip.i_sw = ~i;								// inverting to make bound accordig to real dip-switch (when dip switch is ON it is in LOW level)
	
}
//------------------------------------------------------------------------------
uint8_t set_well_lev_auto_blocking(void){
	return dip.fail_blocking == RESET ? (SET) : (RESET);
}
//--------------------------------------------------------------------------------
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
	tres_I = (double)(dip.i_sw + 1)*10; // *10 to increase accuracy on display
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
		logic = e_err;
}

// by Sir0ga90, 23.04.2016

#include "dip_sw.h"
//------------------------------------------------------------------------------
extern dip_sw dip;

//extern uint32_t U_val;
//extern uint32_t I_val;
extern uint16_t tres_U;
extern uint8_t tres_I;


void get_dip_sw_vals(void){
	
	dip.fail_blocking = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7);
	dip.lev1_logic    = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14);
	dip.lev2_logic    = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
	dip.lev_s_inv     = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	dip.delay_on_off  = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	dip.threshold_U   = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	
	static uint16_t i;
	i = GPIOB->IDR & 960;        	//960 = 0000 0011 1100 0000 - for bits of PORT_B wich are 4_current DIP_switches
	i >>= 6;											//0000 0011 1100 0000 -> 0000 0000 0000 1111
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
	if (dip.threshold_U == RESET)  		//because in ON state switch on the LOW level
		tres_U = 220 * 0.1;
}
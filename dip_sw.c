// by Sir0ga90, 23.04.2016

#include "dip_sw.h"
//------------------------------------------------------------------------------
extern dip_sw dip;

void get_func_vals(void){
	
	dip.fail_blocking = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7);
	dip.lev1_logic    = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14);
	dip.lev2_logic    = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
	dip.lev_s_inv     = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	dip.delay_on_off  = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	dip.threshold_U   = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	
	static uint16_t i;
	i = GPIOB->IDR & 960;
	i >>= 6;
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

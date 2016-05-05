// by Sir0ga90, 23.04.2016

#ifndef _DIP_SW_H_
#define _DIP_SW_H_

#include "stm32f0xx_hal.h"

//----------------------------------------------------------------------------------------------------------------------------
typedef struct dip_sw_st{
	
					 uint8_t fail_blocking  			:1;			// ON/OFF motor blocking after 5 fail-toggle		'1' - ON			'0' - OFF
					 uint8_t lev1_logic   				:1;			// NO/NC in normal state												'1' - NO			'0' - NC
					 uint8_t lev2_logic   				:1;			// NO/NC in normal state												'1' - NO			'0' - NC
					 uint8_t lev_s_inv		 				:1;			// level state inversion												'1' - ON			'0' - OFF
					 uint8_t delay_on_off 				:1;     // delay after changing level state: 						'1' - 10s			'0' - 5s
					 uint8_t threshold_U 					:1;     // U-protection: 																'1' - ±15%		'0' - ±10%
	
	__packed uint8_t i_sw									:4;			// 4 switches for current adjustment of motor
																								// from 1 to 16A
																								// in form of 4-bit number:
																								//	 			dip_sw:	7		8		9		10		value, A
								//																							
								//																								0		0		0		0 		1
								//																								1		0		0		0			2
								//																								0		1		0		0 		3
								//																								1		1		0		0			4
								//																								0		0		1		0 		5
								//																								1		0		1		0			6
								//																								0		1		1		0 		7
								//																								1		1		1		0			8
								//																								0		0		0		1 		9
								//																								1		0		0		1			10
								//																								0		1		0		1 		11
								//																								1		1		0		1			12
								//																								0		0		1		1 		13
								//																								1		0		1		1			14
								//																								0		1		1		1 		15
								//																								1		1		1		1			16
																								
}dip_sw;
//----------------------------------------------------------------------------------------------------------------------------

void get_func_vals(void);
void enable_func_vals(void);

#endif

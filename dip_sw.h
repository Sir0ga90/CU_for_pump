// by Sir0ga90, 23.04.2016

#ifndef _DIP_SW_H_
#define _DIP_SW_H_

#include "stm32f0xx_hal.h"

//----------------------------------------------------------------------------------------------------------------------------
typedef struct dip_sw_st{
	
					 uint8_t fail_blocking  			:1;			// ON/OFF motor blocking after 5 fail-toggle		'1' - ON			'0' - OFF
					 uint8_t lev1_logic   				:1;			// NO/NC in normal state												'1' - NO			'0' - NC
					 uint8_t lev2_logic   				:1;			// NO/NC in normal state												'1' - NO			'0' - NC
					 uint8_t logic_inv		 				:1;			// level state inversion												'1' - ON			'0' - OFF
					 uint8_t input_delay 					:1;     // delay after changing level state: 						'1' - 5s			'0' - 10s
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

inline void get_dip_sw_vals(void);
inline void enable_func_vals(void);
inline void set_U_tres(void);
inline void set_logic_lev1(void);
inline void set_logic_lev2(void);
inline void set_input_delay(void);
inline void set_work_logic(void);

#endif

//by Sir0ga, 12.04.2016

#ifndef LEVEL_H
#define LEVEL_H

#include "stm32f0xx_hal.h"

//---------------------------------------------------------------types
typedef enum {dry, full}Well_level;																			// levels in well

//enum Sen{sen_dry, sen_in_water};

typedef struct Sen_st{
	
						uint8_t in_water :1;
	__packed 	uint8_t dry      :1;
	
}Sens;

	
typedef struct Tank_st{
	
	uint8_t						error_level	: 1;							// if levels ON in wrong order: 
																									//			L2-ON,  	L1-OFF	
	uint8_t 					empty 			: 1;							// if 	L2-OFF, 	L1-OFF	
	uint8_t 					half_full 	: 1;							// if 	L2-OFF, 	L1-ON
	__packed uint8_t 	full 				: 1;							// if 	L2-ON,  	L1-ON 
																									// "__packed" - for aligning struct in memory
}Tank;
//---------------------------------------------------------------globals

//---------------------------------------------------------------extern

//---------------------------------------------------------------func_prototyps
inline Tank get_tank_level(void);
inline void get_well_level(void);
inline void level_indication(void);
inline void work(void);

#endif

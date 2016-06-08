//by Sir0ga, 12.04.2016

#ifndef LEVEL_H
#define LEVEL_H

#include "stm32f0xx_hal.h"
#include "sw_timer.h"

//---------------------------------------------------------------types
typedef enum {
	w_err,
	dry, 
	full
}Well_level;																			// levels in well


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
inline Tank 				get_tank_level(void);
inline Well_level 	get_well_level(void);
inline void 				level_indication(void);
inline void 				level_work(void);
inline void 				well_level_work(void);

extern volatile SW_TIMER soft_timer[SwTimerCount];

extern Well_level 	w_level;
extern Tank 				tank;
extern Sens					sen1;
extern Sens					sen2;

extern TIM_HandleTypeDef htim14;
extern uint8_t input_delay;

#endif

//by Sir0ga, 12.04.2016

#ifndef LEVEL_H
#define LEVEL_H

#include "stm32f0xx_hal.h"

//---------------------------------------------------------------types
typedef enum {error, empty, half_full, full}Level; 												// levels in tank
typedef enum {dry, water}Well_level;																			// levels in well

typedef struct Tank_st{
	
	uint8_t 					empty 		: 1;
	uint8_t 					half_full : 1;
	__packed uint8_t 	full 			: 1;							// "__packed" - for aligning struct in memory

}Tank;
//---------------------------------------------------------------globals

//---------------------------------------------------------------extern

//---------------------------------------------------------------func_prototyps
void read_level(void);

#endif

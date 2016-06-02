#ifndef LOGIC_H
#define LOGIC_H

#include "level.h"

//-------------------------------------------------------------------
typedef enum {
	m_off, 
	m_on
}Motor_state;

typedef enum{
	e_err, 
	e_pump, 
	e_drain
}Logic;

//-------------------------------------------------------------------
inline void pump			(Motor_state *);
inline void drain			(Motor_state *);
inline void work_logic(Motor_state *);
inline void motor_on	(Motor_state *);
inline void motor_off	(Motor_state *);
inline void stop			(Motor_state *);

//=====================================================================
extern Tank tank;
extern Logic logic;

#endif

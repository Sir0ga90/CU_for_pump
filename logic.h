#ifndef LOGIC_H
#define LOGIC_H

#include "level.h"


//-------------------------------------------------------------------
typedef enum {
	m_off, 
	m_on
}Motor_state;

#include "error.h"

typedef enum{
	e_err, 
	e_pump, 
	e_drain
}Logic;

//-------------------------------------------------------------------
inline void pump			(Motor_state *, Error *);
inline void drain			(Motor_state *, Error *);
inline void work_logic(Motor_state *, Error *);
inline void motor_on	(Motor_state *, Error *);
inline void motor_off	(Motor_state *);
inline void stop			(Motor_state *);

//=====================================================================
extern Tank tank;
extern Logic logic;
extern uint8_t work_flag;

#endif

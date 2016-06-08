#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum e_error{
	E_OFF,							// no error
	E_U = 555,					// voltage error
	E_I = 666,					// high current error
	ELI = 777,					// low current error
	ELO = 888,					// dry well
	ELL = 999						// wrong placment of sensors in tank
}Error;

#include "logic.h"

//-----------------------------------------------------------------------------------
typedef enum E_Val_on_disp{
	e_no_val,
	e_cnl,
	e_err_tp
}Val_on_disp;

//=====================================================================================
inline void check_u(void);
inline void check_i(void);
inline void check_well(void);
inline void check_lvls(Motor_state *motor);
inline void while_error_delay(Motor_state *motor);
inline void error_check(Motor_state *motor);
inline void err_disp_toggle(void);
inline void motor_start_delay(void);

inline void while_well_err_delay(void);
inline void toggling_cnt(volatile uint8_t *, Well_level *);
inline void wait_rst_but(void);

//=======================================================================================
extern uint32_t 		I_val;
extern uint32_t 		U_val;
extern double 			tres_U;
extern double 			tres_I;
extern Well_level 	w_level;
extern Error			 	error_type;
extern Tank 				tank;

static uint16_t nominal_U = 220;   		// 220 volts
extern uint8_t work_counter;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;

extern void start_u_i_check( Motor_state *motor);
extern uint8_t flag_auto_blocking;

#endif

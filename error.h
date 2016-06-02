#ifndef _ERROR_H_
#define _ERROR_H_

#include "logic.h"

enum error {
	E_OFF,							// no error
	E_U = 555,					// voltage error
	E_I = 666,					// high current error
	ELI = 777,					// low current error
	ELO = 888,					// dry well
	ELL = 999						// wrong placment of sensors in tank
};
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

//=======================================================================================
extern uint32_t 		I_val;
extern uint32_t 		U_val;
extern double 			tres_U;
extern double 			tres_I;
extern Well_level 	w_level;
extern enum error 	error_type;
extern Tank 				tank;

static uint16_t nominal_U = 220;   		// 220 volts
extern uint8_t work_counter;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;

#endif

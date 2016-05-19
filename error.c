// unpdb

#include "error.h"
#include "dip_sw.h"
#include "logic.h"
#include "level.h"

extern uint32_t 		I_val;
extern uint32_t 		U_val;
extern double 			tres_U;
extern double 			tres_I;
extern Well_level 	w_level;
extern enum error 	error_type;
extern Tank 				tank;

//------------------------------------------------------------------
void check_u(void){
		if ((U_val > (U_val + tres_U)) || 
				(U_val < (U_val - tres_U))) {
					
					stop();
					error_type = E_U;
		}
		else error_type = E_OFF;
}

//-------------------------------------------------------------------
void check_i(void){
	if (I_val > (tres_I + (tres_I * 0.2))){ 						// + 20%
		stop();
		error_type = E_I;
	}
			
	else if (I_val < (tres_I - (tres_I / 0.4))){				// -40%
		stop();
		error_type = ELI;
	}
	
	else error_type = E_OFF;
}

//-------------------------------------------------------------------
void check_well(void){
	if (w_level == dry){
		stop();
		error_type = ELO;
	}
	else error_type = E_OFF;
}
//-------------------------------------------------------------------
void check_lvls(void){
	if (tank.error_level == SET){
		stop();
		error_type = ELL;
	}
		
}
//-------------------------------------------------------------------
void error_check(void){
	check_u();
	check_i();
	check_well();
	check_lvls();
}

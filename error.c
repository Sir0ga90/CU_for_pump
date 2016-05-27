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

static uint16_t nominal_U = 220;   		// 220 volts

//------------------------------------------------------------------
void check_u(void){
		if ((U_val > (nominal_U + tres_U)) || 
				(U_val < (nominal_U - tres_U))) {
					
					error_type = E_U;
					
		}
		else 
			error_type = E_OFF;
				
}
//-------------------------------------------------------------------
void check_i(void){
	if (I_val > ( tres_I + (tres_I * 0.2) ) ){ 						// + 20%
		error_type = E_I;
	}
			
	else if (I_val < ( tres_I - (tres_I * 0.4) ) ){				// -40%
		error_type = ELI;
	}
	
	else if (error_type != E_U) error_type = E_OFF;

}

//-------------------------------------------------------------------
void check_well(void){
	if (w_level == dry){
		error_type = ELO;
	}
}
//-------------------------------------------------------------------
void check_lvls(void){
	if (tank.error_level == SET){
		stop();
		error_type = ELL;
	}
}
//-------------------------------------------------------------------
void while_error_delay(void){
	switch (error_type) {
		case E_U:
			stop();
			while (error_type == E_U){
				check_u();
			}
			
			break;
			
		case E_I:
			stop();
			while (error_type == E_I){
				check_i();
			}

			break;
			
		case ELI:
			stop();
			while (error_type == ELI){
				check_i();
			}

			break;
			
		case ELO:
			stop();
			while (w_level == dry){
			get_well_level();
			}

			break;
			
		case ELL:
			stop();
			while (tank.error_level == SET){
			level_work();
			}

			break;
			
		default:
			error_type = E_OFF;
			break;
	}
}
//-------------------------------------------------------------------
void error_check(void){
	check_u();
	check_i();
	check_well();
	check_lvls();
	if (error_type != E_OFF) while_error_delay();
}


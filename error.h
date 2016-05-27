#ifndef _ERROR_H_
#define _ERROR_H_

#include "dip_sw.h"
#include "blink.h"

enum error {
	E_OFF,							// no error
	E_U = 555,					// voltage error
	E_I = 666,					// high current error
	ELI = 777,					// low current error
	ELO = 888,					// dry well
	ELL = 999						// wrong placment of sensors in tank
};

//----------------------------------------------------------------------------------
inline void check_u(void);
inline void check_i(void);
inline void check_well(void);
inline void check_lvls(void);
inline void while_error_delay(void);
inline void error_check(void);

#endif
